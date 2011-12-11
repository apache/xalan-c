::  Volume in drive I is LEDS-USB
::  Volume Serial Number is 27B6-025E
:: 


mkdir ..\..\build\docs\xalan\xalan-c\resources

call make-xalanc usagepatterns
call make-xalanc samples
call make-xalanc resources
:: call make-xalanc build_instruct
call make-xalanc install
call make-xalanc download
call make-xalanc overview
call make-xalanc charter
:: call make-xalanc history
call make-xalanc whatsnew
call make-xalanc extensionslib
call make-xalanc commandline
call make-xalanc readme
call make-xalanc releasenotes
call make-xalanc programming
call make-xalanc getstarted
call make-xalanc index
call make-xalanc extensions
call make-xalanc faq
call make-xalanc test-faqs
call make-xalanc license

call make-xalanc buildlibs
call make-xalanc secureweb
call make-xalanc builddocs

copy xalan-c-graphic\*.* ..\..\build\docs\xalan\xalan-c
copy xslt-resources\*.* ..\..\build\docs\xalan\xalan-c\resources

