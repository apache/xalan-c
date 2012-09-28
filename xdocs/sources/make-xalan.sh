#!/bin/sh
#
# Bourne shell script to create a single xalan-c webpage
# Parameter $1 is the basename of the page to generate.
#
#-------------------------------------------------------------
#
# Set the current path to include the "Xalan" stylesheet transformation utility
#
# $1               The document filename (without extension) to transform
#
# -- The Environment Variables for this script --
#
# ${XALANBOOKXML}     The <book> navigation file for this project
#
# ${XALANCMD}         The Xalan command-line XSLT transformation utility
#
# ${XALANSTYLE}       The XSLT stylesheet to 
#
# ${XALANSRCDIR}      Source directory of STYLEBOOK markup XML documents
#
# ${XALANOUTDIR}      Target directory of XHTML web page documents
#
# ${XALANLOGOIMG}     Web Page Logo
#
# ${XALANLOGOWIDTH}   Web Page Logo Width
#
# ${XALANLOGOHEIGHT}  Web Page Logo Height
#
# ${XALANLOGOLINK}    Active Link for Web Page Logo
#
# ${XALANTITLELINK}   Active Link for Web Page Title
#
# ${WEBCREATED}       Web Page Creation Date
#
# -- The XSLT Stylesheet Top-Level Parameters
#
# sectionid        Top-level stylesheet parameter (document file id)
#
# createdate       Top-level stylesheet parameter (web page creation date)
#
# logoimage        Apache Trademark Logo Image
#
# logolink         Logo Image - hot link
#
# bookxml          The <book>.xml file for navigation
#
# booklink         The Hot-Link for Book Title in Header
#

# Set some environment variables

XALANBOOKXML=xalan.xml

XALANCMD=Xalan

XALANSTYLE=stylebook.xslt

XALANSRCDIR=xalan

XALANOUTDIR=../../build/docs/xalan-c

XALANLOGOIMG=resources/XalanC-Logo-tm.png

XALANLOGOWIDTH=190

XALANLOGOHEIGHT=90

XALANLOGOLINK=../index.html

XALANTITLELINK=index.html

WEBCREATED=`date +"%a %F"`

## DEBUG BEGIN

#echo INPUT PARAM = $1
#echo XALANBOOKXML = ${XALANBOOKXML}
#echo XALANCMD = ${XALANCMD}
#echo XALANSTYLE = ${XALANSTYLE}
#echo XALANSRCDIR = ${XALANSRCDIR}
#echo XALANOUTDIR = ${XALANOUTDIR}
#echo XALANLOGOIMG = ${XALANLOGOIMG}
#echo XALANLOGOWIDTH = ${XALANLOGOWIDTH}
#echo XALANLOGOHEIGHT = ${XALANLOGOHEIGHT}
#echo XALANLOGOLINK = ${XALANLOGOLINK}
#echo XALANTITLELINK = ${XALANTITLELINK}
#echo WEBCREATED = ${WEBCREATED}
#exit

## DEBUG END


#
# Create Target Directories
#

if [ ! -d "${XALANOUTDIR}" ]; then
  mkdir -p "${XALANOUTDIR}"
fi

#IF EXIST "%XALANOUTDIR%" GOTO NEXT1
#MKDIR "%XALANOUTDIR%"
#:NEXT1

if [ ! -d "${XALANOUTDIR}/resources" ] ; then
  mkdir -p "${XALANOUTDIR}/resources"
fi

#IF EXIST "%XALANOUTDIR%/resources" GOTO NEXT2
#MKDIR "%XALANOUTDIR%/resources"
#:NEXT2

#
# Now create the specific web page from StyleBook XML
#

"${XALANCMD}" \
  -p sectionid "'$1'" \
  -p xmlsources "'${XALANSRCDIR}'" \
  -p createdate "'${WEBCREATED}'" \
  -p logoimage "'${XALANLOGOIMG}'" \
  -p logowidth "'${XALANLOGOWIDTH}'" \
  -p logoheight "'${XALANLOGOHEIGHT}'" \
  -p logolink "'${XALANLOGOLINK}'" \
  -p bookxml "'${XALANBOOKXML}'" \
  -p booklink "'${XALANTITLELINK}'" \
   "${XALANSRCDIR}/$1.xml" "${XALANSTYLE}"  >"${XALANOUTDIR}/$1.html"
