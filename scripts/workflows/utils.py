import os, io, sys, json, py7zr, requests, subprocess

class Colors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    WARNING = '\033[93m'
    FAIL = '\033[91m'
    ENDC = '\033[0m'
    BOLD = '\033[1m'
    UNDERLINE = '\033[4m'

def download_file(url, output):
    """
    Downloads a file from the given URL and saves it to the specified path.

    Args:
        url (str): The URL of the file to download.
        output (str): The local file path where the file will be saved.

    Returns:
        bool: True if the file was downloaded successfully, False otherwise.
    """
    try:
        # Ensure the directory exists
        os.makedirs(os.path.dirname(output), exist_ok=True)

        with requests.get(url, stream=True) as response:
            response.raise_for_status()  # Raise an HTTPError for bad responses
            with open(output, 'wb') as file:
                for chunk in response.iter_content(chunk_size=8192):
                    file.write(chunk)
        print(f"File downloaded successfully: {output}")
        return True
    except requests.exceptions.RequestException as e:
        print(f"Error downloading file: {e}")
        return False
    except Exception as e:
        print(f"Unexpected error: {e}")
        return False

def extract_7z_file(file_path, output_dir):
    """
    Extracts a .7z file to the specified output directory.

    Args:
        file_path (str): The path to the .7z file to extract.
        output_dir (str): The directory where the files will be extracted.

    Returns:
        bool: True if the extraction was successful, False otherwise.
    """
    try:
        # Ensure the output directory exists
        os.makedirs(output_dir, exist_ok=True)

        with py7zr.SevenZipFile(file_path, mode='r') as archive:
            archive.extractall(path=output_dir)
        print(f"File extracted successfully to: {output_dir}")
        return True
    except Exception as e:
        print(f"Error extracting file: {e}")
        return False

def call_python_script(script_path, *args):
    """
    Calls a Python script as a subprocess with the given arguments.

    Args:
        script_path (str): Path to the Python script to be executed.
        *args: Arguments to pass to the script.

    Returns:
        int: Exit code of the subprocess.
    """
    try:
        # Build the command
        command = [sys.executable, script_path] + list(args)

        # Call the subprocess
        result = subprocess.run(command, check=True, capture_output=True, text=True)

        # Print the output of the called script
        print(f"Output:\n{result.stdout}")
        print(f"Errors:\n{result.stderr}")

        return result.returncode
    except subprocess.CalledProcessError as e:
        print(f"Error calling script: {e}")
        print(f"Output:\n{e.output}")
        print(f"Errors:\n{e.stderr}")
        return e.returncode

# Raising an exception with a custom message
def raise_exception(condition, break_flow=False):
    def exception_message(cond):
        if not cond:
            raise ValueError("Condition was not met. Please check your input.")

    if break_flow:
        try:
            exception_message(condition)  # This will raise the exception
        except ValueError as e:
            print(f"Caught an exception: {e}")
    else:
        exception_message(condition)  # This will raise the exception

def get_system_separator():
    """Returns the file separator based on the operating system."""
    if os.name == 'nt':  # For Windows
        return '\\'
    else:  # For Unix-like systems (Linux, macOS)
        return '/'

def fetch_after_last_separator(input_string, separator):
    # Split the string from the right at the last occurrence of the separator
    _, _, after_separator = input_string.rpartition(separator)
    return after_separator

def capture_stdout(func, *args, **kwargs):
    # Create a new StringIO stream to capture stdout
    captured_output = io.StringIO()

    # Save the original stdout
    original_stdout = sys.stdout

    # Redirect stdout to the captured output
    sys.stdout = captured_output

    try:
        # Call the function and capture its stdout
        func(*args, **kwargs)
    finally:
        # Restore the original stdout
        sys.stdout = original_stdout

    # Get the captured output
    return captured_output.getvalue()

def fetch_json_from_output(string_in):
    lines = string_in.split('\n')
    if 'HTTP error' in string_in:
        for line in lines:
            if 'Response' in line:
                return json.loads(line.replace('Response: ', ''))
    else:
        for line in lines:
            if 'jsonrpc' in line:
                return json.loads(line)
