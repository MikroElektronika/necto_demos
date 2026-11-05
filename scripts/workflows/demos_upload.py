import os, sys, re, argparse, json, \
       shutil, hashlib, uuid

from datetime import datetime, timezone

# Add the parent directory to sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import utils as utils
import classes.class_gh as gh
import classes.class_es as es_cls

## Name of the release asset which keeps track of packed demo hashes.
METADATA_ASSET = 'metadata.json'

## Default values for documents which are not indexed yet.
## Taken over from documents previously created by DBP.
DEFAULT_DOC_FIELDS = {
    'author': {
        'fullName': 'MIKROE',
        'id': 'd94fa629-63d1-4804-9ee3-df710fa464a5'
    },
    'readme': None,
    'clicks': [],
    'categories': [],
    'class': 'INTERNAL',
    'downloadCount': 0
}

## Manifest fields copied directly into indexed document.
MANIFEST_DOC_FIELDS = [
    'display_name', 'name', 'type', 'version', 'demo', 'contents', 'link_github'
]

def is_status_success(params):
    return params['status'] == 'Success'

def find_directories_with_regex(base_path, regex_pattern):
    """
    Finds all demo directories (containing project/manifest.json)
    whose name matches the regex pattern.
    """
    matching_dirs = []
    regex = re.compile(regex_pattern, re.IGNORECASE)  # Case-insensitive regex

    for root, dirs, files in os.walk(base_path):
        for dir_name in dirs:
            dir_path = os.path.join(root, dir_name)
            if regex.match(dir_name) and \
               os.path.isfile(os.path.join(dir_path, 'project', 'manifest.json')):
                matching_dirs.append(dir_path)

    return sorted(matching_dirs)

def hash_directory_contents(directory):
    """ Content hash of a directory - independent of timestamps and archive format. """
    digest = hashlib.sha256()

    for root, dirs, files in os.walk(directory):
        dirs.sort()
        for filename in sorted(files):
            file_path = os.path.join(root, filename)
            relative_path = os.path.relpath(file_path, directory).replace(os.sep, '/')
            digest.update(relative_path.encode('utf-8'))
            digest.update(b'\0')
            with open(file_path, 'rb') as file:
                for chunk in iter(lambda: file.read(1024 * 1024), b''):
                    digest.update(chunk)
            digest.update(b'\0')

    return digest.hexdigest()

def package_demo(package_in, package_out):
    """ Packs the demo using packer scripts. Returns path to created archive. """
    result = utils.fetch_json_from_output(
        utils.capture_stdout(
            utils.call_python_script,
            'tmp/click_zip_packer/src/main.py',
            'pack', '--input', os.path.join(package_in, 'project'),
            '--output', package_out
        )
    )
    utils.raise_exception(is_status_success(result['params']))
    return result['params']['output_path']

def fetch_indexed_doc(es, index_name, name):
    """ Returns (doc_id, source) of document with exact 'name', or (None, None). """
    response = es.search(
        index=index_name,
        body={
            'size': 50,
            'query': {
                'match_phrase': {
                    'name': name
                }
            }
        }
    )
    for hit in response['hits']['hits']:
        if hit['_source'].get('name') == name:
            return hit['_id'], hit['_source']
    return None, None

def build_doc(manifest, indexed_doc, link, published, package_changed):
    if indexed_doc:
        doc = dict(indexed_doc)
    else:
        doc = json.loads(json.dumps(DEFAULT_DOC_FIELDS))
        doc['id'] = str(uuid.uuid4())

    for field in MANIFEST_DOC_FIELDS:
        if field in manifest:
            doc[field] = manifest[field]

    doc['clicks'] = manifest.get('contents', {}).get('clicks', doc.get('clicks', []))
    doc['link'] = link
    doc['published'] = published
    doc['package_changed'] = package_changed
    doc['_type'] = '_doc'

    return doc

def main(args, gh_instance, demos, es):
    colors = utils.Colors
    published_now = datetime.now(timezone.utc).replace(microsecond=0).isoformat()

    existing_metadata = gh_instance.asset_download_json(METADATA_ASSET)
    if existing_metadata is None:
        print(f"{colors.WARNING}{METADATA_ASSET} not found in release - all selected demos will be (re)uploaded.{colors.ENDC}")
        existing_metadata = {}
    new_metadata = dict(existing_metadata)

    index_names = [i for i in (args.index_names or '').split('|') if i]
    any_asset_changed = False
    summary = []

    for demo in demos:
        print(f"\n\n{colors.UNDERLINE}Running for {demo}{colors.ENDC}")

        with open(os.path.join(demo, 'project', 'manifest.json'), 'r', encoding='utf-8') as f:
            manifest = json.load(f)

        asset_name = f"{manifest['name']}.zip"

        ## Hash original sources, so copyright year replacement does not trigger changes
        source_dir = os.path.join(os.getcwd(), 'demos', os.path.relpath(demo, os.path.join(os.getcwd(), 'demos_modified')))
        demo_hash = hash_directory_contents(source_dir)

        previous = existing_metadata.get(asset_name)
        existing_asset = gh_instance.fetch_asset(asset_name)
        content_changed = bool(previous) and previous.get('hash') != demo_hash
        asset_changed = (not previous) or content_changed or (not existing_asset)

        if asset_changed:
            print(f"{colors.WARNING}Changes detected for {asset_name}.{colors.ENDC}")
            package_path = package_demo(
                demo,
                os.path.join('output', utils.fetch_after_last_separator(demo, utils.get_system_separator()))
            )
            ## Asset name must stay stable, so the indexed download link stays valid
            if os.path.basename(package_path) != asset_name:
                renamed_path = os.path.join(os.path.dirname(package_path), asset_name)
                shutil.move(package_path, renamed_path)
                package_path = renamed_path
            if args.dry_run:
                print(f"{colors.OKCYAN}[Dry run] Would replace release asset {asset_name}.{colors.ENDC}")
            else:
                gh_instance.asset_upload(package_path, replace=True)
                any_asset_changed = True
            new_metadata[asset_name] = {
                'name': manifest['name'],
                'version': manifest['version'],
                'hash': demo_hash
            }
        else:
            print(f"{colors.OKGREEN}No changes for {asset_name} - release asset kept.{colors.ENDC}")

        asset = gh_instance.fetch_asset(asset_name)
        if asset:
            link = asset['browser_download_url']
        else:
            link = f"https://github.com/{gh_instance.repo}/releases/download/{gh_instance.release['tag_name']}/{asset_name}"

        ## Map index documents to the GitHub asset
        for index_name in index_names:
            doc_id, indexed_doc = fetch_indexed_doc(es, index_name, manifest['name'])

            indexed_version = indexed_doc.get('version') if indexed_doc else None
            version_changed = indexed_version != manifest['version']
            link_changed = (indexed_doc or {}).get('link') != link

            if indexed_doc and not (asset_changed or version_changed or link_changed):
                print(f"{colors.OKGREEN}[{index_name}] {manifest['name']} is up to date.{colors.ENDC}")
                continue

            if indexed_doc and content_changed and not version_changed:
                print(f"{colors.WARNING}WARNING: [{index_name}] {manifest['name']} content changed but manifest version "
                      f"is still {manifest['version']} - NECTO will not offer it as an update. "
                      f"Bump version in manifest.json and changelog.md.{colors.ENDC}")

            package_changed = (not indexed_doc) or version_changed or asset_changed
            published = published_now if package_changed or not indexed_doc else indexed_doc.get('published', published_now)
            doc = build_doc(manifest, indexed_doc, link, published, package_changed)
            if not doc_id:
                doc_id = doc['id']

            action = 'CREATE' if not indexed_doc else 'UPDATE'
            summary.append(f"[{index_name}] {action} {manifest['name']} {indexed_version} -> {manifest['version']}")

            if args.dry_run:
                print(f"{colors.OKCYAN}[Dry run] [{index_name}] Would {action.lower()} {doc_id}:\n{json.dumps(doc, indent=4)}{colors.ENDC}")
                continue

            resp = es.index(index=index_name, doc_type=None, id=doc_id, body=doc)
            print(f"{colors.OKGREEN}[{index_name}] {resp['result']} {resp['_id']} - {doc['link']}{colors.ENDC}")

    ## Keep track of uploaded content hashes
    if any_asset_changed:
        os.makedirs('output', exist_ok=True)
        metadata_path = os.path.join('output', METADATA_ASSET)
        with open(metadata_path, 'w', encoding='utf-8') as f:
            json.dump(new_metadata, f, indent=4)
        gh_instance.asset_upload(metadata_path, replace=True)

    print(f"\n\n{colors.HEADER}Indexing summary:{colors.ENDC}")
    for line in summary or ['Nothing to index.']:
        print(f"  {line}")

if __name__ == "__main__":
    def str2bool(v):
        if isinstance(v, bool):
            return v
        if v.lower() in ('yes', 'true', 't', 'y', '1'):
            return True
        elif v.lower() in ('no', 'false', 'f', 'n', '0', ''):
            return False
        else:
            raise argparse.ArgumentTypeError('Boolean value expected.')

    parser = argparse.ArgumentParser(description='Pack demos, replace changed GitHub release assets and map ES index documents to them.')
    parser.add_argument("gh_repo", help="Github repository name, e.g., 'username/repo'", type=str)
    parser.add_argument("gh_token", help="GitHub Token", type=str)
    parser.add_argument("gh_tag", help="GitHub tag name", type=str)
    parser.add_argument("packer_download_link", type=str, help="Download link for packer scripts.")
    parser.add_argument('demo', type=str, nargs='?', help='Specify a demo name or regex (optional).', default=None)
    parser.add_argument('--all_demos', type=str2bool, help='If True - will pack and upload all demos found.', default=False)
    parser.add_argument('--index_names', type=str, help='Target ES index names divided by |. Empty - no indexing.', default='')
    parser.add_argument('--dry_run', type=str2bool, help='If True - only report what would be uploaded/indexed.', default=False)

    args = parser.parse_args()

    ## Copies current demos for further editing
    demo_dir = os.path.join(os.getcwd(), 'demos_modified')
    if os.path.exists(demo_dir):
        shutil.rmtree(demo_dir)
    utils.copy_files_and_folders(os.path.join(os.getcwd(), 'demos'), demo_dir)

    ## Set appropriate copyright year
    utils.replace_copyright_year(demo_dir)

    ## If all demos requested, get full list
    demo_regex = args.demo
    if args.all_demos:
        demo_regex = '^demo.+'

    utils.raise_exception(demo_regex)
    demos = find_directories_with_regex(demo_dir, demo_regex)
    utils.raise_exception(demos)
    print("%sSelected demos:\n  %s" % (utils.Colors.OKBLUE, '\n  '.join(demos)))

    ## Initialize github instance for current repository
    gh_instance = gh.repo(
        repo = args.gh_repo,
        token = args.gh_token,
        release_id = args.gh_tag or 'latest'
    )

    ## Connect to ES only if indexing is requested
    es = None
    if args.index_names:
        es = es_cls.index.init(os.environ['ES_HOST'], os.environ['ES_USER'], os.environ['ES_PASSWORD'])

    ## Remove any previous output
    if os.path.exists(os.path.join(os.getcwd(), 'output')):
        shutil.rmtree(os.path.join(os.getcwd(), 'output'))

    ## Download packer scripts
    if not utils.download_file(args.packer_download_link, "tmp/packer.7z"):
        raise SystemExit("%sDownload failed." % utils.Colors.FAIL)
    if not utils.extract_7z_file("tmp/packer.7z", "tmp"):
        raise SystemExit("%sExtraction failed." % utils.Colors.FAIL)
    print("%sDownload completed.\n\n" % utils.Colors.OKBLUE)

    main(args, gh_instance, demos, es)
