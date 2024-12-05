import os, json

def transform_json(original_json):
    """
    Transforms the given JSON content into the required structure.

    Args:
        original_json (dict): The original JSON content.

    Returns:
        dict: The transformed JSON content.
    """
    last_version = original_json.get("version", "")
    if len(last_version.split('.')) > 3:
        version = '3.0.0'
    else:
        version = last_version
    return {
        "contents": {
            "clicks": [],
            "examples": [
                {
                    "path": original_json.get("display_name", "")
                }
            ]
        },
        "display_name": original_json.get("display_name", ""),
        "name": original_json.get("name", ""),
        "type": "project",
        "version": version
    }

def transform_json_from_file(input_file_path, output_file_path):
    """
    Reads a JSON file, transforms it into the required structure, and saves the output to another file.

    Args:
        input_file_path (str): Path to the input JSON file.
        output_file_path (str): Path to save the transformed JSON file.
    """
    with open(input_file_path, 'r') as file:
        original_json = json.load(file)

    if 'type' in original_json:
        if original_json['type'] != 'project':
            transformed_json = transform_json(original_json)

            with open(output_file_path, 'w') as file:
                json.dump(transformed_json, file, indent=4)

            print(f"JSON '{output_file_path}' transformed.")
            return

    print(f"Nothing to transform for '{output_file_path}'.")

def find_and_process_manifest_files(root_path):
    """
    Searches for 'manifest.json' files in the given directory and its subdirectories,
    transforms their content, and saves the result as 'transformed_manifest.json' in the same directory.

    Args:
        root_path (str): The root directory to start searching from.
    """
    for dirpath, _, filenames in os.walk(root_path):
        for filename in filenames:
            if filename == "manifest.json":
                input_file_path = os.path.join(dirpath, filename)
                output_file_path = input_file_path
                try:
                    transform_json_from_file(input_file_path, output_file_path)
                except Exception as e:
                    print(f"Failed to process '{input_file_path}': {e}")

# Example usage
if __name__ == "__main__":
    root_directory = "demos"
    find_and_process_manifest_files(root_directory)
