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
!MESSAGE "XercesParserLiaison - Win64 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XercesParserLiaison - Win64 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!ELSEIF  "$(CFG)" == "XercesParserLiaison - Win64 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Release\XercesParserLiaison"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /c
# ADD CPP /D "WIN64" /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:IA64 /out:"..\..\..\..\Build\Win64\VC6\Release/XercesParserLiaison_1_5_0.dll" /implib:"..\..\..\..\Build\Win64\VC6\Release/XercesParserLiaison_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Release"

!ELSEIF  "$(CFG)" == "XercesParserLiaison - Win64 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Debug\XercesParserLiaison"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /YX /FD /GZ /c
# ADD CPP /D "WIN64" /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_DEBUG" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:IA64 /out:"..\..\..\..\Build\Win64\VC6\Debug/XercesParserLiaison_1_5_0D.dll" /implib:"..\..\..\..\Build\Win64\VC6\Debug/XercesParserLiaison_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XercesParserLiaison - Win32 Release"
# Name "XercesParserLiaison - Win32 Debug"
# Name "XercesParserLiaison - Win32 Release with symbols"
# Name "XercesParserLiaison - Win64 Release"
# Name "XercesParserLiaison - Win64 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\FormatterToDeprecatedXercesDOM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\FormatterToXercesDOM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttributeBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesBridgeHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesBridgeNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCDATASectionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCDATASectionWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCommentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCommentWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentFragmentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentTypeBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentTypeWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOM_NodeHack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMFormatterWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMImplementationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMImplementationWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMWrapperException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityReferenceBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityReferenceWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesLiaisonXalanDOMStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapAttributeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNodeListBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNodeListWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNotationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNotationWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesProcessingInstructionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesProcessingInstructionWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesToXalanNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperToXalanNodeMap.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\FormatterToDeprecatedXercesDOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\FormatterToXercesDOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttributeBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesBridgeHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesBridgeNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesBridgeTypes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCDATASectionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCDATASectionWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCommentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCommentWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentFragmentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentTypeBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentTypeWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOM_NodeHack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMFormatterWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMImplementationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMImplementationWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMWrapperException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityReferenceBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityReferenceWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesLiaisonXalanDOMStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNodeListBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNodeListWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNotationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNotationWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesParserLiaisonDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesProcessingInstructionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesProcessingInstructionWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesToXalanNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperToXalanNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperTypes.hpp
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
