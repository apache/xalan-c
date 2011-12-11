::  Volume in drive I is LEDS-USB
::  Volume Serial Number is 27B6-025E
:: 


mkdir ..\..\build\docs\xalan\xalan-j\xsltc\resources

call make-xalanj-xsltc index
call make-xalanj-xsltc xsltc_compiler
call make-xalanj-xsltc xsl_whitespace_design
call make-xalanj-xsltc xsl_sort_design
call make-xalanj-xsltc xsl_key_design
call make-xalanj-xsltc xsl_comment_design
call make-xalanj-xsltc xsl_lang_design
call make-xalanj-xsltc xsl_unparsed_design
call make-xalanj-xsltc xsl_if_design
call make-xalanj-xsltc xsl_choose_design
call make-xalanj-xsltc xsl_include_design
call make-xalanj-xsltc xsl_variable_design
call make-xalanj-xsltc xsltc_runtime
call make-xalanj-xsltc xsltc_dom
call make-xalanj-xsltc xsltc_namespace
call make-xalanj-xsltc xsltc_trax
call make-xalanj-xsltc xsltc_predicates
call make-xalanj-xsltc xsltc_iterators
call make-xalanj-xsltc xsltc_native_api
call make-xalanj-xsltc xsltc_trax_api
call make-xalanj-xsltc xsltc_performance


copy xalan-xsltc-graphic\*.* ..\..\build\docs\xalan\xalan-j\xsltc
copy xslt-resources\*.* ..\..\build\docs\xalan\xalan-j\xsltc\resources

