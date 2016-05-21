#!/bin/sh

set -ex

rm -rf autom4te.cache

libtoolize -f -c
aclocal -I m4 --force
autoconf -f -W all
automake -f -a -c -W all --foreign

rm -rf autom4te.cache

