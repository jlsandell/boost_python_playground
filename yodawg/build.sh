#!/bin/bash

# You'll want to change these to the appropriate directories.
# My build environment is rather messy.

boost_path="/usr/local/lib"

include_path="/usr/local/include"
python_path="/usr/include/python2.6"

g++ -I $python_path -I $include_path -L $boost_path -lboost_filesystem -lpython2.6 -lboost_python yodawg.cpp -o yodawg.exe
