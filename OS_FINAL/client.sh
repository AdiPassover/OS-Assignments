#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <path_to_txt_file>"
    exit 1
fi

file_path="$1"

{
    while IFS= read -r line
    do
        echo "$line"
        sleep 0.01
    done < "$file_path"
} | nc -q 1 localhost 9034