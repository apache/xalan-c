# Microsoft Developer Studio Project File - Name="XPath" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XPath - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XPath.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XPath.mak" CFG="XPath - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XPath - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XPath - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XPath - Win32 Release with symbols" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XPath - Win64 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XPath - Win64 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XPath - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release\XPath"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XPATH_BUILD_DLL" /D "WIN32" /D "_MBCS" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release/XPath_1_5_0.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release/XPath_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release"

!ELSEIF  "$(CFG)" == "XPath - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Debug\XPath"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XPATH_BUILD_DLL" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Debug/XPath_1_5_0D.dll" /implib:"..\..\..\..\Build\Win32\VC6\Debug/XPath_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XPath - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XPath___Win32_Release_with_symbols"
# PROP BASE Intermediate_Dir "XPath___Win32_Release_with_symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols\XPath"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XPATH_BUILD_DLL" /D "XALAN_XERCES" /D "WIN32" /D "_MBCS" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XPATH_BUILD_DLL" /D "XALAN_XERCES" /D "WIN32" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib /nologo /dll /debug /machine:I386
# ADD LINK32 xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release.symbols/XPath_1_5_0S.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release.symbols/XPath_1S.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XPath - Win64 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Release\XPath"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /c
# ADD CPP /D "WIN64" /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XPATH_BUILD_DLL" /D "WIN32" /D "_MBCS" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:IA64 /out:"..\..\..\..\Build\Win64\VC6\Release/XPath_1_5_0.dll" /implib:"..\..\..\..\Build\Win64\VC6\Release/XPath_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Release"

!ELSEIF  "$(CFG)" == "XPath - Win64 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Debug\XPath"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /D "WIN64" /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XPATH_BUILD_DLL" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:IA64 /out:"..\..\..\..\Build\Win64\VC6\Debug/XPath_1_5_0D.dll" /implib:"..\..\..\..\Build\Win64\VC6\Debug/XPath_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XPath - Win32 Release"
# Name "XPath - Win32 Debug"
# Name "XPath - Win32 Release with symbols"
# Name "XPath - Win64 Release"
# Name "XPath - Win64 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\ElementPrefixResolverProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FormatterStringLengthCounter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FoundIndex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\Function.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionConcat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionContains.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionLang.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionNamespaceURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionNormalizeSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionStartsWith.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstring.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstringAfter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstringBefore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionTranslate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\MutableNodeRefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NodeRefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NodeRefListBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\ResultTreeFragBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByValue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByValueAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanXPathException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XBoolean.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetNodeProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetNodeProxyAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetResultTreeFragProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNull.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumberAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumberBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectFactoryDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxyBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxyText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectTypeCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathConstructionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathConstructionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEnvSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEnvSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEvaluator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExecutionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExpression.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactoryBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactoryDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFunctionTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathParserException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathProcessorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XSpan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringCached.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringCachedAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringReferenceAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenNumberAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenNumberAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenStringAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenStringAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XUnknown.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\ElementPrefixResolverProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FormatterStringLengthCounter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FoundIndex.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\Function.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionConcat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionContains.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionLang.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionNamespaceURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionNormalizeSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionStartsWith.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstring.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstringAfter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstringBefore.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionTranslate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\MutableNodeRefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NameSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NodeRefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NodeRefListBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\ResultTreeFragBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByValue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByValueAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanXPathException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XBoolean.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetNodeProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetNodeProxyAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetResultTreeFragProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNull.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumberAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumberBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectFactoryDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxyBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxyText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectTypeCallback.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathConstructionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathConstructionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEnvSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEnvSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEvaluator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExecutionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExpression.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactoryBlock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactoryDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFunctionTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathParserException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathProcessorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XSpan.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringCached.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringCachedAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringReferenceAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenNumberAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenNumberAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenStringAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenStringAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XUnknown.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Res\XPath\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\Res\XPath\XPath.rc
# End Source File
# End Group
# End Target
# End Project
