#!/bin/bash

/opt/clion-2017.2.3/bin/cmake/bin/cmake --build /home/paul/dev/fluidsim/cmake-build-debug --target fluidsim -- -j 2
if [ ! $? ]; then
    echo "Build error!"
    exit
fi
/home/paul/dev/fluidsim/cmake-build-debug/fluidsim
if [ ! $? ]; then
    echo "Run error!"
    exit
fi
/usr/bin/python3.6 /home/paul/dev/fluidsim/py/render.py
if [ ! $? ]; then
    echo "Render error!"
    exit
fi