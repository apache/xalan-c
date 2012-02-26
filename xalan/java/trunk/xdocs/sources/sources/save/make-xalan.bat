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

SET WEBCREATED=June 8, 2011

SET XALANEXSLT=..\..\bin\exslt.exe

::SET XALANSTYLE=xalan-book.xslt

SET XALANSTYLE=mytest.xslt

SET XALANXMLDIR=xalan

SET XALANOUTDIR=..\html

ECHO "%XALANEXSLT%" -p "sectionid='%1'" -p "createdate='%WEBCREATED%'" %XALANXMLDIR%\%1.xml %XALANSTYLE% %XALANOUTDIR%\%1.html

     "%XALANEXSLT%" -p "sectionid='%1'" -p "createdate='%WEBCREATED%'" %XALANXMLDIR%\%1.xml %XALANSTYLE% %XALANOUTDIR%\%1.html

