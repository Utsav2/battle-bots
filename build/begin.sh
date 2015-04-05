#!/bin/bash
set -e
set -o pipefail
cmake clean ../
make clean
cmake ../
make  
echo "everything is done, $USER"