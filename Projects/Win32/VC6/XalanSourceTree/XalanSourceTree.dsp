# Microsoft Developer Studio Project File - Name="XalanSourceTree" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XalanSourceTree - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XalanSourceTree.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XalanSourceTree.mak" CFG="XalanSourceTree - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XalanSourceTree - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanSourceTree - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanSourceTree - Win32 Release with symbols" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanSourceTree - Win64 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XalanSourceTree - Win64 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XalanSourceTree - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release\XalanSourceTree"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "WIN32" /D "_MBCS" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release/XalanSourceTree_1_5_0.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release/XalanSourceTree_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release"

!ELSEIF  "$(CFG)" == "XalanSourceTree - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Debug\XalanSourceTree"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Debug/XalanSourceTree_1_5_0D.dll" /implib:"..\..\..\..\Build\Win32\VC6\Debug/XalanSourceTree_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XalanSourceTree - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XalanSourceTree___Win32_Release_with_symbols"
# PROP BASE Intermediate_Dir "XalanSourceTree___Win32_Release_with_symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols\XalanSourceTree"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "WIN32" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "WIN32" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\..\Build\Win32\Vc6\Release\XMLSupport.lib ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib /nologo /dll /pdb:none /machine:I386
# SUBTRACT BASE LINK32 /debug
# ADD LINK32 xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release.symbols/XalanSourceTree_1_5_0S.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release.symbols/XalanSourceTree_1S.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XalanSourceTree - Win64 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Release\XalanSourceTree"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /D "WIN64" /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "WIN32" /D "_MBCS" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:IA64 /out:"..\..\..\..\Build\Win64\VC6\Release/XalanSourceTree_1_5_0.dll" /implib:"..\..\..\..\Build\Win64\VC6\Release/XalanSourceTree_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Release"

!ELSEIF  "$(CFG)" == "XalanSourceTree - Win64 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Debug\XalanSourceTree"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /D "WIN64" /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:IA64 /out:"..\..\..\..\Build\Win64\VC6\Debug/XalanSourceTree_1_5_0D.dll" /implib:"..\..\..\..\Build\Win64\VC6\Debug/XalanSourceTree_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XalanSourceTree - Win32 Release"
# Name "XalanSourceTree - Win32 Debug"
# Name "XalanSourceTree - Win32 Release with symbols"
# Name "XalanSourceTree - Win64 Release"
# Name "XalanSourceTree - Win64 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\FormatterToSourceTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttributeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttributeNSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttrNS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeCommentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeContentHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDocumentFragment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementAAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementANS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementANSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNAAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNANS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNANSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeProcessingInstructionAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextIWS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextIWSAllocator.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\FormatterToSourceTree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttributeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttributeNSAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttrNS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeCommentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeContentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementAAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementANS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementANSAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNAAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNANS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNANSAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeProcessingInstructionAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextIWS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextIWSAllocator.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Res\XalanSourceTree\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\Res\XalanSourceTree\XalanSourceTree.rc
# End Source File
# End Group
# End Target
# End Project
