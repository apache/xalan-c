# Microsoft Developer Studio Project File - Name="XalanDOM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XalanDOM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XalanDOM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XalanDOM.mak" CFG="XalanDOM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XalanDOM - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanDOM - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanDOM - Win32 Release with symbols" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XalanDOM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release\XalanDOM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANDOM_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_DOM_BUILD_DLL" /FD /c
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
# ADD LINK32 ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /pdb:none /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "XalanDOM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Debug\XalanDOM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALANDOM_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_DOM_BUILD_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug\xerces-c_1D.lib /nologo /dll /debug /machine:I386 /pdbtype:sept

!ELSEIF  "$(CFG)" == "XalanDOM - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XalanDOM___Win32_Release_with_symbols"
# PROP BASE Intermediate_Dir "XalanDOM___Win32_Release_with_symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols\XalanDOM"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GX /Zi /O2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_DOM_BUILD_DLL" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_DOM_BUILD_DLL" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386
# ADD LINK32 ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib /nologo /dll /debug /machine:I386

!ENDIF 

# Begin Target

# Name "XalanDOM - Win32 Release"
# Name "XalanDOM - Win32 Debug"
# Name "XalanDOM - Win32 Release with symbols"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanAttr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanCDataSection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanCharacterData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocumentFragment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocumentType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMImplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEmptyNamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEntityReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeListDummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeListSurrogate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNotation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanText.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanCDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEmptyNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEntity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeListDummy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeListSurrogate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNotation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanText.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
