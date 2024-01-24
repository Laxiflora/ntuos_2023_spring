#!/bin/bash

# Define the directory where you want to search for files
search_directory="./"

# Define the specific filename you want to match
filename_to_remove="specific_filename.txt"

# Use the find command to search for files with the specified filename
# and then use the -exec option to remove each file found
find "$search_directory" -type f -name "$filename_to_remove" -exec rm {} \;