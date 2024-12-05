import os, re, argparse

import utils as utils

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

def package_and_upload(package_in, package_out, username, password, url):
    ## First pack the DEMO
    result = utils.fetch_json_from_output(
        utils.capture_stdout(
            utils.call_python_script,
            'tmp/packer/main.py',
            'pack', '--input', package_in,
            '--output', package_out
        )
    )
    utils.raise_exception(is_status_success(result['params']))

    ## Then upload the DEMO
    result = utils.fetch_json_from_output(
        utils.capture_stdout(
            utils.call_python_script,
            'tmp/packer/main.py',
            'upload', '--input', result['params']['output_path'],
            '--username', username,
            '--password', password,
            '--url', url
        )
    )
    if 'hydra:description' in result:
        print("%sUpload failed for %s with error: %s" % (utils.Colors.FAIL, package_out, result['hydra:description']))
    else:
        print("%sUpload successful for %s" % (utils.Colors.OKBLUE, package_out))
        utils.raise_exception(is_status_success(result['params']))

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
    parser.add_argument("packer_download_link", type=str, help="Download link for packer scripts.")
    parser.add_argument("packer_username", help="DBP username.")
    parser.add_argument('packer_password', type=str, help='DBP password.')
    parser.add_argument('packer_url', type=str, help='Upload URL for packages.')
    parser.add_argument('demo', type=str, nargs='?', help='Specify a demo name or regex (optional).', default=None)
    parser.add_argument('--all_demos', type=str2bool, help='If True - will pack and upload all demos found.', default=False)
    parser.add_argument('--dbp', type=str2bool, help='If True - will pack and upload all demos found to new DBP.', default=True)

    ## Parse the arguments
    args = parser.parse_args()

    ## If true, converts current demos to new DBP structure
    demo_dir = os.path.join(os.getcwd(), 'demos')
    if (args.dbp):
        demo_dir = os.path.join(os.getcwd(), 'demos_dbp')
        utils.call_python_script('scripts/dbp/main.py')

    ## If all demos requested, get full list
    demo_regex = args.demo
    if args.all_demos:
        demo_regex = '^demo.+'

    utils.raise_exception(demo_regex)
    demos = find_directories_with_regex('demos_dbp', demo_regex)

    ## Download packer scripts
    success = utils.download_file(args.packer_download_link, "tmp/packer.7z")
    if success:
        success = utils.extract_7z_file("tmp/packer.7z", "tmp")
        if success:
            print("%sDownload completed." % utils.Colors.OKBLUE)
            for demo in demos:
                print("%sRunning for %s" % (utils.Colors.UNDERLINE, demo))
                package_and_upload(
                    os.path.join(
                        demo, 'project'
                    ),
                    os.path.join(
                        'output', utils.fetch_after_last_separator(
                            demo, utils.get_system_separator()
                        )
                    ),
                    args.packer_username,
                    args.packer_password,
                    args.packer_url
                )
        else:
            print("%sExtraction failed." % utils.Colors.FAIL)
    else:
        print("%sDownload failed." % utils.Colors.FAIL)
