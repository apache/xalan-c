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
!MESSAGE "XercesParserLiaison - Win32 Release with symbols" (based on "Win32 (x86) Dynamic-Link Library")
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release/XercesParserLiaison_1_5_0.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release/XercesParserLiaison_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release"

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
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Debug/XercesParserLiaison_1_5_0D.dll" /implib:"..\..\..\..\Build\Win32\VC6\Debug/XercesParserLiaison_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XercesParserLiaison - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XercesParserLiaison___Win32_Release_with_symbols"
# PROP BASE Intermediate_Dir "XercesParserLiaison___Win32_Release_with_symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols\XercesParserLiaison"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release.symbols/XercesParserLiaison_1_5_0S.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release.symbols/XercesParserLiaison_1S.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XercesParserLiaison - Win32 Release"
# Name "XercesParserLiaison - Win32 Debug"
# Name "XercesParserLiaison - Win32 Release with symbols"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\FormatterToDeprecatedXercesDOM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\FormatterToXercesDOM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttributeBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentWrapper.cpp
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

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentTypeWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOM_NodeHack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMFormatterWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMWrapperException.cpp
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

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesLiaisonXalanDOMStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapAttributeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesToXalanNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperToXalanNodeMap.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\FormatterToDeprecatedXercesDOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\FormatterToXercesDOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttributeBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeTypes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentWrapper.hpp
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

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentTypeWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOM_NodeHack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMFormatterWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMWrapperException.hpp
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

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesLiaisonXalanDOMStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationWrapper.hpp
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

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesToXalanNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperToXalanNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperTypes.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Res\XercesParserLiaison\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\Res\XercesParserLiaison\XercesParserLiaison.rc
# End Source File
# End Group
# End Target
# End Project
