#!/bin/bash

DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )
export PYTHONPATH=$DIR/build:$PATH
for filename in tests/*.py; do
	python "$filename" 
done
