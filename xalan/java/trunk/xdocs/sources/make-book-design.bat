::  Volume in drive I is LEDS-USB
::  Volume Serial Number is 27B6-025E
:: 


mkdir ..\..\build\docs\xalan\xalan-j\design\resources

call make-xalanj-design design2_0_0

copy design\*.* ..\..\build\docs\xalan\xalan-j\design
copy xslt-resources\*.* ..\..\build\docs\xalan\xalan-j\design\resources

