#!/bin/sh
# Wrapper script around mirrors.cgi script for ASF infra support for mirrors.
# (We must change directories in order for python to pick up the includes)
# Original: Stefan's page on mirrors

cd /www/www.apache.org/dyn/mirrors
/www/www.apache.org/dyn/mirrors/mirrors.cgi $*

