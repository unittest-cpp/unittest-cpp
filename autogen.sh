#!/bin/sh

test -e ./.autotools_aux || mkdir .autotools_aux

libtoolize -cq
aclocal -I m4 --install           # Generate aclocal
autoconf                          # Generate configure script 
autoheader                        # Generate config.h
automake --add-missing --copy     # Generate Makefile.in and other scripts 
