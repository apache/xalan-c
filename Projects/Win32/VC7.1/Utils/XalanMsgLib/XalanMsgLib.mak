# Microsoft Developer Studio Generated NMAKE File, Based on XalanMsgLib.dsp
!IF "$(CFG)" == ""
CFG=XalanMsgLib - Win32 Debug
!MESSAGE No configuration specified. Defaulting to XalanMsgLib - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "XalanMsgLib - Win32 Release" && "$(CFG)" != "XalanMsgLib - Win32 Release.ICU" && "$(CFG)" != "XalanMsgLib - Win32 Debug" && "$(CFG)" != "XalanMsgLib - Win32 Debug.ICU" && "$(CFG)" != "XalanMsgLib - Win64 Release" && "$(CFG)" != "XalanMsgLib - Win64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XalanMsgLib.mak" CFG="XalanMsgLib - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XalanMsgLib - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanMsgLib - Win32 Release.ICU" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanMsgLib - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanMsgLib - Win32 Debug.ICU" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanMsgLib - Win64 Release" (based on "Win64 (x86) Console Application")
!MESSAGE "XalanMsgLib - Win64 Debug" (based on "Win64 (x86) Console Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

NO_EXTERNAL_DEPS=1

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe
LINK32=link.exe

XERCESINCLUDE=/I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include"
XERCESLIB=/libpath:"$(XERCESCROOT)\lib"
XERCESLIBDEBUG=$(XERCESLIB) /libpath:"$(XERCESCROOT)\Build\Win32\VC7.1\Debug"
XERCESLIBRELEASE=$(XERCESLIB) /libpath:"$(XERCESCROOT)\Build\Win32\VC7.1\Release"

LINK32_OBJS= \
	"$(INTDIR)\XalanMsgLib.obj" \
	"$(INTDIR)\Localization.res"

!IF  "$(CFG)" == "XalanMsgLib - Win32 Release"

BUILDRESDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Release
OUTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
INTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib

ALL : "$(BUILDRESDIR)\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC70.idb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "$(INTDIR)\Localization.res"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).dll"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).exp"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 $(XERCESINCLUDE) /I "..\..\..\..\..\src" /I ".\$(BUILDRESDIR)\Nls\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$(DLLNAME).bsc" 
BSC32_SBRS= \
	

LINK32_FLAGS=/nologo /dll /incremental:no /pdb:none /machine:I386 /out:"$(BUILDRESDIR)\$(DLLNAME).dll" /implib:"$(BUILDRESDIR)\$(DLLNAME).lib" $(XERCESLIBRELEASE)


"$(BUILDRESDIR)\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XalanMsgLib - Win32 Release.ICU"

BUILDRESDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Release.ICU
OUTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
INTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib

ALL : "$(BUILDRESDIR)\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC70.idb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "$(INTDIR)\Localization.res"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).dll"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).exp"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 $(XERCESINCLUDE) /I "..\..\..\..\..\src" /I ".\$(BUILDRESDIR)\Nls\Include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\$(DLLNAME).bsc" 
BSC32_SBRS= \
	

LINK32_FLAGS=/nologo /dll /incremental:no /pdb:none /machine:I386 /out:"$(BUILDRESDIR)\$(DLLNAME).dll" /implib:"$(BUILDRESDIR)\$(DLLNAME).lib" $(XERCESLIBRELEASE)


"$(BUILDRESDIR)\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XalanMsgLib - Win32 Debug"

BUILDRESDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Debug
OUTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
INTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
# Begin Custom Macros
OutDir=.\$(BUILDRESDIR)\Util\XalanMsgLib
# End Custom Macros

ALL : "$(BUILDRESDIR)\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC70.idb"
	-@erase "$(INTDIR)\VC70.pdb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "$(INTDIR)\Localization.res"	
	-@erase "$(INTDIR)\XalanMsgLib.sbr"
	-@erase "$(OUTDIR)\XalanMsgLib.bsc"
	-@erase "$(OUTDIR)\XalanMsgLib.exp"
	-@erase "$(OUTDIR)\$(DLLNAME).lib"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od $(XERCESINCLUDE) /I "..\..\..\..\..\src" /I ".\$(BUILDRESDIR)\Nls\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/D "_DEBUG" /win32 

LINK32_FLAGS= /nologo /dll /pdb:none /debug /machine:I386 /out:"$(BUILDRESDIR)\$(DLLNAME).dll" /implib:"$(BUILDRESDIR)\$(DLLNAME).lib" $(XERCESLIBDEBUG)


"$(BUILDRESDIR)\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XalanMsgLib - Win32 Debug.ICU"

BUILDRESDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU
OUTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
INTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
# Begin Custom Macros
OutDir=.\$(BUILDRESDIR)\Util\XalanMsgLib
# End Custom Macros

ALL : "$(BUILDRESDIR)\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC70.idb"
	-@erase "$(INTDIR)\VC70.pdb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "$(INTDIR)\Localization.res"	
	-@erase "$(INTDIR)\XalanMsgLib.sbr"
	-@erase "$(OUTDIR)\XalanMsgLib.bsc"
	-@erase "$(OUTDIR)\XalanMsgLib.exp"
	-@erase "$(OUTDIR)\$(DLLNAME).lib"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od $(XERCESINCLUDE) /I "..\..\..\..\..\src" /I ".\$(BUILDRESDIR)\Nls\Include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/D "_DEBUG" /win32 

LINK32_FLAGS= /nologo /dll /pdb:none /debug /machine:I386 /out:"$(BUILDRESDIR)\$(DLLNAME).dll" /implib:"$(BUILDRESDIR)\$(DLLNAME).lib" $(XERCESLIBDEBUG)


"$(BUILDRESDIR)\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XalanMsgLib - Win64 Release"

BUILDRESDIR=.\..\..\..\..\..\Build\Win64\VC7.1\Release
OUTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
INTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib

ALL : "$(BUILDRESDIR)\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC70.idb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "$(INTDIR)\Localization.res"	
	-@erase "$(BUILDRESDIR)\$(DLLNAME).dll"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).exp"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).lib"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W4 /GX /O2 $(XERCESINCLUDE) /I "..\..\..\..\..\src" /I ".\$(BUILDRESDIR)\Nls\Include" /D "WIN64" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
	

LINK32_FLAGS=/nologo /dll /incremental:no /pdb:none /machine:IX86 /out:"$(BUILDRESDIR)\$(DLLNAME).dll" /implib:"$(BUILDRESDIR)\$(DLLNAME).lib" /machine:IA64 $(XERCESLIBRELEASE)


"$(BUILDRESDIR)\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "XalanMsgLib - Win64 Debug"

BUILDRESDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Debug
OUTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
INTDIR=.\$(BUILDRESDIR)\Util\XalanMsgLib
# Begin Custom Macros
OutDir=.\$(BUILDRESDIR)\Util\XalanMsgLib
# End Custom Macros

ALL : "$(BUILDRESDIR)\$(DLLNAME).dll"


CLEAN :
	-@erase "$(INTDIR)\VC70.idb"
	-@erase "$(INTDIR)\VC70.pdb"
	-@erase "$(INTDIR)\XalanMsgLib.obj"
	-@erase "$(INTDIR)\Localization.res"	
	-@erase "$(OUTDIR)\XalanMsgLib.exp"
	-@erase "$(OUTDIR)\$(DLLNAME).lib"
	-@erase "$(BUILDRESDIR)\$(DLLNAME).dll"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W4 /Gm /GX /ZI /Od $(XERCESINCLUDE) /I "..\..\..\..\..\src" /I ".\$(BUILDRESDIR)\Nls\Include" /D "WIN64" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANMSGLIB_EXPORTS" /D "XALAN_XALANMSGLIB_BUILD_DLL" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/D "_DEBUG" /win32 


LINK32_FLAGS= /nologo /dll /pdb:none /debug /machine:IX86 /out:"$(BUILDRESDIR)\$(DLLNAME).dll" /implib:"$(BUILDRESDIR)\$(DLLNAME).lib" /machine:IA64 $(XERCESLIBDEBUG)


"$(BUILDRESDIR)\$(DLLNAME).dll" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("XalanMsgLib.dep")
!INCLUDE "XalanMsgLib.dep"
!ELSE 
!MESSAGE Warning: cannot find "XalanMsgLib.dep"
!ENDIF 
!ENDIF 



SOURCE=..\..\..\..\..\src\xalanc\Utils\XalanMsgLib\XalanMsgLib.cpp

RCSSOURCE=..\..\..\..\..\Projects\Win32\Res\Localization\Localization.rc

!IF  "$(CFG)" == "XalanMsgLib - Win32 Release" || "$(CFG)" == "XalanMsgLib - Win32 Release.ICU" || "$(CFG)" == "XalanMsgLib - Win64 Release"

!IF ! EXIST( "$(BUILDRESDIR)\Nls\Include\LocalMsgIndex.hpp" )
!MESSAGE Can't find a file: $(BUILDRESDIR)\nls\include\LocalMsgIndex.hpp  , it should be created by the build process, after running MsgCreator.exe application
!ENDIF

"$(INTDIR)\XalanMsgLib.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

"$(INTDIR)\Localization.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\Localization.res" /i "..\..\..\..\..\Projects\Win32\Res\Localization" /d "NDEBUG" $(RCSSOURCE)


!ELSEIF  "$(CFG)" == "XalanMsgLib - Win32 Debug" || "$(CFG)" == "XalanMsgLib - Win32 Debug.ICU" || "$(CFG)" == "XalanMsgLib - Win64 Debug"

!IF ! EXIST( "$(BUILDRESDIR)\Nls\Include\LocalMsgIndex.hpp" )
!ERROR Can't find a file: $(BUILDRESDIR)\nls\include\LocalMsgIndex.hpp  , it should be created by the build process, after running MsgCreator.exe application
!ENDIF

"$(INTDIR)\XalanMsgLib.obj"	"$(INTDIR)\XalanMsgLib.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)

"$(INTDIR)\Localization.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) /l 0x409 /fo"$(INTDIR)\Localization.res" /i "..\..\..\..\..\Projects\Win32\Res\Localization" /d "_DEBUG" $(RCSSOURCE)
	
!ENDIF 
