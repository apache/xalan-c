# Microsoft Developer Studio Project File - Name="XercesParserLiaison" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XercesParserLiaison - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XercesParserLiaison.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XercesParserLiaison.mak" CFG="XercesParserLiaison - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XercesParserLiaison - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XercesParserLiaison - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XercesParserLiaison - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release\XercesParserLiaison"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /FD /c
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
# ADD LINK32 ..\..\..\..\Build\Win32\Vc6\Release\XMLSupport.lib ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib /nologo /dll /pdb:none /machine:I386
# SUBTRACT LINK32 /debug

!ELSEIF  "$(CFG)" == "XercesParserLiaison - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Debug\XercesParserLiaison"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "XML_DEBUG" /YX /FD /GZ /c
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

!ENDIF 

# Begin Target

# Name "XercesParserLiaison - Win32 Release"
# Name "XercesParserLiaison - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttributeBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentFragmentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentTypeBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOM_NodeHack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesToXalanNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTreeWalker.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttributeBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeCommon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentFragmentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentTypeBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOM_NodeHack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesParserLiaisonDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesToXalanNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTreeWalker.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
