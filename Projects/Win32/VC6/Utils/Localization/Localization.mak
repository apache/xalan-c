# This makefike requires Tools.ini file with settings

# Check parameters:
# Should be defined:
# LOCAL, in form of "en" or "en_US"
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

!IF "$(LOCAL)" == ""
!	MESSAGE Local is not defined. The local can be defined in the command line or in Tools.ini file
!	ERROR Local must be defined
!ELSE
!	MESSAGE Used LOCAL 	= $(LOCAL)
!ENDIF

!IF "$(TYPE)" == ""
!	ERROR Localization type must be defined.Possible values are "inmem" and "icu"
!ENDIF

!IF "$(TYPE)" != "inmem" && "$(TYPE)" != "icu"
!	ERROR Invalid configuration TYPE="$(TYPE)" specified. 
!ELSE
!	MESSAGE Used TYPE 	= $(TYPE)
!ENDIF

!IF "$(TYPE)" == "icu" && "$(ICUROOT)"=="UNDEFINED"
!ERROR Please define ICUROOT in xml-xalan\c\Projects\Win32\VC6\Utils\Localization\tools.ini file
!ENDIF

!IF EXIST ($(NLSDIR)\$(LOCAL)\$(MSGFILENAME)$(LOCAL)$(XIFFEXT))
!	MESSAGE Used XLIFF file: "$(MSGFILENAME)$(LOCAL)$(XIFFEXT)"
!ELSE
!	ERROR File "$(NLSDIR)\$(LOCAL)\$(MSGFILENAME)$(LOCAL)$(XIFFEXT)" is required
!ENDIF


#====================== INMEM part =================================================================

!IF "$(TYPE)" == "inmem"

ALL :	$(OUTPUTDIR)\$(PKGNAME).dll $(INDEFFILEDIR)\XalanMsgIndex.hpp

	
$(OUTPUTDIR)\$(PKGNAME).dll : $(XALANMSGLIB_SRC)\LocalMsgData.hpp
	$(MAKE) /$(MAKEFLAGS) /NOLOGO /f ..\XalanMsgLib\XalanMsgLib.mak CFG="XalanMsgLib - $(BITS) $(CFG)" VER=$(VER) $(MAKE_PARAMS)
	
$(INDEFFILEDIR)\XalanMsgIndex.hpp:

$(XALANMSGLIB_SRC)\LocalMsgData.hpp   : $(NLSDIR)\$(LOCAL)\$(MSGFILENAME)$(LOCAL)$(XIFFEXT) 
	@if not exist $(OUTPUTDIR)\MsgCreator.exe ( $(MAKE) /f ..\MsgCreator\MsgCreator.mak CFG="MsgCreator - $(BITS) $(CFG)" $(MAKE_PARAMS))
	$(OUTPUTDIR)\MsgCreator.exe $(NLSDIR)\$(LOCAL)\$(MSGFILENAME)$(LOCAL)$(XIFFEXT) -TYPE $(TYPE)
	@move LocalMsgIndex.hpp $(INDEFFILEDIR)
	@move LocalMsgData.hpp	$(XALANMSGLIB_SRC)
	
#====================== End of INMEM part =============================================================
		
#====================== ICU part ======================================================================
	

!ELSEIF "$(TYPE)" == "icu"

GENRB    = genrb.exe
PKGDATA  = pkgdata.exe


ALL :	$(OUTPUTDIR)\$(PKGNAME).dll $(INDEFFILEDIR)\XalanMsgIndex.hpp

	
$(OUTPUTDIR)\$(PKGNAME).dll : $(NLSDIR)\$(LOCAL)\$(LOCAL).txt
	$(GENRB) --package-name $(PKGNAME) -d $(NLSDIR)\$(LOCAL)  $(NLSDIR)\$(LOCAL)\$(LOCAL).txt
	echo $(NLSDIR)\$(LOCAL)\$(PKGNAME)_$(LOCAL).res > res-file-list.txt
	$(PKGDATA) --name $(PKGNAME) -T $(OUTPUTDIR) -v -O R:$(ICUROOT)  --mode dll -d $(OUTPUTDIR) res-file-list.txt	

$(INDEFFILEDIR)\XalanMsgIndex.hpp : $(NLSDIR)\$(LOCAL)\$(LOCAL).txt			
			
$(NLSDIR)\$(LOCAL)\$(LOCAL).txt : $(NLSDIR)\$(LOCAL)\$(MSGFILENAME)$(LOCAL)$(XIFFEXT)
	if not exist $(OUTPUTDIR)\MsgCreator.exe ( $(MAKE) /f ..\MsgCreator\MsgCreator.mak CFG="MsgCreator - $(BITS) $(CFG)" $(MAKE_PARAMS))
	$(OUTPUTDIR)\MsgCreator.exe $(NLSDIR)\$(LOCAL)\$(MSGFILENAME)$(LOCAL)$(XIFFEXT) -TYPE $(TYPE) -LOCAL $(LOCAL)
	@move LocalMsgIndex.hpp $(INDEFFILEDIR)
	if not exist $(NLSDIR)\$(LOCAL) (mkdir $(NLSDIR)\$(LOCAL) )
	@move $(LOCAL).txt $(NLSDIR)\$(LOCAL)
	
#====================== End of ICU part ==================================================================

!ENDIF

#====================== Commom part =================================================================
CLEAN : 
	-@erase $(NLSDIR)\$(LOCAL)\$(LOCAL).txt
	-@del $(NLSDIR)\ICU\$(PKGNAME)_$(LOCAL).res
	-@erase res-file-list.txt
	-@erase $(OUTPUTDIR)\$(PKGNAME)*
	$(MAKE) /f ..\MsgCreator\MsgCreator.mak CFG="MsgCreator - $(BITS) $(CFG)" CLEAN
	$(MAKE) /NOLOGO /f ..\XalanMsgLib\XalanMsgLib.mak CFG="XalanMsgLib - $(BITS) $(CFG)" CLEAN	
	-@erase $(INDEFFILEDIR)\XalanMsgIndex.hpp
	-@erase	$(XALANMSGLIB_SRC)\LocalMsgData.hpp

#====================== End of commom part ==========================================================
