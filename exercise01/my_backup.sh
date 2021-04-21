#!/bin/bash
cp --update * "$1"

#   cp          - copies source to destination or multiple source(s) to directory. 
#                 format:  cp -flag Source Destination
#   --update    - copy only when the source file is newer than the destination
#                 file, or when the destination file is missing. (man pages)
#   *           - all folders in the directory in which the script is called
#   "$1"        - The first argument, containing the path to the backup file