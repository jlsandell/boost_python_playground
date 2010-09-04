#!/bin/bash

boost_path="/home/jeremy/development/root/usr/lib/"
include_path="/home/jeremy/development/root/usr/include"
python_path="/usr/include/python2.6"

g++ -I $python_path -I $include_path -L $boost_path -lboost_python embiggenator.cpp -shared -o embiggenator.so
