# Microsoft Developer Studio Project File - Name="Harness" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Harness - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Harness.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Harness.mak" CFG="Harness - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Harness - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Harness - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Harness - Win32 Release with symbols" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Harness - Win64 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Harness - Win64 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Harness - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\Build\Win32\VC6\Release\Harness"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:I386 /out:"..\..\Build\Win32\VC6\Release/Harness_1_8.dll" /implib:"..\..\Build\Win32\VC6\Release/Harness_1.lib" /libpath:"..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\lib"

!ELSEIF  "$(CFG)" == "Harness - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\Build\Win32\VC6\Debug\Harness"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /Gf /Gy /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /Yc /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"..\..\Build\Win32\VC6\Debug/Harness_1_8D.dll" /implib:"..\..\Build\Win32\VC6\Debug/Harness_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug" /libpath:"$(XERCESCROOT)\Build\Win32\VC6\Debug" /libpath:"$(XERCESCROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Harness - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Harness___Win32_Release_with_symbols"
# PROP BASE Intermediate_Dir "Harness___Win32_Release_with_symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\Build\Win32\VC6\Release.symbols\Harness"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I "..\harness\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib ..\..\Build\Win32\VC6\Release\PlatformSupport.lib ..\..\Build\Win32\VC6\Release\XalanDOM.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /incremental:yes /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"..\..\Build\Win32\VC6\Release.symbols/Harness_1_8S.dll" /implib:"..\..\Build\Win32\VC6\Release.symbols/Harness_1S.lib" /libpath:"..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Harness - Win64 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\Win64\VC6\Release"
# PROP Intermediate_Dir "..\..\Build\Win64\VC6\Release\Harness"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "WIN64" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:IX86 /out:"..\..\Build\Win64\VC6\Release/Harness_1_8.dll" /implib:"..\..\Build\Win64\VC6\Release/Harness_1.lib" /libpath:"..\..\..\..\xml-xerces\c\Build\Win64\VC6\Release" /libpath:"$(XERCESCROOT)\Build\Win64\VC6\Release" /libpath:"$(XERCESCROOT)\lib" /machine:IA64

!ELSEIF  "$(CFG)" == "Harness - Win64 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Build\Win64\VC6\Debug"
# PROP Intermediate_Dir "..\..\Build\Win64\VC6\Debug\Harness"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /Gf /Gy /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "WIN64" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "HARNESS_EXPORTS" /Yc /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:IX86 /out:"..\..\Build\Win64\VC6\Debug/Harness_1_8D.dll" /implib:"..\..\Build\Win64\VC6\Debug/Harness_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\xml-xerces\c\Build\Win64\VC6\Debug" /libpath:"$(XERCESCROOT)\Build\Win64\VC6\Debug" /libpath:"$(XERCESCROOT)\lib" /machine:IA64
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Harness - Win32 Release"
# Name "Harness - Win32 Debug"
# Name "Harness - Win32 Release with symbols"
# Name "Harness - Win64 Release"
# Name "Harness - Win64 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\FileUtility.cpp

!IF  "$(CFG)" == "Harness - Win32 Release"

!ELSEIF  "$(CFG)" == "Harness - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Harness - Win32 Release with symbols"

!ELSEIF  "$(CFG)" == "Harness - Win64 Release"

!ELSEIF  "$(CFG)" == "Harness - Win64 Debug"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\HarnessInit.cpp
# End Source File
# Begin Source File

SOURCE=.\XMLFileReporter.cpp

!IF  "$(CFG)" == "Harness - Win32 Release"

!ELSEIF  "$(CFG)" == "Harness - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "Harness - Win32 Release with symbols"

!ELSEIF  "$(CFG)" == "Harness - Win64 Release"

!ELSEIF  "$(CFG)" == "Harness - Win64 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\FileUtility.hpp
# End Source File
# Begin Source File

SOURCE=.\HarnessDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=.\HarnessInit.hpp
# End Source File
# Begin Source File

SOURCE=.\XMLFileReporter.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Harness.rc
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# End Target
# End Project
