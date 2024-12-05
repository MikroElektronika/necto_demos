import os, re, json, shutil

def lowercase_files_and_folders(directory_path):
    """
    Lowercase all file and folder names in the provided directory path, except 'CMakeLists.txt'.
    """
    try:
        # Rename folders and files from bottom up
        for root, dirs, files in os.walk(directory_path, topdown=False):
            # Rename files
            for file_name in files:
                if file_name == "CMakeLists.txt":
                    continue
                old_path = os.path.join(root, file_name)
                new_path = os.path.join(root, file_name.lower())
                if old_path != new_path:
                    os.rename(old_path, new_path)
                    print(f"Renamed file: {old_path} -> {new_path}")

            # Rename directories
            for dir_name in dirs:
                old_path = os.path.join(root, dir_name)
                new_path = os.path.join(root, dir_name.lower())
                if old_path != new_path:
                    os.rename(old_path, new_path)
                    print(f"Renamed folder: {old_path} -> {new_path}")
    except Exception as e:
        print(f"Error while lowercasing files and folders: {e}")

def move_manifest_to_project_folder(base_dir):
    """
    Recursively find all 'manifest.json' files in the directory and move them to their respective 'project' folder,
    ensuring no nested 'project' folders are created.

    Args:
        base_dir (str): The base directory to start the search.
    """
    for root, _, files in os.walk(base_dir):
        for file in files:
            if file == "manifest.json":
                manifest_path = os.path.join(root, file)

                # Check if the manifest is already in a 'project' folder
                if os.path.basename(root) == "project":
                    print(f"'{manifest_path}' is already in the 'project' folder. Skipping...")
                    continue

                # Determine the project folder path
                project_folder = os.path.join(root, "project")

                # Create the 'project' folder if it doesn't exist
                os.makedirs(project_folder, exist_ok=True)

                # Move the 'manifest.json' to the 'project' folder
                new_path = os.path.join(project_folder, file)
                shutil.move(manifest_path, new_path)

                print(f"Moved '{manifest_path}' to '{new_path}'")

def update_versions_in_details(base_directory):
    for root, dirs, files in os.walk(base_directory):
        # Find the manifest.json file
        if 'manifest.json' in files:
            manifest_path = os.path.join(root, 'manifest.json')

            # Read and parse the manifest.json file
            with open(manifest_path, 'r') as manifest_file:
                try:
                    manifest_data = json.load(manifest_file)
                    version = manifest_data.get('version', None)
                    if version:
                        # Path to details.md in the 'resources' folder
                        details_path = os.path.join(root, 'resources', 'details.md')

                        # Update details.md if it exists
                        if os.path.exists(details_path):
                            with open(details_path, 'r') as details_file:
                                details_content = details_file.readlines()

                            # Use regex to find the line with the version and replace it
                            updated_content = ''
                            for line in details_content:
                                if '**Version**' in line:
                                    updated_content += f'- **Version**     : {version}\n'
                                else:
                                    updated_content += line

                            # Write the updated content back to details.md
                            with open(details_path, 'w') as details_file:
                                details_file.write(updated_content)
                            print(f'Updated {details_path} with version {version}')
                        else:
                            print(f'Details file not found for {manifest_path}')
                    else:
                        print(f'No version found in {manifest_path}')
                except json.JSONDecodeError:
                    print(f'Error decoding JSON from {manifest_path}')

def move_resources_to_project(base_path):
    """
    Moves 'resources' folders to their respective 'project' folder.
    """
    try:
        for root, dirs, _ in os.walk(base_path, topdown=True):
            if 'resources' in dirs and 'project' not in root:
                resources_path = os.path.join(root, 'resources')

                # Determine the respective 'project' folder
                project_folder = os.path.join(root, "project")

                # If the 'project' folder exists, move the 'resources' folder into it
                if os.path.isdir(project_folder):
                    target_path = os.path.join(project_folder, 'resources')
                    shutil.move(resources_path, target_path)
                    print(f"Moved: {resources_path} -> {target_path}")
                else:
                    print(f"Skipped moving {resources_path}: Target 'project' folder does not exist.")
    except Exception as e:
        print(f"Error while moving resources folders: {e}")

def rename_example_to_project(base_path):
    """
    Renames all 'example' folders to 'project'.
    """
    try:
        for root, dirs, _ in os.walk(base_path, topdown=False):
            for dir_name in dirs:
                if dir_name == "example":
                    old_path = os.path.join(root, dir_name)
                    new_path = os.path.join(root, "project")
                    os.rename(old_path, new_path)
                    print(f"Renamed folder: {old_path} -> {new_path}")
    except Exception as e:
        print(f"Error while renaming 'example' folders to 'project': {e}")

def restructure_directories_recursively(base_path):
    """
    Recursively restructure directories to move files from 'Examples/example/'
    to the parent directory, remove 'Examples' folders, rename 'example' folders to 'project',
    and lowercase files and folders.
    """
    try:
        # Traverse the directory tree recursively
        for root, dirs, _ in os.walk(base_path, topdown=False):
            if "Examples" in dirs:
                examples_path = os.path.join(root, "Examples")

                for example_subdir in os.listdir(examples_path):
                    example_path = os.path.join(examples_path, example_subdir)
                    target_path = os.path.join(root, example_subdir.lower())

                    if os.path.isdir(example_path):
                        # Ensure the target directory exists
                        os.makedirs(target_path, exist_ok=True)

                        # Move files and subdirectories to the target directory
                        for item in os.listdir(example_path):
                            source_item = os.path.join(example_path, item)
                            if 'CMakeLists.txt' in item:
                                target_item = os.path.join(target_path, item)
                            else:
                                target_item = os.path.join(target_path, item.lower())
                            shutil.move(source_item, target_item)
                            print(f"Moved: {source_item} -> {target_item}")

                # Remove the Examples folder
                shutil.rmtree(examples_path)
                print(f"Removed: {examples_path}")

        # Lowercase all file and folder names in the directory tree
        lowercase_files_and_folders(base_path)

        # Rename 'example' folders to 'project'
        rename_example_to_project(base_path)

    except Exception as e:
        print(f"Error while restructuring directories: {e}")

if __name__ == "__main__":
    base_path = "demos"
    restructure_directories_recursively(base_path)
    move_manifest_to_project_folder(base_path)
    move_resources_to_project(base_path)
    update_versions_in_details(base_path)
