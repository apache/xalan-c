::
:: Set the current path to include a stylesheet transformation utility
::
:: %1               The document filename (without extension) to transform
::
:: -- The Environment Variables for this script --
::
:: %XALANBOOKXML%     The <book> navigation file for this project
::
:: %XALANCMD%         The Xalan command-line XSLT transformation utility
::
:: %XALANSTYLE%       The XSLT stylesheet to 
::
:: %XALANSRCDIR%      Source directory of STYLEBOOK markup XML documents
::
:: %XALANOUTDIR%      Target directory of XHTML web page documents
::
:: %XALANLOGOIMG%     Web Page Logo
::
:: %XALANLOGOWIDTH%   Web Page Logo Width
::
:: %XALANLOGOHEIGHT%  Web Page Logo Height
::
:: %XALANLOGOLINK%    Active Link for Web Page Logo
::
:: %XALANTITLELINK%   Active Link for Web Page Title
::
:: %WEBCREATED%       Web Page Creation Date
::
:: -- The XSLT Stylesheet Top-Level Parameters
::
:: sectionid        Top-level stylesheet parameter (document file id)
::
:: createdate       Top-level stylesheet parameter (web page creation date)
::
:: logoimage        Apache Trademark Logo Image
::
:: logolink         Logo Image - hot link
::
:: bookxml          The <book>.xml file for navigation
::
:: booklink         The Hot-Link for Book Title in Header
::

SET XALANBOOKXML=xalan.xml

SET XALANCMD=..\..\bin\xalan.exe

SET XALANSTYLE=stylebook.xslt

SET XALANSRCDIR=xalan

SET XALANOUTDIR=..\..\build\docs\xalan-c

SET XALANLOGOIMG=resources/XalanC-Logo-tm.png

SET XALANLOGOWIDTH=190

SET XALANLOGOHEIGHT=90

SET XALANLOGOLINK=../index.html

SET XALANTITLELINK=index.html

SET WEBCREATED=%DATE%

::
:: Create Target Directories
::

IF EXIST "%XALANOUTDIR%" GOTO NEXT1
MKDIR "%XALANOUTDIR%"
:NEXT1
IF EXIST "%XALANOUTDIR%\resources" GOTO NEXT2
MKDIR "%XALANOUTDIR%\resources"
:NEXT2

::
:: Now create the specific web page from StyleBook XML
::

"%XALANCMD%" -p sectionid "'%1'" -p xmlsources "'%XALANSRCDIR%'" -p createdate "'%WEBCREATED%'" -p logoimage "'%XALANLOGOIMG%'" -p logowidth "'%XALANLOGOWIDTH%'" -p logoheight "'%XALANLOGOHEIGHT%'" -p logolink "'%XALANLOGOLINK%'" -p bookxml "'%XALANBOOKXML%'" -p booklink "'%XALANTITLELINK%'" %XALANSRCDIR%\%1.xml %XALANSTYLE%  >%XALANOUTDIR%\%1.html
