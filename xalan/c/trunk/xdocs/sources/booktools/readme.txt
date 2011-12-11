
        Tools to automatically build StyleBook web pages


BOOKTOOLS
---------

I am preparing a set of book tools to automatically generate
an Apache StyleBook source, but can be customized to drive
any number of book creations.

The initial project is to have an XML based configuration
file from which command files can be created.

A standard XSLT template will take the configuration XML
file and create for UNIX a Bourne Shell (SH) script, or
create for WINDOWS a Command (CMD/BAT) script.

Running the created script will create all the web pages
associated with a StyleBook project.


CONFIGURATION PARAMETERS
------------------------

BUILDFOR		WIN, UNIX

	This defines whether the text output is for Windows or UNIX.

XALANCMD		..\..\bin\xalan

	This is the standard Apache XALAN command-line transformation program.

XMLDIR			xalan

	This is the source directory for the StyleBook XML sources

HTMLDIR			..\..\build\docs\html

	This is the target directory for the resulting web pages

RESOURCEDIR		resources

	This is a subdirectory of HTMLDIR that holds the project resources.
	This includes the *.css files for the browsers and project
	trademark logo images.

IMAGEDIR		(empty)

	This is a subdirectory of HTMLDIR that holds the images required
	for the project *.html pages.  If (empty), the images are copied
	into the HTMLDIR directory.

IMAGETYPES		png svg jpg jpeg gif

	This is the list of file extensions of files found in XMLDIR
	that should be copied to IMAGEDIR.

BOOKXML			xalan.xml

	This is the master StyleBook <book> XML that is used for
	project navigation.

BOOKXSLT		style-book.xslt (xalan-book.xslt)

	This is the XSLT stylesheet that will transform the
	StyleBook XML sources into HTML or XHTML web page content.

BOOKCSS			apache-xalan.css

	This is the Browser HTML CSS stylesheet that is used
	to flavour the web pages for presentation.

BOOKRESOURCES		xslt-resources

	This is the repository of files that will be copied
	to the RESOURCEDIR. It holds the CSS stylesheets and
	trademark logos for common project page reference.

BOOKIMAGES		xalan-graphic

	This is an additional repository of content that are
	copied to IMAGEDIR.  The IMAGETYPES filter is not used here.

LOGOLINK		http://xalan.apache.org	- http://www.apache.org

	This is the URL reference that is linked to the trademark
	LOGOIMAGE.

LOGOIMAGE		XalanC-Logo.png
  
	This is the image in RESOURCEDIR to use as the web-page
	trademark logo.

LOGOHEIGHT		90

	This is a the box height in pixels to use for the LOGOIMAGE.

LOGOWIDTH		190

	This is the box width in pixels to use for the LOGOIMAGE.

TITLELINK		index.html

	This is the URL to use for the project Title Line
	found in the web page header.


EXAMPLE
-------

1. Connect to the documentation sources:

	xalan/(c | java)/trunk/xdocs/sources


2. Make the [xalan] command-line available for interactive use.


3. Make the document construction scripts with [xalan]

   <for WINDOWS>

	[xalan] bookproject.xml  makebuild.xslt  docbuild.bat


   <for UNIX>

	[xalan] bookproject.xml  makebuild.xslt  docbuild.sh


4. Build the documentation

	[docbuild]


5. Test the new web pages with several web browsers

	The web pages are found in the HTMLDIR directory.


6. Copy the new web pages to your web server

	The web pages are constructed into the HTMLDIR directory.



SAMPLE CONFIGURATION
--------------------

<STYLECONFIG>
  <BUILDFOR>WIN</BUILDFOR>
  <XALANCMD>..\..\bin\xalan</XALANCMD>
  <XMLDIR>xalan</XMLDIR>
  <HTMLDIR>..\..\build\docs\xalan-c</HTMLDIR>
  <RESOURCEDIR>resources</RESOURCEDIR>
  <IMAGEDIR/>
  <IMAGETYPES>png svg jpg jpeg gif map html</IMAGETYPES>
  <BOOKXML>xalan.xml</BOOKXML>
  <BOOKXSLT>xalan-book.xslt</BOOKXSLT>
  <BOOKCSS>apache-xalan.css</BOOKCSS>
  <BOOKRESOURCES>xslt-resources</BOOKRESOURCES>
  <BOOKIMAGES>xalan-graphic</BOOKIMAGES>
  <LOGOLINK>http://xalan.apache.org</LOGOLINK>
  <LOGOHEIGHT>90</LOGOHEIGHT>
  <LOGOWIDTH>190</LOGOWIDTH>
  <TITLELINK>index.html</TITLELINK>
</STYLECONFIG>
