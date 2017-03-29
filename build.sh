#!/bin/bash

if [ -g ./vainfo ];
then rm -rf ./va_info
else
    echo "There is no vainfo"
fi

gcc -g -o vainfo vainfo.c -lva -lX11 -lva-x11
