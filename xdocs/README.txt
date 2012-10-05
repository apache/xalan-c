
         XALAN-C++ USER GUIDE     DEVELOPER NOTES

Edited:  2012-10-05

The html documentation pages are available on our website.

   http://xalan.apache.org/xalan-c/index.html

The html documentation pages can be built from the Xalan-C
source distribution.  Let ${XALANC} indicate the top directory
of the source distribution.

   ${XALANC}/src/ = the program library source files
   ${XALANC}/xdocs/ = the program documentation source files
   ${XALANC}/build/docs/ = the directory created for documentation builds
   ${XALANC}/build/docs/apiDocs/html/ = apiDocs created here, then moved
   ${XALANC}/build/docs/xalan-c/ =  the html documentation tree

#Setup UNIX for building the documents

1. Requires the "Xalan" executable in the ${PATH} environment.

2. Requires the libxerces-c.so and libxalan-c.so libraries in a path
   that is accessable to the "Xalan" executable.

3. Requires a current installation of the Doxygen package.

4. The interactive graphics for apiDocs require the GraphViz (dot) package.

#Build the documentation on UNIX

1. Connect to the ${XALANC}/xdocs/ directory

2. Run the make-apiDocs.sh (full graphics) or make-apiDocs-nograf (no graphics).

3. The script automatically chains to ${XALANC}/xdocs/sources/make-book.sh
   for building the remainder of the documentation.

4. You can now relocate and package the documentation at
   ${XALNAC}/build/docs/xalan-c/ to your final destination of choice.

#Setup WINDOWS for building the documents

1. Create a %XALANC%\bin\ directory. Then copy the xalan.exe and the DLL
   libraries for xerces-c and xalan-c into that directory.  This is where
   the make-xalan.bat file expects to find the xalan.exe executable.

2. Connect to the %XALANC%\xdocs\sources\ directory.

3. Run the make-book.bat script to create the %XALANC%\build\docs\xalan-c\
   documentation.

4. You can now relocate and package the documentation at
   %XALANC%\build\docs\xalan-c\ to your final destination of choice.

Note: WINDOWS has no Doxygen program for building the apiDocs.
Building apiDocs requires a Linux or UNIX platform.  You might
be able to use WINDOWS JavaDocs to get an interactive apiDocs reference.

#About the StyleBook XML Markup

The obsolete Apache Java Stylebook program is no longer being used.
The StyleBook XML markup has been slightly modified to accommodate
standard XSLT stylesheet transformations.

## Source Directory Layout

  ${XALAN}/xdocs/style/dtd/*  -- the validation DTD definitions
  ${XALAN}/xdocs/sources/stylebook.xslt -- the XML StyleBook transform
  ${XALAN}/xdocs/sources/xalan/* -- the StyleBook XML page content
  ${XALAN}/xdocs/sources/xalan-graphic/* -- page specific graphics
  ${XALAN}/xdocs/sources/xslt-resources/* -- logos and css stylesheets
  ${XALAN}/xdocs/sources/xalan.xml  -- the book navigation panel
  ${XALAN}/xdocs/sources/entities.ent -- book XML general entity definitions
  ${XALAN}/xdocs/sources/xalan/resources.xml -- general resource links

The resulting document from "stylebook.xslt" is strict XHTML with 
markup rules that can also be supported on transitional browsers.
To guard against XML element collapse, we use the Unicode 
zero-width-non-joining character to provide non-empty content.
This is not enforced by the "stylebook.xslt" transform.  The
StyleBook XML sources should guard against unwanted element collapse.

## Document Building Scripts

  ${XALAN}/xdocs/sources/make-xalan.sh {pagename} -- transform a single page
  ${XALAN}/xdocs/sources/make-xalan.bat {pagename} -- transform a single page

  ${XALAN}/xdocs/sources/make-book.sh -- transform an entire book
  ${XALAN}/xdocs/sources/make-book.bat -- transform an entire book


   


