
         XALAN-C++ USER GUIDE     DEVELOPER NOTES


These are some notes for creating the XALAN-C++ User Guide from
StyleBook XML sources.

Java and the Apache StyleBook program are no longer required.
The XML transformations are now done using a standard XSLT transformation
utility.

The resulting web pages have static content without any javascript
or other scripting requirements.

The resulting web pages scale well with a wide variety of web browsers.
The browsers should support xhtml version 1.0.  Even thou the pages
are rendered as strict, the productions accommodate most browsers that
only render the loose or transitional implementation.


-------------------------------------------------------------------------
MANIFEST

sbk:(Source Tree) = trunk\xdocs

  html\                             Where StyleBook web pages are created
  html\apiDocs\                     Where doxygen API web pages are located

  style\dtd\                        The StyleBook DOCTYPE Definitions

  sources\                          Where my processing scripts are located
  sources\xalan\                    The StyleBook XML for Xalan User Guide
  sources\docs\                     The StyleBook - XML test suite
  sources\xalan-graphic\            Image files for xalan StyleBook XML
  sources\xslt-resources\           Resource files for html StyleBook web pages

  sources\entities.ent              Version entities for xalan StyleBook XML
  sources\xalan-book.xslt           StyleBook XSLT for Xalan StyleBook XML
  sources\make-xalan.bat            Transform single xalan StyleBook XML chapter
  sources\make-book.bat             Transform all xalan StyleBook XML chapters

  sources\mybook.xslt               StyleBook XSLT for StyleBook XML Tests
  sources\make-doc.bat              Transform a single StyleBook XML Test
  sources\make-tests.bat            Transform all StyleBook XML Tests

  sources\xslt-resources\apache-xalan.css     The html (css) stylesheet
  sources\xslt-resources\asf_logo.png         The ASF logo for header section

-------------------------------------------------------------------------

xalan and stylebook source edit is required to replace (sbk:) prefix.

The (sbk:) uri prefix is non-standard and since replaced with relative
system URLs.  In the (sources\xalan, sources\docs) directories, thex
(sbk:) is replaced with (../../).

The transformed XML places a copyright byline on each web page created.
You should edit the (make-xalan.bat, make-doc.bat) files to supply a 
proper create-date.

You should also edit the (make-xalan.bat, make-doc.bat) to invoke your
XSLT command line transformation utility.

The exslt.exe processor I use for XML transformation is based on the
xalan command-line transformer.  The critical capability is the ability
to insert top-level xslt parameters into the transformation process.

The xslt top-level parameters are:

   $sectionid   =   ($sectionid).xml  // chapter to transform
   $createdate  =   "date string"     // date of web page creation

The $sectionid is used in creating the <div id="navLeft"> html.

The $createdate is used in creating the <div id="footer"> html.

-------------------------------------------------------------------------

After doing the transformations, the following files should be copied
to the target (html) directory paths.

   sources\xslt-resources\*  -> html\resources\

   sources\xalan-graphic\*   -> html\

The Xalan-C++ API Reference Manual created by the Doxygen and GraphVue
programs is to be copied into the (html\apiDocs\) directory.

-------------------------------------------------------------------------

=====
TO DO
=====

Resolve the broken linkages.

Resolve the version packaging information.

Prepare an updated product revision history.

Prepare an SVN branch for documentation pre-release (v1.11)

Prepare an SVN branch for source code pre-release (v1.11)

Prepare a repository for specific binary releases (v1.11)





