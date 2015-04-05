#!/bin/bash
DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
set -e
set -o pipefail
cd $DIR
cd build
cmake clean ../
make clean
cmake ../
make
if [ -n "$1" ]; then 
	python $1
fi 
echo "everything is done, $USER"
