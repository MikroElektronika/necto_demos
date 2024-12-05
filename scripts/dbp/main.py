import os, shutil

import restructure_demos as demos
import transform_manifests as manifest

def copy_base_directory(base_path):
    """
    Copies the entire base directory to a new location before making changes.

    Args:
        base_path (str): The path to the base directory.

    Returns:
        str: The path to the copied directory.
    """
    try:
        copied_path = f"{base_path}_dbp"
        if os.path.exists(copied_path):
            shutil.rmtree(copied_path)  # Remove the existing copy if it already exists
        shutil.copytree(base_path, copied_path)
        print(f"Copied '{base_path}' to '{copied_path}'.")
        return copied_path
    except Exception as e:
        print(f"Error while copying base directory: {e}")
        raise

if __name__ == "__main__":
    base_path = "demos"
    # Copy the base directory first
    copied_path = copy_base_directory(base_path)
    manifest.find_and_process_manifest_files(copied_path)
    demos.restructure_directories_recursively(copied_path)
    demos.move_manifest_to_project_folder(copied_path)
    demos.move_resources_to_project(copied_path)
    demos.update_versions_in_details(copied_path)
