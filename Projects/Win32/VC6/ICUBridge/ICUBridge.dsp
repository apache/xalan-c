# Microsoft Developer Studio Project File - Name="ICUBridge" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ICUBridge - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ICUBridge.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ICUBridge.mak" CFG="ICUBridge - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ICUBridge - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ICUBridge - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ICUBridge - Win32 Release with symbols" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ICUBridge - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release\ICUBridge"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ICUBRIDGE_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_ICUBRIDGE_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 icuuc.lib icuin.lib xerces-c_2.lib /nologo /dll /pdb:none /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release/ICUBridge_1_5_0.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release/ICUBridge_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"..\..\..\..\..\..\icu\lib"

!ELSEIF  "$(CFG)" == "ICUBridge - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Debug\ICUBridge"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "ICUBRIDGE_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_ICUBRIDGE_BUILD_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 icuucd.lib icuind.lib xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Debug/ICUBridge_1_5_0D.dll" /implib:"..\..\..\..\Build\Win32\VC6\Debug/ICUBridge_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug" /libpath:"..\..\..\..\..\..\icu\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "ICUBridge - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ICUBridge___Win32_Release_with_symbols"
# PROP BASE Intermediate_Dir "ICUBridge___Win32_Release_with_symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols\ICUBridge"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_ICUBRIDGE_BUILD_DLL" /D "XALAN_XERCES" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_ICUBRIDGE_BUILD_DLL" /D "XALAN_XERCES" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 icuuc.lib icuin.lib xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release.symbols/ICUBridge_1_5_0S.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release.symbols/ICUBridge_1S.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"..\..\..\..\..\..\icu\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "ICUBridge - Win32 Release"
# Name "ICUBridge - Win32 Debug"
# Name "ICUBridge - Win32 Release with symbols"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\FunctionICUFormatNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCleanup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCollationCompareFunctor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCollationCompareFunctorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUXalanNumberFormatFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUXalanNumberFormatProxy.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\FunctionICUFormatNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCleanup.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCollationCompareFunctor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCollationCompareFunctorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUXalanNumberFormatFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUXalanNumberFormatProxy.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Res\ICUBridge\ICUBridge.rc
# End Source File
# Begin Source File

SOURCE=..\..\Res\ICUBridge\resource.h
# End Source File
# End Group
# End Target
# End Project
