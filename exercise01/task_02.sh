#!/bin/bash
INFILE=$1
OUTFILE=$2
# help message when the number of provided arguments is not two
if [ $# -ne 2 ]
then
    echo "Please provide two files when calling this script." >&2
    exit 1
fi

# Log error message to error.log file when $OUTFILE is not writable.
if [ -w "$OUTFILE" ]
then
    if [ -e "$INFILE" ]
    then 
        cat "$INFILE" >> "$OUTFILE" 
    fi
else
    echo "error: $2 is not writable" 2> error.log
    exit 1
fi

# Function of script: The content of the first file is appended to the second file.