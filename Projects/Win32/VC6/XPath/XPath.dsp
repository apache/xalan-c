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
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XPATH_BUILD_DLL" /D "XALAN_XERCES" /D "WIN32" /D "_MBCS" /FD /c
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
# ADD LINK32 ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib /nologo /dll /debug /machine:I386

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
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_DEBUG" /D "XML_DEBUG" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XPATH_BUILD_DLL" /D "XALAN_XERCES" /D "WIN32" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug\xerces-c_1D.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XPath - Win32 Release"
# Name "XPath - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ElementPrefixResolverProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FoundIndex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\Function.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionBoolean.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionCeiling.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionConcat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionContains.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionCount.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionFalse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionFloor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLang.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLast.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLocalName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNamespaceURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNormalizeSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionPosition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionRound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionStartsWith.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionStringLength.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstring.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstringAfter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstringBefore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionTranslate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionTrue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\MutableNodeRefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeRefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeRefListBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\PrefixResolver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\QName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ResultTreeFragBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\SimpleNodeLocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XBoolean.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XLocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSetAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNull.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumberAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectFactoryDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectTypeCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEnvSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEnvSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExecutionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExpression.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactoryDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFunctionTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathParserException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathProcessorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPointer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XResultTreeFragAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XSpan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XUnknown.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ElementPrefixResolverProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FoundIndex.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\Function.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionBoolean.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionCeiling.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionConcat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionContains.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionCount.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionDefaultStringArgument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionDoc.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionFalse.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionFloor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLang.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLast.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLocalName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNamespaceURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNormalizeSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionPosition.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionRound.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionStartsWith.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionStringLength.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstring.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstringAfter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstringBefore.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSum.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionTranslate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionTrue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\MutableNodeRefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NameSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeRefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeRefListBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\PrefixResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\QName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ResultTreeFragBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\SimpleNodeLocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XBoolean.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XLocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSetAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNull.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumberAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectFactoryDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectTypeCallback.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEnvSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEnvSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExecutionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExpression.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactoryDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFunctionTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathParserException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathProcessorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPointer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XResultTreeFragAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XSpan.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XUnknown.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
