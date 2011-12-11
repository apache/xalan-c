::  Volume in drive I is LEDS-USB
::  Volume Serial Number is 27B6-025E
:: 


mkdir ..\..\build\docs\xalan\xalan-j\resources

call make-xalanj resources
call make-xalanj index
call make-xalanj charter
call make-xalanj whatsnew
call make-xalanj readme
call make-xalanj overview
call make-xalanj downloads
call make-xalanj getstarted
call make-xalanj xsltc_usage
call make-xalanj faq
call make-xalanj samples
call make-xalanj commandline
call make-xalanj commandline_xsltc
call make-xalanj features
call make-xalanj trax
call make-xalanj xpath_apis
call make-xalanj usagepatterns

call make-xalanj public_apis
call make-xalanj dtm
call make-xalanj extensions
call make-xalanj extensionslib
call make-xalanj extensions_xsltc
call make-xalanj builds
call make-xalanj bugreporting
call make-xalanj contact_us

copy xalan-j-graphic\*.* ..\..\build\docs\xalan\xalan-j
copy xslt-resources\*.* ..\..\build\docs\xalan\xalan-j\resources

