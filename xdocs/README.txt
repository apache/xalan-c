
         XALAN-C++ USER GUIDE     DEVELOPER NOTES
Edited:  2012-09-16

UNIX platforms can make both the xalan-c API document and the xalan-c website.

WINDOWS platforms can only make the xalan-c website without the xalan-C apiDocs.

Java and the Apache StyleBook program are no longer reuired.
The XML transformations are now done using a standard XSLT
command-line stylesheet transformation utility.  The Apache Xalan
program or Xalan.exe program is used for this transformation.
The resulting web pages have static content without any javascript
or other scripting requirements.

The resulting web pages scale well with a wide variety of web browsers.
The browsers should support xhtml version 1.0.  Even thou the pages
are rendered as strict, the productions accommodate most browsers that
only render the loose or transitional implementation.

Download the source package from repository or subversion

   http://www.apache.org/dyn/closer/xalan/xalan-c
   http://svn.apache.org/repos/asf/xalan/c/trunk

The documentation products will be built to the target directories

   c/trunk/build/docs/apiDocs
   c/trunk/build/docs/xalan-c

UNIX Build xalan-c apiDocs

   Prerequisite: The Doxygen package must be installed (doxygen) program.
   Prerequisite: The GraphViz package must be installed (dot) program.

   cd {c/trunk/xdocs}
   Edit the "make-apiDocs.sh" script

   Run the "make-apiDocs.sh" script

UNIX Build xalan-c website pages

   Prerequisite: The "Xalan" command-line executable must be available

   cd {c/trunk/xdocs/sources}
   Run the "make-book.sh" script

WINDOWS Build the xalan-c website pages

   Prerequisite: The "xalan.exe" command-line executable must be available

   cd {c/trunk/xdocs/sources}
   Run the "make-book.bat" script

-------------------------------------------------------------------
The "sbk:" prefix is non-standard and was recognized by the deprecated
Apache Stylebook Java program.  The "sbk:" prefix has now been replaced
with relative URLs for use with a standard XSLT stylesheet processor.

MANIFEST

  {xalan/c/trunk}

  bin/
        Placeholder for Xalan.exe and DLL files for Windows Binary executables.

  build/ 
        Placeholder where products are constructed.

  build/docs/appDocs/
        Placeholder for construction of xalan-c apiDocs.

  build/docs/xalan-c/
        Placeholder for construction of xalan-c webpages.

  src/
        Source tree of C/C++ sources for making the xalan library and 
        Xalan command-line processor.  The *.hpp source files contain
        the structured components from which the apiDocs document is 
        constructed.

  xdocs/ 
        Directory tree of documentation sources and build scripts.

  xdocs/DoxyfileXalan     The configuration file for doxygen program.

* xdocs/make-apiDocs.sh   The Bourne shell script to build apiDocs.

  xdocs/html/apiDocs/
        Directory of doxygen include files and Apache feather logo
        for apiDocs pages.

  xdocs/style/dtd/        The Stylebook validation DTD files.

  xdocs/sources/
        Directory tree of sources and scripts to build the xalan-c
        webpages or User Guide.

  xdocs/sources/entities.ent
        Some included DTD entity definitions.

  xdocs/sources/xalan-xml
        The Stylebook XML page navigation source.

  xdocs/sources/stylebook.xslt
        The standardized Xalan stylebook transformation stylesheet.

  xdocs/sources/xalan/
        Directory of Stylebook XML page sources for xalan-c User Guide.

  xdocs/sources/xalan-graphic/
        Special graphics required by some xdocs/sources/xalan/ sources.
        These files are copied to build/docs/xalan-c/

  xdocs/sources/xslt-resources/
        The Xalan LOGOs and css stylesheets for rendering web pages.
        These files are copied to build/docs/xalan-c/resources/

* xdocs/sources/make-book.sh
        The UNIX Bourne shell script to build the xalan-c web pages.

* xdocs/sources/make-book.bat
        The Windows command script to build the xalan-c web pages.

-------------------------------------------------------------------
The Xalan-C website is published to "svnpubsub" by copying 

  build/docs/xalan-c/*

to the "svnpubsub" repository tree

  {svn}/xalan/site/docs/xalan/xalan-c/

then perform a repository subversion check-in.
-------------------------------------------------------------------

These notes are provided by Steven J. Hathaway for the
Xalan Documentation Project.

Current information is available on the Xalan website
http://xalan.apache.org/xalan-c





