#!/bin/bash
for filename in `find . | egrep '\.hpp'`;
do
  gcov-5 -n -o . $filename > /dev/null;
done
