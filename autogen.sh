#!/bin/sh

set -ex

rm -rf autom4te.cache

aclocal --force
libtoolize -f -c
autoconf -f -W all
automake -f -a -c -W all --foreign

rm -rf autom4te.cache

