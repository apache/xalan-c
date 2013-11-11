#!/bin/sh
#
# Script file to create Xalan-C apiDocs
#
# This script requires current versions of "Doxygen" and
# "GraphViz (dot)" programs to be installed on your UNIX.
#

# Various OS's have different ways of specifying the library path
#
# Solaris and Linux
# LD_LIBRARY_PATH=/usr/local/lib
#
# HP-UX
# SHLIB_PATH=/usr/local/lib
#
# AIX and BSD
# LIBPATH=/usr/local/lib
#
# Mac OS X
# DYLD_LIBRARY_PATH=/usr/local/lib
#
# Cygwin and MinGW
# PATH=
#

# Add the current directory to the PATH for nested scripts

PATH=$PATH:.

# Add the PATH for the Xalan program

# export PATH=/usr/local/bin:$PATH

# Make the necessary target paths

if [ ! -d ../build/docs/apiDocs ] ; then
  mkdir -p ../build/docs/apiDocs
fi
if [ ! -d ../build/docs/xalan-c ] ; then
  mkdir -p ../build/docs/xalan-c
fi

# Create the xalan-c apiDocs using (doxygen) and (dot).

doxygen DoxyfileXalan

# We may not want the many *.md5 files

rm ../build/docs/apiDocs/html/*.md5

# Copy the Apache Trademark Graphic

cp html/apiDocs/asf_logo_wide.gif ../build/docs/apiDocs/html

# Move the apiDocs to the xalan-c website tree

if [ -e ../build/docs/xalan-c/apiDocs ] ; then
  rm -rf ../build/docs/xalan-c/apiDocs
fi
mv ../build/docs/apiDocs/html ../build/docs/xalan-c/apiDocs

#
# Done with xalan-c apiDocs build
#

#
# Chain to sources to create xalan-c website pages
#

cd sources
make-book.sh
cd ..

