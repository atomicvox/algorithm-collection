#!/bin/bash

# Define an array
myarray=("apple" "banana" "cherry")

# Check if the input argument is provided
if [[ -z "$1" ]]; then
    echo "Usage: $0 <item>"
    exit 1
fi

# Use printf and grep to check for the item in the array
if printf '%s\0' "${myarray[@]}" | grep -Fxqz -- "$1"; then
    echo "'$1' is in the array."
else
    echo "'$1' is not in the array."
fi
