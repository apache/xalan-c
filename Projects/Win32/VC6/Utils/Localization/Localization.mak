# This makefike requires Tools.ini file with settings

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
PKGNAME=$(LIBNAME)_$(VER)D
!ELSEIF "$(CFG)" == "Release.symbols" 
PKGNAME=$(LIBNAME)_$(VER)S
!ELSE
PKGNAME=$(LIBNAME)_$(VER)
!ENDIF

#====================== INMEM part =================================================================

!IF "$(TYPE)" == "inmem"


ALL :	PREPARE $(TMPINCLUDESDIR)\LocalMsgData.hpp $(OUTPUTDIR)\$(PKGNAME).dll

	
$(OUTPUTDIR)\$(PKGNAME).dll : $(TMPINCLUDESDIR)\LocalMsgData.hpp
	$(MAKE) /$(MAKEFLAGS) /NOLOGO /f ..\XalanMsgLib\XalanMsgLib.mak CFG="XalanMsgLib - $(BITS) $(CFG)" DLLNAME=$(PKGNAME) $(MAKE_PARAMS)
	

$(TMPINCLUDESDIR)\LocalMsgData.hpp   : $(NLSDIR)\$(LOCALE)\$(MSGFILENAME)$(LOCALE)$(XIFFEXT) 
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
	$(GENRB) --package-name $(PKGNAME) -d $(INTDIR)\Icu $(INTDIR)\Icu\$(LOCALE).txt
	echo $(INTDIR)\Icu\$(PKGNAME)_$(LOCALE).res > $(INTDIR)\Icu\res-file-list.txt
	$(PKGDATA) --name $(PKGNAME) -T $(INTDIR)\Icu -v -O R:$(ICUROOT) --mode dll -d $(OUTPUTDIR) $(INTDIR)\Icu\res-file-list.txt	
			
			
$(INTDIR)\Icu\$(LOCALE).txt : $(NLSDIR)\$(LOCALE)\$(MSGFILENAME)$(LOCALE)$(XIFFEXT)
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
	-@erase $(TMPINCLUDESDIR)\XalanMsgIndex.hpp
	-@erase	$(TMPINCLUDESDIR)\LocalMsgData.hpp
!IF "$(TYPE)" == "icu"
	$(PKGDATA) --name $(PKGNAME) -T $(INTDIR)\Icu -v -O R:$(ICUROOT) -k --mode dll -d $(OUTPUTDIR) $(INTDIR)\Icu\res-file-list.txt
!ENDIF
	-@erase $(INTDIR)\Icu\$(LOCALE).txt
	-@del $(NLSDIR)\ICU\$(PKGNAME)_$(LOCALE).res
	-@erase $(INTDIR)\Icu\res-file-list.txt
	-@erase $(OUTPUTDIR)\$(PKGNAME)*
PREPARE:

#====================== End of commom part ==========================================================
