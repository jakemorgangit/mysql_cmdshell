#!/bin/bash

ALLOW_LIST_FILE="allow.list"
TEMPLATE_FILE="template.c"
OUTPUT_FILE="mysql_cmdshell.c"

if [ ! -f "$ALLOW_LIST_FILE" ]; then
    echo "Error: allow.list file not found!"
    exit 1
fi

if [ ! -f "$TEMPLATE_FILE" ]; then
    echo "Error: template.c file not found!"
    exit 1
fi

# Create the output file and start writing from the template
while IFS= read -r line; do
    if [[ "$line" == "static const char *allowed_commands[] = {" ]]; then
        echo "$line" >> "$OUTPUT_FILE"
        while IFS= read -r cmd; do
            echo "    \"$cmd\"," >> "$OUTPUT_FILE"
        done < "$ALLOW_LIST_FILE"
    else
        echo "$line" >> "$OUTPUT_FILE"
    fi
done < "$TEMPLATE_FILE"

echo "Generated $OUTPUT_FILE with embedded allowed commands from $ALLOW_LIST_FILE."

