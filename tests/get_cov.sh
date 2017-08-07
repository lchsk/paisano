#!/bin/bash

for filename in `find . -iregex ".*\.cpp"`;
do
    echo $filename
    gcov -n -o . $filename > /dev/null;
done
