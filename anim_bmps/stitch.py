import os
import subprocess

def rename_files(directory):
    # List all files in the directory
    files = os.listdir(directory)
    
    # Iterate over each file
    for filename in files:
        # Split the file name and extension
        name, ext = os.path.splitext(filename)

        # Check if the file name contains only digits
        if name.isdigit():
            # Pad the digits with leading zeros to make it five digits long
            new_name = f"filename_{name.zfill(4)}{ext}"
        else:
            # Check if the filename already starts with "filename_"
            if not name.startswith("filename_"):
                # Add "filename_" prefix
                new_name = f"filename_{name}{ext}"
            else:
                # If already has "filename_", keep the name as it is
                new_name = filename

        # Rename the file
        os.rename(os.path.join(directory, filename), os.path.join(directory, new_name))

def convert_to_png(input_directory, output_directory):
    # Create the output directory if it doesn't exist
    os.makedirs(output_directory, exist_ok=True)

    # List all files in the input directory
    files = os.listdir(input_directory)
    
    # Iterate over each file
    for filename in files:
        # Split the file name and extension
        name, ext = os.path.splitext(filename)

        # Check if the file is an image file
        if ext.lower() in (".jpg", ".jpeg", ".gif", ".bmp", ".tiff"):
            # Construct the input and output file paths
            input_filepath = os.path.join(input_directory, filename)
            output_filepath = os.path.join(output_directory, f"{name}.png")

            # Use subprocess to convert the image to PNG using ImageMagick
            subprocess.run(["convert", input_filepath, "-quality", "100", output_filepath])


# Test the functions
directory = "./source/"

rename_files(directory)
convert_to_png('./source/', './pngs/')