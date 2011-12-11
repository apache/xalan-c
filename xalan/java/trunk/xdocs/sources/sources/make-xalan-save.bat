::
:: Set the current path to include a stylesheet transformation utility
::
:: %1               The document filename (without extension) to transform
::
:: %XALANEXSLT%     The exslt stylesheet transformation utility
::
:: %XALANSTYLE%     The XSLT stylesheet to 
::
:: %XALANXMLDIR%    Source directory of STYLEBOOK markup XML documents
::
:: %XALANOUTDIR%    Target directory of XHTML web page documents
::
:: %WEBCREATED%     Web Page Creation Date
::
:: sectionid        Top-level stylesheet parameter (document file id)
::
:: createdate       Top-level stylesheet parameter (web page creation date)
::

SET WEBCREATED=%DATE%

SET XALANEXSLT=..\..\bin\xalan.exe

SET XALANSTYLE=xalan-book.xslt

SET XALANXMLDIR=xalan

SET XALANOUTDIR=..\..\build\docs\html

:: Now perform the transformation of the specific StyleBook XML page

"%XALANEXSLT%" -p sectionid "'%1'" -p createdate "'%WEBCREATED%'" -p logoimage "'resources/XalanC-Logo.png'" -p logolink "'http://xalan.apache.org'" -p bookxml "'xalan.xml'" -p booklink "'index.html'" %XALANXMLDIR%\%1.xml %XALANSTYLE%  >%XALANOUTDIR%\%1.html

