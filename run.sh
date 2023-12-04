#!/bin/bash

EXAMPLE=$1

# find the file in the `python` directory, starting with the number $EXAMPLE
FILE=$(find python -name "${EXAMPLE}*.py" | sort | head -n 1)

# run the file
python $FILE 