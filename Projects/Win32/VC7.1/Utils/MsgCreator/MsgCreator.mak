# Microsoft Developer Studio Generated NMAKE File, Based on MsgCreator.dsp
!IF "$(CFG)" == ""
CFG=MsgCreator - Win32 Debug
!MESSAGE No configuration specified. Defaulting to MsgCreator - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "MsgCreator - Win32 Release" && "$(CFG)" != "MsgCreator - Win32 Release.ICU" && "$(CFG)" != "MsgCreator - Win32 Debug" && "$(CFG)" != "MsgCreator - Win32 Debug.ICU" && "$(CFG)" != "MsgCreator - Win64 Release" && "$(CFG)" != "MsgCreator - Win64 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MsgCreator.mak" CFG="MsgCreator - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MsgCreator - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "MsgCreator - Win32 Release.ICU" (based on "Win32 (x86) Console Application")
!MESSAGE "MsgCreator - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "MsgCreator - Win32 Debug.ICU" (based on "Win32 (x86) Console Application")
!MESSAGE "MsgCreator - Win64 Release" (based on "Win64 (x86) Console Application")
!MESSAGE "MsgCreator - Win64 Debug" (based on "Win64 (x86) Console Application")
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
RSC=rc.exe
XALANINCLUDE=.\..\..\..\..\..\src
XERCESINCLUDE=/I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include"
XERCESLIB=/libpath:"$(XERCESCROOT)\lib"
XERCESLIBDEBUG=$(XERCESLIB) /libpath:"$(XERCESCROOT)\Build\Win32\VC7.1\Debug"
XERCESLIBRELEASE=$(XERCESLIB) /libpath:"$(XERCESCROOT)\Build\Win32\VC7.1\Release"

NO_EXTERNAL_DEPS=1


!IF  "$(CFG)" == "MsgCreator - Win32 Release"

OUTDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Release\Util\MsgCreator
INTDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Release\Util\MsgCreator

ALL : "..\..\..\..\..\Build\Win32\VC7.1\Release\MsgCreator.exe"
CLEAN :
	-@erase "$(INTDIR)\ICUResHandler.obj"
	-@erase "$(INTDIR)\InMemHandler.obj"
	-@erase "$(INTDIR)\MsgCreator.obj"
	-@erase "$(INTDIR)\MsgFileOutputStream.obj"
	-@erase "$(INTDIR)\NLSHandler.obj"
	-@erase "$(INTDIR)\SAX2Handler.obj"
	-@erase "$(INTDIR)\vc70.idb"
	-@erase "$(INTDIR)\MsgCreator.pch"
	-@erase "..\..\..\..\..\Build\Win32\VC7.1\Release\MsgCreator.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W4 /GX /O2 /I ".\\" /I $(XALANINCLUDE) $(XERCESINCLUDE) /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\MsgCreator.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MsgCreator.bsc" 
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_3.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\MsgCreator.pdb" /machine:I386  $(XERCESLIBRELEASE) /out:"..\..\..\..\..\Build\Win32\VC7.1\Release\MsgCreator.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ICUResHandler.obj" \
	"$(INTDIR)\InMemHandler.obj" \
	"$(INTDIR)\MsgCreator.obj" \
	"$(INTDIR)\NLSHandler.obj" \
	"$(INTDIR)\SAX2Handler.obj" \
	"$(INTDIR)\MsgFileOutputStream.obj"

"..\..\..\..\..\Build\Win32\VC7.1\Release\MsgCreator.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Release.ICU"

OUTDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Release.ICU\Util\MsgCreator
INTDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Release.ICU\Util\MsgCreator
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win32\VC7.1\Release.ICU\Util\MsgCreator
# End Custom Macros

ALL : "..\..\..\..\..\Build\Win32\VC7.1\Release.ICU\MsgCreator.exe"

CLEAN :
	-@erase "$(INTDIR)\ICUResHandler.obj"
	-@erase "$(INTDIR)\InMemHandler.obj"
	-@erase "$(INTDIR)\MsgCreator.obj"
	-@erase "$(INTDIR)\MsgFileOutputStream.obj"
	-@erase "$(INTDIR)\NLSHandler.obj"
	-@erase "$(INTDIR)\SAX2Handler.obj"
	-@erase "$(INTDIR)\vc70.idb"
	-@erase "$(INTDIR)\MsgCreator.pch"
	-@erase "..\..\..\..\..\Build\Win32\VC7.1\Release.ICU\MsgCreator.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W4 /GX /O2 /I ".\\" /I $(XALANINCLUDE) $(XERCESINCLUDE) /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\MsgCreator.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MsgCreator.bsc" 
BSC32_SBRS= \

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_3.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\MsgCreator.pdb" /machine:I386  $(XERCESLIBRELEASE) /out:"..\..\..\..\..\Build\Win32\VC7.1\Release.ICU\MsgCreator.exe" 
LINK32_OBJS= \
	"$(INTDIR)\ICUResHandler.obj" \
	"$(INTDIR)\InMemHandler.obj" \
	"$(INTDIR)\MsgCreator.obj" \
	"$(INTDIR)\NLSHandler.obj" \
	"$(INTDIR)\SAX2Handler.obj" \
	"$(INTDIR)\MsgFileOutputStream.obj"

"..\..\..\..\..\Build\Win32\VC7.1\Release.ICU\MsgCreator.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Debug"

OUTDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Debug\Util\MsgCreator
INTDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Debug\Util\MsgCreator
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win32\VC7.1\Debug\Util\MsgCreator
# End Custom Macros

ALL : "..\..\..\..\..\Build\Win32\VC7.1\Debug\MsgCreator.exe" "$(OUTDIR)\MsgCreator.bsc"


CLEAN :
	-@erase "$(INTDIR)\ICUResHandler.obj"
	-@erase "$(INTDIR)\ICUResHandler.sbr"
	-@erase "$(INTDIR)\InMemHandler.obj"
	-@erase "$(INTDIR)\InMemHandler.sbr"
	-@erase "$(INTDIR)\MsgCreator.obj"
	-@erase "$(INTDIR)\MsgCreator.sbr"
	-@erase "$(INTDIR)\MsgFileOutputStream.obj"
	-@erase "$(INTDIR)\MsgFileOutputStream.sbr"
	-@erase "$(INTDIR)\NLSHandler.obj"
	-@erase "$(INTDIR)\NLSHandler.sbr"
	-@erase "$(INTDIR)\SAX2Handler.obj"
	-@erase "$(INTDIR)\SAX2Handler.sbr"
	-@erase "$(INTDIR)\vc70.idb"
	-@erase "$(INTDIR)\vc70.pdb"
	-@erase "$(OUTDIR)\MsgCreator.bsc"
	-@erase "$(OUTDIR)\MsgCreator.pch"
	-@erase "$(OUTDIR)\MsgCreator.pdb"
	-@erase "..\..\..\..\..\Build\Win32\VC7.1\Debug\MsgCreator.exe"
	-@erase "..\..\..\..\..\Build\Win32\VC7.1\Debug\MsgCreator.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W4 /Gm /GX /ZI /Od /I ".\\" /I $(XALANINCLUDE) $(XERCESINCLUDE) /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\MsgCreator.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MsgCreator.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ICUResHandler.sbr" \
	"$(INTDIR)\InMemHandler.sbr" \
	"$(INTDIR)\MsgCreator.sbr" \
	"$(INTDIR)\NLSHandler.sbr" \
	"$(INTDIR)\SAX2Handler.sbr" \
	"$(INTDIR)\MsgFileOutputStream.sbr"

"$(OUTDIR)\MsgCreator.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_3D.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\MsgCreator.pdb" /debug /machine:I386  $(XERCESLIBDEBUG) /out:"..\..\..\..\..\Build\Win32\VC7.1\Debug\MsgCreator.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ICUResHandler.obj" \
	"$(INTDIR)\InMemHandler.obj" \
	"$(INTDIR)\MsgCreator.obj" \
	"$(INTDIR)\NLSHandler.obj" \
	"$(INTDIR)\SAX2Handler.obj" \
	"$(INTDIR)\MsgFileOutputStream.obj"

"..\..\..\..\..\Build\Win32\VC7.1\Debug\MsgCreator.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Debug.ICU"

OUTDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU\Util\MsgCreator
INTDIR=.\..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU\Util\MsgCreator
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU\Util\MsgCreator
# End Custom Macros

ALL : "..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU\MsgCreator.exe" "$(OUTDIR)\MsgCreator.bsc"


CLEAN :
	-@erase "$(INTDIR)\ICUResHandler.obj"
	-@erase "$(INTDIR)\ICUResHandler.sbr"
	-@erase "$(INTDIR)\InMemHandler.obj"
	-@erase "$(INTDIR)\InMemHandler.sbr"
	-@erase "$(INTDIR)\MsgCreator.obj"
	-@erase "$(INTDIR)\MsgCreator.sbr"
	-@erase "$(INTDIR)\MsgFileOutputStream.obj"
	-@erase "$(INTDIR)\MsgFileOutputStream.sbr"
	-@erase "$(INTDIR)\NLSHandler.obj"
	-@erase "$(INTDIR)\NLSHandler.sbr"
	-@erase "$(INTDIR)\SAX2Handler.obj"
	-@erase "$(INTDIR)\SAX2Handler.sbr"
	-@erase "$(INTDIR)\vc70.idb"
	-@erase "$(INTDIR)\vc70.pdb"
	-@erase "$(OUTDIR)\MsgCreator.bsc"
	-@erase "$(INTDIR)\MsgCreator.pch"
	-@erase "$(OUTDIR)\MsgCreator.pdb"
	-@erase "..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU\MsgCreator.exe"
	-@erase "..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU\MsgCreator.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W4 /Gm /GX /ZI /Od /I ".\\" /I $(XALANINCLUDE) $(XERCESINCLUDE) /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\MsgCreator.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MsgCreator.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ICUResHandler.sbr" \
	"$(INTDIR)\InMemHandler.sbr" \
	"$(INTDIR)\MsgCreator.sbr" \
	"$(INTDIR)\NLSHandler.sbr" \
	"$(INTDIR)\SAX2Handler.sbr" \
	"$(INTDIR)\MsgFileOutputStream.sbr"

"$(OUTDIR)\MsgCreator.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_3D.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\MsgCreator.pdb" /debug /machine:I386  $(XERCESLIBDEBUG) /out:"..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU\MsgCreator.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\ICUResHandler.obj" \
	"$(INTDIR)\InMemHandler.obj" \
	"$(INTDIR)\MsgCreator.obj" \
	"$(INTDIR)\NLSHandler.obj" \
	"$(INTDIR)\SAX2Handler.obj" \
	"$(INTDIR)\MsgFileOutputStream.obj"

"..\..\..\..\..\Build\Win32\VC7.1\Debug.ICU\MsgCreator.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MsgCreator - Win64 Debug"

OUTDIR=.\..\..\..\..\..\Build\Win64\VC7.1\Debug\Util\MsgCreator
INTDIR=.\..\..\..\..\..\Build\Win64\VC7.1\Debug\Util\MsgCreator
# Begin Custom Macros
OutDir=.\..\..\..\..\..\Build\Win64\VC7.1\Debug\Util\MsgCreator
# End Custom Macros

ALL : "..\..\..\..\..\Build\Win64\VC7.1\Debug\MsgCreator.exe" "$(OUTDIR)\MsgCreator.bsc"


CLEAN :
	-@erase "$(INTDIR)\ICUResHandler.obj"
	-@erase "$(INTDIR)\ICUResHandler.sbr"
	-@erase "$(INTDIR)\InMemHandler.obj"
	-@erase "$(INTDIR)\InMemHandler.sbr"
	-@erase "$(INTDIR)\MsgCreator.obj"
	-@erase "$(INTDIR)\MsgCreator.sbr"
	-@erase "$(INTDIR)\MsgFileOutputStream.obj"
	-@erase "$(INTDIR)\MsgFileOutputStream.sbr"
	-@erase "$(INTDIR)\NLSHandler.obj"
	-@erase "$(INTDIR)\NLSHandler.sbr"
	-@erase "$(INTDIR)\SAX2Handler.obj"
	-@erase "$(INTDIR)\SAX2Handler.sbr"
	-@erase "$(INTDIR)\vc70.idb"
	-@erase "$(INTDIR)\vc70.pdb"
	-@erase "$(OUTDIR)\MsgCreator.bsc"
	-@erase "$(INTDIR)\MsgCreator.pch"
	-@erase "$(OUTDIR)\MsgCreator.pdb"
	-@erase "..\..\..\..\..\Build\Win64\VC7.1\Debug\MsgCreator.exe"
	-@erase "..\..\..\..\..\Build\Win64\VC7.1\Debug\MsgCreator.ilk"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MLd /W4 /Gm /GX /ZI /Od /I ".\\" /I $(XALANINCLUDE) $(XERCESINCLUDE) /D "WIN32" /D "WIN64" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /Fr"$(INTDIR)\\" /Fp"$(INTDIR)\MsgCreator.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MsgCreator.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\ICUResHandler.sbr" \
	"$(INTDIR)\InMemHandler.sbr" \
	"$(INTDIR)\MsgCreator.sbr" \
	"$(INTDIR)\NLSHandler.sbr" \
	"$(INTDIR)\SAX2Handler.sbr" \
	"$(INTDIR)\MsgFileOutputStream.sbr"

"$(OUTDIR)\MsgCreator.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_3D.lib /nologo /subsystem:console /incremental:yes /pdb:"$(OUTDIR)\MsgCreator.pdb" /debug /machine:IX86 $(XERCESLIBDEBUG) /out:"..\..\..\..\..\Build\Win64\VC7.1\Debug\MsgCreator.exe" /pdbtype:sept /machine:IA64
LINK32_OBJS= \
	"$(INTDIR)\ICUResHandler.obj" \
	"$(INTDIR)\InMemHandler.obj" \
	"$(INTDIR)\MsgCreator.obj" \
	"$(INTDIR)\NLSHandler.obj" \
	"$(INTDIR)\SAX2Handler.obj" \
	"$(INTDIR)\MsgFileOutputStream.obj"

"..\..\..\..\..\Build\Win64\VC7.1\Debug\MsgCreator.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "MsgCreator - Win64 Release" 

OUTDIR=.\..\..\..\..\..\Build\Win64\VC7.1\Release\Util\MsgCreator
INTDIR=.\..\..\..\..\..\Build\Win64\VC7.1\Release\Util\MsgCreator

ALL : "..\..\..\..\..\Build\Win64\VC7.1\Release\MsgCreator.exe"
CLEAN :
	-@erase "$(INTDIR)\ICUResHandler.obj"
	-@erase "$(INTDIR)\InMemHandler.obj"
	-@erase "$(INTDIR)\MsgCreator.obj"
	-@erase "$(INTDIR)\MsgFileOutputStream.obj"
	-@erase "$(INTDIR)\NLSHandler.obj"
	-@erase "$(INTDIR)\SAX2Handler.obj"
	-@erase "$(INTDIR)\vc70.idb"
	-@erase "..\..\..\..\..\Build\Win64\VC7.1\Release\MsgCreator.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /ML /W4 /GX /O2 /I ".\\" /I $(XALANINCLUDE) $(XERCESINCLUDE) /D "WIN64" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /Fp"$(INTDIR)\MsgCreator.pch" /YX /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\MsgCreator.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=kernel32.lib user32.lib xerces-c_3.lib /nologo /subsystem:console /incremental:no /pdb:"$(OUTDIR)\MsgCreator.pdb" /machine:IX86 $(XERCESLIBRELEASE) /out:"..\..\..\..\..\Build\Win64\VC7.1\Release\MsgCreator.exe" /machine:IA64
LINK32_OBJS= \
	"$(INTDIR)\ICUResHandler.obj" \
	"$(INTDIR)\InMemHandler.obj" \
	"$(INTDIR)\MsgCreator.obj" \
	"$(INTDIR)\NLSHandler.obj" \
	"$(INTDIR)\SAX2Handler.obj" \
	"$(INTDIR)\MsgFileOutputStream.obj"

"..\..\..\..\..\Build\Win64\VC7.1\Release\MsgCreator.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
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
!IF EXISTS("MsgCreator.dep")
!INCLUDE "MsgCreator.dep"
!ELSE 
!MESSAGE Warning: cannot find "MsgCreator.dep"
!ENDIF 
!ENDIF 



SOURCE=..\..\..\..\..\src\xalanc\Utils\MsgCreator\ICUResHandler.cpp

!IF  "$(CFG)" == "MsgCreator - Win32 Release" || "$(CFG)" == "MsgCreator - Win32 Release.ICU" || "$(CFG)" == "MsgCreator - Win64 Release"


"$(INTDIR)\ICUResHandler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Debug" || "$(CFG)" == "MsgCreator - Win32 Debug.ICU" || "$(CFG)" == "MsgCreator - Win64 Debug"


"$(INTDIR)\ICUResHandler.obj"	"$(INTDIR)\ICUResHandler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\..\src\xalanc\Utils\MsgCreator\InMemHandler.cpp

!IF  "$(CFG)" == "MsgCreator - Win32 Release" || "$(CFG)" == "MsgCreator - Win32 Release.ICU" || "$(CFG)" == "MsgCreator - Win64 Release"


"$(INTDIR)\InMemHandler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Debug" || "$(CFG)" == "MsgCreator - Win32 Debug.ICU" || "$(CFG)" == "MsgCreator - Win64 Debug"


"$(INTDIR)\InMemHandler.obj"	"$(INTDIR)\InMemHandler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\..\src\xalanc\Utils\MsgCreator\MsgCreator.cpp

!IF  "$(CFG)" == "MsgCreator - Win32 Release" || "$(CFG)" == "MsgCreator - Win32 Release.ICU" || "$(CFG)" == "MsgCreator - Win64 Release"


"$(INTDIR)\MsgCreator.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Debug" || "$(CFG)" == "MsgCreator - Win32 Debug.ICU" || "$(CFG)" == "MsgCreator - Win64 Debug"


"$(INTDIR)\MsgCreator.obj"	"$(INTDIR)\MsgCreator.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\..\src\xalanc\Utils\MsgCreator\MsgFileOutputStream.cpp

!IF  "$(CFG)" == "MsgCreator - Win32 Release" || "$(CFG)" == "MsgCreator - Win32 Release.ICU" || "$(CFG)" == "MsgCreator - Win64 Release"


"$(INTDIR)\MsgFileOutputStream.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Debug" || "$(CFG)" == "MsgCreator - Win32 Debug.ICU" || "$(CFG)" == "MsgCreator - Win64 Debug"


"$(INTDIR)\MsgFileOutputStream.obj"	"$(INTDIR)\MsgFileOutputStream.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\..\src\xalanc\Utils\MsgCreator\NLSHandler.cpp

!IF  "$(CFG)" == "MsgCreator - Win32 Release" || "$(CFG)" == "MsgCreator - Win32 Release.ICU" || "$(CFG)" == "MsgCreator - Win64 Release"


"$(INTDIR)\NLSHandler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Debug" || "$(CFG)" == "MsgCreator - Win32 Debug.ICU" || "$(CFG)" == "MsgCreator - Win64 Debug"


"$(INTDIR)\NLSHandler.obj"	"$(INTDIR)\NLSHandler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 

SOURCE=..\..\..\..\..\src\xalanc\Utils\MsgCreator\SAX2Handler.cpp

!IF  "$(CFG)" == "MsgCreator - Win32 Release" || "$(CFG)" == "MsgCreator - Win32 Release.ICU" || "$(CFG)" == "MsgCreator - Win64 Release"


"$(INTDIR)\SAX2Handler.obj" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ELSEIF  "$(CFG)" == "MsgCreator - Win32 Debug" || "$(CFG)" == "MsgCreator - Win32 Debug.ICU" || "$(CFG)" == "MsgCreator - Win64 Debug"


"$(INTDIR)\SAX2Handler.obj"	"$(INTDIR)\SAX2Handler.sbr" : $(SOURCE) "$(INTDIR)"
	$(CPP) $(CPP_PROJ) $(SOURCE)


!ENDIF 




