::  Volume in drive I is LEDS-USB
::  Volume Serial Number is 27B6-025E
:: 


mkdir ..\..\build\docs\xalan-c\resources

call make-xalan usagepatterns
call make-xalan samples
call make-xalan resources
:: call make-xalan build_instruct
call make-xalan install
call make-xalan download
call make-xalan overview
call make-xalan charter
:: call make-xalan history
call make-xalan whatsnew
call make-xalan extensionslib
call make-xalan commandline
call make-xalan readme
call make-xalan releasenotes
call make-xalan programming
call make-xalan getstarted
call make-xalan index
call make-xalan extensions
call make-xalan faq
:: call make-xalan test-faqs
call make-xalan license

call make-xalan buildlibs
call make-xalan secureweb
call make-xalan builddocs

copy xalan-graphic\*.* ..\..\build\docs\xalan-c
copy xslt-resources\*.* ..\..\build\docs\xalan-c\resources

