#!/bin/bash

DATA_DIR=data # source directory for psueodocode files
successful=0
failed=0

if !(test -f "bin/main"); then
    echo "Main executable not found in directory bin"
fi
# run tests
for file in $DATA_DIR/*.pcode; do 
    printf "Testing file $file: "
    ./bin/main $file
    if [[$? -eq 0]]; then
        successful=$successful + 1
        printf "Success \n"
    else
        failed=$failed + 1
        printf "Failure \n"
    fi
done