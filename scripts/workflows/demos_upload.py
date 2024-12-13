import os, sys, re, aiohttp, \
       argparse, asyncio, json, shutil

# Add the parent directory to sys.path
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import utils as utils
import classes.class_gh as gh

def is_status_success(params):
    return params['status'] == 'Success'

def find_directories_with_regex(base_path, regex_pattern):
    """
    Finds and lists all directories matching the regex pattern in the provided path.

    Args:
        base_path (str): The path where to search for directories.
        regex_pattern (str): The regex pattern to match directory names.

    Returns:
        list: A list of directories matching the regex pattern.
    """
    matching_dirs = []
    regex = re.compile(regex_pattern, re.IGNORECASE)  # Case-insensitive regex

    # Walk through the directory structure
    for root, dirs, files in os.walk(base_path):
        for dir_name in dirs:
            if regex.match(dir_name):  # Match directory names against the regex
                matching_dirs.append(os.path.join(root, dir_name))

    return matching_dirs

def package_and_upload_dbp(package_in, package_out, **kwargs):
    package_path = None

    package_in = os.path.join(
        package_in, 'project'
    )

    ## First pack the DEMO
    result = utils.fetch_json_from_output(
        utils.capture_stdout(
            utils.call_python_script,
            'tmp/click_zip_packer/src/main.py',
            'pack', '--input', package_in,
            '--output', package_out
        )
    )
    utils.raise_exception(is_status_success(result['params']))
    package_path = result['params']['output_path']

    ## Then upload the DEMO
    result = utils.fetch_json_from_output(
        utils.capture_stdout(
            utils.call_python_script,
            'tmp/click_zip_packer/src/main.py',
            'upload', '--input', result['params']['output_path'],
            '--username', kwargs['username'],
            '--password', kwargs['password'],
            '--url', kwargs['url']
        )
    )
    if 'hydra:description' in result:
        package_path = None
        print("%sUpload failed for %s with error: %s\n" % (utils.Colors.FAIL, package_out, result['hydra:description']))
    else:
        print("%sUpload successful for %s\n" % (utils.Colors.OKGREEN, package_out))
        package_path = package_out
        utils.raise_exception(is_status_success(result['params']))

    return package_path

async def main(gh_instance, demos, args, upload_api):
    for demo in demos:
        print("\n\n%sRunning for %s" % (utils.Colors.UNDERLINE, demo))
        package = upload_api(
            package_in = demo,
            package_out = os.path.join(
                'output', utils.fetch_after_last_separator(
                    demo, utils.get_system_separator()
                )
            ),
            username = args.packer_username,
            password = args.packer_password,
            url = args.packer_url
        )

        if package:
            async with aiohttp.ClientSession() as session:
                await gh_instance.asset_upload(session, package, args.gh_tag)

if __name__ == "__main__":
    # First, check for arguments passed
    def str2bool(v):
        if isinstance(v, bool):
            return v
        if v.lower() in ('yes', 'true', 't', 'y', '1'):
            return True
        elif v.lower() in ('no', 'false', 'f', 'n', '0'):
            return False
        else:
            raise argparse.ArgumentTypeError('Boolean value expected.')

    # Then, check for arguments passed
    parser = argparse.ArgumentParser(description='')
    parser.add_argument("gh_repo", help="Github repository name, e.g., 'username/repo'", type=str)
    parser.add_argument("gh_token", help="GitHub Token", type=str)
    parser.add_argument("gh_tag", help="GitHub tag name", type=str)
    parser.add_argument("packer_download_link", type=str, help="Download link for packer scripts.")
    parser.add_argument("packer_username", help="DBP username.")
    parser.add_argument('packer_password', type=str, help='DBP password.')
    parser.add_argument('packer_url', type=str, help='Upload URL for packages.')
    parser.add_argument('demo', type=str, nargs='?', help='Specify a demo name or regex (optional).', default=None)
    parser.add_argument('--all_demos', type=str2bool, help='If True - will pack and upload all demos found.', default=False)

    ## Parse the arguments
    args = parser.parse_args()

    ## Copies current demos for further editing
    demo_dir = os.path.join(os.getcwd(), 'demos_modified')
    utils.copy_files_and_folders(os.path.join(os.getcwd(), 'demos'), demo_dir)

    ## Add category to manifest if not present already
    ## Should be handled automacially by NECTO at this point
    # utils.manifests_add_category(demo_dir, 'NECTO Demo')

    ## If ever needed to convert legacy structured demos to new structure,
    ## this is an example call of the function.
    # utils.call_python_script('scripts/dbp/main.py')

    ## Set appropriate copyright year
    utils.replace_copyright_year(demo_dir)

    ## If all demos requested, get full list
    demo_regex = args.demo
    if args.all_demos:
        demo_regex = '^demo.+'

    utils.raise_exception(demo_regex)
    demos = find_directories_with_regex(demo_dir, demo_regex)

    ## Initialize github instance for current repository
    tag = args.gh_tag
    if not args.gh_tag:
        tag = 'latest'
    gh_instance = gh.repo(
        repo = args.gh_repo,
        token = args.gh_token,
        release_id = tag
    )

    ## Remove any previous output
    ## Useful for local runs, not container runs
    if os.path.exists(os.path.join(os.getcwd(), 'output')):
        shutil.rmtree(os.path.join(os.getcwd(), 'output'))

    ## Download packer scripts
    success = utils.download_file(args.packer_download_link, "tmp/packer.7z")
    if success:
        success = utils.extract_7z_file("tmp/packer.7z", "tmp")
        if success:
            print("%sDownload completed.\n\n" % utils.Colors.OKBLUE)
            asyncio.run(main(gh_instance, demos, args, package_and_upload_dbp))
        else:
            print("%sExtraction failed." % utils.Colors.FAIL)
    else:
        print("%sDownload failed." % utils.Colors.FAIL)
