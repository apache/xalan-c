::  Volume in drive I is LEDS-USB
::  Volume Serial Number is 27B6-025E
:: 


mkdir ..\..\build\docs\xalan\resources

call make-xalan-org index
call make-xalan-org charter

copy xslt-resources\*.* ..\..\build\docs\xalan\resources

