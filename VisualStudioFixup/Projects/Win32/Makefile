#
# Licensed to the Apache Software Foundation (ASF) under one
# or more contributor license agreements. See the NOTICE file
# distributed with this work for additional information
# regarding copyright ownership. The ASF licenses this file
# to you under the Apache License, Version 2.0 (the  "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#


################################################################################################
###
###	 The makefile will create binary packages based on VC6, VC7, Win32 or Win64 builds
###
###	 Usage:
###		nmake [prefix=[path]] [BITS=32*|64] [IDE_VER=VC6*|VC7] [INSTALL_MODE=Debug|Release|Both*] [PACKAGE_NAME=<desirable_package_name>][<make_file_target>]
###
###	 Parameters:
###	 	The Makefile reads next environmental variables:
###	 		XALANCROOT : a regular Xalan-C root, describing location of the built sources. ".\..\.." is a default value
###	 		DESTDIR : destination location on the built binary package. Default is $(XALANCROOT)
###	 		 
###	 		Command line parameters:
###				"prefix" Path provided for prefix should be a relative path and it's appended to $(DESTDIR) 
###	 		 	Virtually all macros in the makefile or in tools.ini file can be redefined from the command line .
###	 		 	For default values see tools.ini file
###
################################################################################################

# =================== Check parameters ans set variables ==========================================

!IFNDEF XALANCROOT
XALANCROOT=.\..\..
!ENDIF

!INCLUDE $(XALANCROOT)\version.incl

!IF "$(PROJECT_NAME)"!="xalan-c" && "$(ICUROOT)x" == "x"
!	ERROR ICUROOT sould be defined
!ENDIF

!IF "$(INSTALL_MODE)"=="Both"
!UNDEF INSTALL_MODE
INSTALL_MODE=Release
BUILD_DEBUG=build_debug
!ENDIF

MSGLIB_PREFIX=XalanMessages

!IF "$(PROJECT_NAME)"!="xalan-c" 
ICUTARGET=icu

!	IF "$(INSTALL_MODE)"=="Debug"
ICULIB_DLL=$(ICUROOT)\bin\icuin??d.dll
!	ELSEIF "$(INSTALL_MODE)"=="Release"
ICULIB_DLL=$(ICUROOT)\bin\icuin??.dll
!	ENDIF

!ELSE
ICUTARGET=
!ENDIF

!IFNDEF DESTDIR
DESTDIR=$(XALANCROOT)
!ENDIF 

!IF "$(prefix)x" != "x"
ZIP_DIR=$(DESTDIR)\$(prefix)
!ELSE
ZIP_DIR=$(DESTDIR)
!ENDIF

XSL_BIN_DEST=$(ZIP_DIR)\$(PACKAGE_NAME)


!IF "$(INSTALL_MODE)"=="Debug"
BIN_SRC_DIR=$(XALANCROOT)\Build\$(BITS)\$(IDE_VER)\Debug
XALAN_DLL=$(XSLT_DLL_NAME)D.dll
XALAN_LIB=$(PROJECT_NAME)_1D.lib
MSG_DLL=$(MSGLIB_DLL_NAME)D.dll
!ELSEIF "$(INSTALL_MODE)"=="Release"
BIN_SRC_DIR=$(XALANCROOT)\Build\$(BITS)\$(IDE_VER)\Release
XALAN_DLL=$(XSLT_DLL_NAME).dll
XALAN_LIB=$(PROJECT_NAME)_1.lib	
MSG_DLL=$(MSGLIB_DLL_NAME).dll
!ENDIF

!IF "$(XALANCROOT)" != "$(XSL_BIN_DEST)"
SAME_LOCATION=different_location
!ENDIF


ALL_SAMPLES =  $(XSL_BIN_DEST)\bin\CompileStylesheet.exe \
                  $(XSL_BIN_DEST)\bin\DocumentBuilder.exe \
                  $(XSL_BIN_DEST)\bin\ExternalFunction.exe \
                  $(XSL_BIN_DEST)\bin\ParsedSourceWrappers.exe \
                  $(XSL_BIN_DEST)\bin\SerializeNodeSet.exe \
                  $(XSL_BIN_DEST)\bin\SimpleTransform.exe \
                  $(XSL_BIN_DEST)\bin\SimpleXPathAPI.exe \
                  $(XSL_BIN_DEST)\bin\SimpleXPathCAPI.exe \
                  $(XSL_BIN_DEST)\bin\StreamTransform.exe \
                  $(XSL_BIN_DEST)\bin\TraceListen.exe \
                  $(XSL_BIN_DEST)\bin\TransformToXercesDOM.exe \
                  $(XSL_BIN_DEST)\bin\UseStylesheetParam.exe \
                  $(XSL_BIN_DEST)\bin\XalanTransform.exe \
                  $(XSL_BIN_DEST)\bin\XalanTransformerCallback.exe \
				  $(XSL_BIN_DEST)\bin\ThreadSafe.exe \
				  $(XSL_BIN_DEST)\bin\Xalan.exe

SAMPLES_DIRS= $(XSL_BIN_DEST)\samples\ApacheModuleXSLT 			\
				$(XSL_BIN_DEST)\samples\CompileStylesheet 		\
				$(XSL_BIN_DEST)\samples\ExternalFunction 		\
				$(XSL_BIN_DEST)\samples\DocumentBuilder 		\
				$(XSL_BIN_DEST)\samples\ParsedSourceWrappers 	\
				$(XSL_BIN_DEST)\samples\SerializeNodeSet 		\
				$(XSL_BIN_DEST)\samples\SimpleTransform 		\
				$(XSL_BIN_DEST)\samples\SimpleXPathAPI 			\
				$(XSL_BIN_DEST)\samples\SimpleXPathCAPI 		\
				$(XSL_BIN_DEST)\samples\StreamTransform 		\
				$(XSL_BIN_DEST)\samples\TraceListen 			\
				$(XSL_BIN_DEST)\samples\TransformToXercesDOM 	\
				$(XSL_BIN_DEST)\samples\UseStylesheetParam 		\
				$(XSL_BIN_DEST)\samples\XalanTransform 			\
				$(XSL_BIN_DEST)\samples\XalanTransformerCallback \
				$(XSL_BIN_DEST)\samples\ThreadSafe 							

SAMPLE_PROJ= $(XSL_BIN_DEST)\samples\Projects

# =================== End of check parameters  ===================================



install: libs include $(SAME_LOCATION) $(BUILD_DEBUG)
	
prepare: $(XSL_BIN_DEST)\bin $(XSL_BIN_DEST)\lib $(XSL_BIN_DEST)\include $(XSL_BIN_DEST)\samples
	
$(XSL_BIN_DEST)\bin $(XSL_BIN_DEST)\lib $(XSL_BIN_DEST)\include $(XSL_BIN_DEST)\samples:
	$(MKDIR) $@ 
	
libs: prepare $(XSL_BIN_DEST)\bin\$(XALAN_DLL) $(XSL_BIN_DEST)\lib\$(XALAN_LIB) $(XSL_BIN_DEST)\bin\$(MSG_DLL) $(ICUTARGET)

$(XSL_BIN_DEST)\bin\$(XALAN_DLL) $(XSL_BIN_DEST)\bin\$(MSG_DLL) $(XSL_BIN_DEST)\lib\$(XALAN_LIB): $(BIN_SRC_DIR)\$(@F)
	$(COPY) $(BIN_SRC_DIR)\$(@F) $(@D)

include: prepare $(XSL_BIN_DEST)\include\xalanc\Include\XalanVersion.hpp

		
$(XSL_BIN_DEST)\include\xalanc\Include\XalanVersion.hpp: $(XALANCROOT)\src\xalanc\Include\XalanVersion.hpp
	$(COPY) $(XALANCROOT)\src\xalanc\DOMSupport\*.h* 		$(XSL_BIN_DEST)\include\xalanc\DOMSupport
	$(COPY) $(XALANCROOT)\src\xalanc\ICUBridge\*.h* 		$(XSL_BIN_DEST)\include\xalanc\\ICUBridge
	$(COPY) $(XALANCROOT)\src\xalanc\Include\*.h* 		$(XSL_BIN_DEST)\include\xalanc\Include
	$(COPY) $(XALANCROOT)\src\xalanc\PlatformSupport\*.h* 		$(XSL_BIN_DEST)\include\xalanc\PlatformSupport
	$(COPY) $(XALANCROOT)\src\xalanc\TestXPath\*.h* 		$(XSL_BIN_DEST)\include\xalanc\TestXPath
	$(COPY) $(XALANCROOT)\src\xalanc\TestXSLT\*.h* 		$(XSL_BIN_DEST)\include\xalanc\TestXSLT
	$(COPY) $(XALANCROOT)\src\xalanc\XalanDOM\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XalanDOM
	$(COPY) $(XALANCROOT)\src\xalanc\XalanExe\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XalanExe
	$(COPY) $(XALANCROOT)\src\xalanc\XalanEXSLT\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XalanEXSLT
	$(COPY) $(XALANCROOT)\src\xalanc\XalanExtensions\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XalanExtensions
	$(COPY) $(XALANCROOT)\src\xalanc\XalanSourceTree\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XalanSourceTree
	$(COPY) $(XALANCROOT)\src\xalanc\XalanTransformer\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XalanTransformer
	$(COPY) $(XALANCROOT)\src\xalanc\XercesParserLiaison\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XercesParserLiaison
	$(COPY) $(XALANCROOT)\src\xalanc\XMLSupport\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XMLSupport
	$(COPY) $(XALANCROOT)\src\xalanc\XPath\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XPath
	$(COPY) $(XALANCROOT)\src\xalanc\XPathCAPI\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XPathCAPI
	$(COPY) $(XALANCROOT)\src\xalanc\XSLT\*.h* 		$(XSL_BIN_DEST)\include\xalanc\XSLT
	$(COPY) $(BIN_SRC_DIR)\Nls\Include\LocalMsgIndex.hpp $(XSL_BIN_DEST)\include\xalanc\PlatformSupport
			
icu:
	$(COPY) $(ICULIB_DLL) $(XSL_BIN_DEST)\bin

different_location: samples install-extras

samples: prepare $(ALL_SAMPLES) $(SAMPLES_DIRS) $(SAMPLE_PROJ)

$(SAMPLES_DIRS) : $(XALANCROOT)\samples\$(@F)
	$(MKDIR) $@
	$(COPY) $?\* $@
	-$(RMDIR) $@\CVS
	
$(SAMPLE_PROJ) : $(XALANCROOT)\samples\$(@F)
		$(MKDIR) $@\Win32\$(IDE_VER) 
		$(COPY) $?\Win32\$(IDE_VER) $@\Win32\$(IDE_VER) 
		-$(RMDIR) $@\Win32\$(IDE_VER)\CVS
		
$(ALL_SAMPLES): $(BIN_SRC_DIR)\$(@F)
	$(COPY) $? $(@D)
	
!IF "$(PROJECT_NAME)"!="xalan-c" 
install-extras:		$(XSL_BIN_DEST)\readme.html $(XSL_BIN_DEST)\LICENSE $(XSL_BIN_DEST)\License.html $(XSL_BIN_DEST)\license.html $(XSL_BIN_DEST)\XLicense.html $(XSL_BIN_DEST)\NOTICE
!ELSE
install-extras:		$(XSL_BIN_DEST)\readme.html $(XSL_BIN_DEST)\LICENSE $(XSL_BIN_DEST)\KEYS  $(XSL_BIN_DEST)\NOTICE
!ENDIF

$(XSL_BIN_DEST)\readme.html $(XSL_BIN_DEST)\KEYS $(XSL_BIN_DEST)\License.html $(XSL_BIN_DEST)\LICENSE $(XSL_BIN_DEST)\XLicense.html $(XSL_BIN_DEST)\NOTICE  : $(XALANCROOT)\$(@F)
	$(FILECOPY) $(XALANCROOT)\$(@F) $(@D)
	
zip: install 
	$(ZIP) $(ZIP_DIR)\$(PACKAGE_NAME).zip  $(ZIP_DIR)
	
	
clean:
	-$(RMDIR) $(ZIP_DIR)

	
build_debug:
	$(MAKE) XALANCROOT=$(XALANCROOT) prefix=$(prefix) INSTALL_MODE=Debug PACKAGE_NAME=$(PACKAGE_NAME) libs COPY="$(COPY)" IDE_VER=$(IDE_VER)

