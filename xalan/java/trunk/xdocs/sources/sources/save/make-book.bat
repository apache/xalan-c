::  Volume in drive I is LEDS-USB
::  Volume Serial Number is 27B6-025E
:: 
::  Directory of I:\apache\trunk\xdocs\sources\xalan
:: 
:: 04/25/2011  02:11 PM            28,624 usagepatterns.xml
:: 04/25/2011  02:11 PM            24,129 samples.xml
:: 04/25/2011  02:11 PM             5,809 resources.xml
:: 04/25/2011  02:11 PM            21,681 build_instruct.xml
:: 04/25/2011  02:11 PM             3,849 install.xml
:: 04/25/2011  02:11 PM             5,353 download.xml
:: 04/25/2011  02:11 PM            10,287 overview.xml
:: 04/25/2011  02:11 PM            19,552 charter.xml
:: 04/25/2011  02:11 PM            15,700 history.xml
:: 04/25/2011  02:11 PM             8,361 whatsnew.xml
:: 04/25/2011  02:11 PM             8,564 extensionslib.xml
:: 04/25/2011  02:11 PM             6,404 commandline.xml
:: 04/25/2011  02:11 PM            27,303 readme.xml
:: 04/25/2011  02:11 PM             4,290 programming.xml
:: 04/25/2011  02:11 PM            11,414 getstarted.xml
:: 04/25/2011  02:11 PM             2,652 index.xml
:: 04/25/2011  02:11 PM            12,339 extensions.xml
:: 05/31/2011  09:21 AM            21,082 faq.xml
::               18 File(s)        237,393 bytes
::                0 Dir(s)     870,219,776 bytes free


mkdir ..\html\resources

call make-xalan usagepatterns
call make-xalan samples
call make-xalan resources
call make-xalan build_instruct
call make-xalan install
call make-xalan download
call make-xalan overview
call make-xalan charter
:: call make-xalan history
call make-xalan whatsnew
call make-xalan extensionslib
call make-xalan commandline
call make-xalan readme
call make-xalan programming
call make-xalan getstarted
call make-xalan index
call make-xalan extensions
call make-xalan faq
call make-xalan license

copy xalan-graphic\*.* ..\html
copy xslt-resources\*.* ..\html\resources

