# This makefike requires Tools.ini file with settings
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

# Check parameters:
# Should be defined:
# LOCALE, in form of "en" or "en_US"
# TYPE, possible values are "inmem" and "icu"


!IF "$(BITS)" != "Win32" && "$(BITS)" != "Win64"
!	ERROR Invalid configuration BITS="$(BITS)" specified. 
!ELSE
!MESSAGE Used BITS 	= $(BITS)
!ENDIF

!IF "$(CFG)" != "Debug" && "$(CFG)" != "Release" && "$(CFG)" != "Release.symbols"
!	ERROR Invalid configuration CFG="$(CFG)" specified. 
!ELSE
!MESSAGE Used CFG 	= $(CFG)
!ENDIF

!IF "$(LOCALE)" == ""
!	MESSAGE Locale is not defined. The locale can be defined in the command line or in Tools.ini file
!	ERROR Locale must be defined
!ELSE
!	MESSAGE Used LOCALE 	= $(LOCALE)
!ENDIF

!IF "$(TYPE)" == ""
!	ERROR Localization type must be defined.Possible values are "inmem" and "icu"
!ENDIF

!IF "$(TYPE)" != "inmem" && "$(TYPE)" != "icu"
!	ERROR Invalid configuration TYPE="$(TYPE)" specified. 
!ELSE
!	MESSAGE Used TYPE 	= $(TYPE)
!ENDIF

!IF "$(TYPE)" == "icu" && "$(ICUROOT)"==""
!ERROR Please define ICUROOT in xml-xalan\c\Projects\Win32\VC6\Utils\Localization\tools.ini file
!ENDIF

!IF EXIST ($(NLSDIR)\$(LOCALE)\$(MSGFILENAME)$(LOCALE)$(XIFFEXT))
!	MESSAGE Used XLIFF file: "$(MSGFILENAME)$(LOCALE)$(XIFFEXT)"
!ELSE
!	ERROR File "$(NLSDIR)\$(LOCALE)\$(MSGFILENAME)$(LOCALE)$(XIFFEXT)" is required
!ENDIF


include ..\..\..\..\..\version.incl

#== Commmon tools
MKDIR=mkdir
MOVE=move

!IF "$(CFG)" == "Debug"
PKGNAME=$(LIBNAME)_$(MS_VER)D
!ELSEIF "$(CFG)" == "Release.symbols" 
PKGNAME=$(LIBNAME)_$(MS_VER)S
!ELSE
PKGNAME=$(LIBNAME)_$(MS_VER)
!ENDIF

XERCESPATH=$(XERCESCROOT)\bin;$(XERCESCROOT)\Build\Win32\VC6\Release;$(XERCESCROOT)\Build\Win32\VC6\Debug;$(XERCESCROOT)\Build\Win64\VC6\Release;$(XERCESCROOT)\Build\Win64\VC6\Debug
ICUPATH=$(ICUROOT)\bin

#====================== INMEM part =================================================================

!IF "$(TYPE)" == "inmem"


ALL :	PREPARE $(TMPINCLUDESDIR)\LocalMsgData.hpp $(OUTPUTDIR)\$(PKGNAME).dll

	
$(OUTPUTDIR)\$(PKGNAME).dll : $(TMPINCLUDESDIR)\LocalMsgData.hpp
	$(MAKE) /$(MAKEFLAGS) /NOLOGO /f ..\XalanMsgLib\XalanMsgLib.mak CFG="XalanMsgLib - $(BITS) $(CFG)" DLLNAME=$(PKGNAME) $(MAKE_PARAMS)
	

$(TMPINCLUDESDIR)\LocalMsgData.hpp   : $(NLSDIR)\$(LOCALE)\$(MSGFILENAME)$(LOCALE)$(XIFFEXT) 
	@set PATH=$(ICUPATH);$(XERCESPATH);$(PATH)
	@if not exist $(OUTPUTDIR)\MsgCreator.exe ( $(MAKE) /f ..\MsgCreator\MsgCreator.mak CFG="MsgCreator - $(BITS) $(CFG)" $(MAKE_PARAMS))
	$(OUTPUTDIR)\MsgCreator.exe $(NLSDIR)\$(LOCALE)\$(MSGFILENAME)$(LOCALE)$(XIFFEXT) -TYPE $(TYPE) -LOCALE $(LOCALE)
	@$(MOVE) LocalMsgIndex.hpp 	$(TMPINCLUDESDIR)
	@$(MOVE) LocalMsgData.hpp	$(TMPINCLUDESDIR)
	
PREPARE:
	@if not exist $(TMPINCLUDESDIR) ( $(MKDIR) $(TMPINCLUDESDIR) ) 	
	
#====================== End of INMEM part =============================================================
		
#====================== ICU part ======================================================================
	

!ELSEIF "$(TYPE)" == "icu"

GENRB    = genrb.exe
PKGDATA  = pkgdata.exe



ALL :	PREPARE $(INTDIR)\Icu\$(LOCALE).txt $(OUTPUTDIR)\$(PKGNAME).dll

	
$(OUTPUTDIR)\$(PKGNAME).dll : $(INTDIR)\Icu\$(LOCALE).txt
	$(ICUPATH)\$(GENRB) --package-name $(PKGNAME) -d $(INTDIR)\Icu $(INTDIR)\Icu\$(LOCALE).txt
	echo $(INTDIR)\Icu\$(PKGNAME)_$(LOCALE).res > $(INTDIR)\Icu\res-file-list.txt
	$(ICUPATH)\$(PKGDATA) --name $(PKGNAME) -T $(INTDIR)\Icu -v --mode dll -d $(OUTPUTDIR) $(INTDIR)\Icu\res-file-list.txt
			
			
$(INTDIR)\Icu\$(LOCALE).txt : $(NLSDIR)\$(LOCALE)\$(MSGFILENAME)$(LOCALE)$(XIFFEXT)
	@set PATH=$(ICUPATH);$(XERCESPATH);$(PATH)
	if not exist $(OUTPUTDIR)\MsgCreator.exe ( $(MAKE) /f ..\MsgCreator\MsgCreator.mak CFG="MsgCreator - $(BITS) $(CFG)" $(MAKE_PARAMS))
	$(OUTPUTDIR)\MsgCreator.exe $(NLSDIR)\$(LOCALE)\$(MSGFILENAME)$(LOCALE)$(XIFFEXT) -TYPE $(TYPE) -LOCALE $(LOCALE)
	@$(MOVE) LocalMsgIndex.hpp $(TMPINCLUDESDIR)
	@$(MOVE) $(LOCALE).txt $(INTDIR)\Icu
	
PREPARE:
	@if not exist $(INTDIR)\Icu ( $(MKDIR) $(INTDIR)\Icu )
	@if not exist $(TMPINCLUDESDIR) ( $(MKDIR) $(TMPINCLUDESDIR) ) 	
#====================== End of ICU part ==================================================================

!ENDIF

#====================== Commom part =================================================================
CLEAN : 
	$(MAKE) /f ..\MsgCreator\MsgCreator.mak CFG="MsgCreator - $(BITS) $(CFG)" CLEAN
	$(MAKE) /NOLOGO /f ..\XalanMsgLib\XalanMsgLib.mak CFG="XalanMsgLib - $(BITS) $(CFG)" CLEAN	
	-@erase $(TMPINCLUDESDIR)\LocalMsgIndex.hpp
	-@erase	$(TMPINCLUDESDIR)\LocalMsgData.hpp
!IF "$(TYPE)" == "icu"
	$(ICUPATH)\$(PKGDATA) --name $(PKGNAME) -T $(INTDIR)\Icu -v -k --mode dll -d $(OUTPUTDIR) $(INTDIR)\Icu\res-file-list.txt
!ENDIF
	-@erase $(INTDIR)\Icu\$(LOCALE).txt
	-@del $(NLSDIR)\ICU\$(PKGNAME)_$(LOCALE).res
	-@erase $(INTDIR)\Icu\res-file-list.txt
	-@erase $(OUTPUTDIR)\$(PKGNAME)*
PREPARE:

#====================== End of commom part ==========================================================
