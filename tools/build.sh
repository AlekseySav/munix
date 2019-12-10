#!/bin/bash

#
# tools/build v2.0
# run tools/build.sh boot/boot system > Image
#

if [ $# = 1 ] && [ $1 = clean ]; then
    rm -f tools/boot tools/system
    exit 0
fi;

if [ $# != 2 ]; then
    echo "Usage: build Munix Image" >&2
    exit 1
fi;

objcopy -O binary -R .note -R .comment -S $1 tools/boot
objcopy -O binary -R .note -R .comment -S $2 tools/system
truncate -s 510 tools/boot

cat tools/boot
printf "U\252"          # print 0xaa55 magic number. Looks ugly, but works
cat tools/system
