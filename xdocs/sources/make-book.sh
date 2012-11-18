#!/bin/sh
#
# Bourne shell script to create Xalan-C documentation pages
#
# SET THE LIBRARY PATH FOR YOUR OPERATING SYSTEM, REQUIRED BY "Xalan" PROGRAM
#
#  SOLARIS AND LINUX
# export LD_LIBRARY_PATH=/usr/local/lib
#
#  AIX AND BSD
# export LIBPATH=/usr/local/lib
#
#  HPUX
# export SHLIB_PATH=/usr/local/lib
#
#  MAC OS/X
# export DYLD_LIBRARY_PATH=/usr/local/lib
#
#  CYGWIN AND MINGW
# export PATH=$PATH;/usr/local/lib
#

umask 0002
mkdir -p ../../build/docs/xalan-c/resources

make-xalan.sh usagepatterns
make-xalan.sh samples
make-xalan.sh resources
# make-xalan.sh build_instruct
make-xalan.sh install
make-xalan.sh download
make-xalan.sh overview
make-xalan.sh charter
# make-xalan.sh history
make-xalan.sh whatsnew
make-xalan.sh extensionslib
make-xalan.sh commandline
make-xalan.sh readme
make-xalan.sh releasenotes
make-xalan.sh programming
make-xalan.sh getstarted
make-xalan.sh index
make-xalan.sh extensions
make-xalan.sh faq
# make-xalan.sh test-faqs
make-xalan.sh license

make-xalan.sh buildlibs
make-xalan.sh secureweb
make-xalan.sh builddocs

cp xalan-graphic/*.* ../../build/docs/xalan-c
cp xslt-resources/*.* ../../build/docs/xalan-c/resources

