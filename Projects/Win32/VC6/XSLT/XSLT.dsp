# Microsoft Developer Studio Project File - Name="XSLT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=XSLT - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "XSLT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "XSLT.mak" CFG="XSLT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "XSLT - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XSLT - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XSLT - Win32 Release with symbols" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "XSLT - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release\XSLT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release/XSLT_1_5_0.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release/XSLT_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release"

!ELSEIF  "$(CFG)" == "XSLT - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Debug\XSLT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Debug/XSLT_1_5_0D.dll" /implib:"..\..\..\..\Build\Win32\VC6\Debug/XSLT_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "XSLT - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "XSLT___Win32_Release_with_symbols"
# PROP BASE Intermediate_Dir "XSLT___Win32_Release_with_symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols\XSLT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /D "XALAN_XERCES" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /D "XALAN_XERCES" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release.symbols/XSLT_1_5_0S.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release.symbols/XSLT_1S.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XSLT - Win32 Release"
# Name "XSLT - Win32 Debug"
# Name "XSLT - Win32 Release with symbols"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPart.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPartSimple.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPartXPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\Constants.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\CountersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemApplyImport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemApplyTemplates.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemAttribute.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemAttributeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCallTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemChoose.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCopy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCopyOf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemDecimalFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemEmpty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemExtensionCall.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemFallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemForEach.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemIf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemLiteralResult.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemOtherwise.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemSort.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTemplateElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTextLiteral.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemUse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemValueOf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemVariable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemWhen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemWithParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ExtensionFunctionHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ExtensionNSHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionCurrent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionElementAvailable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionFormatNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionFunctionAvailable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionGenerateID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionSystemProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionUnparsedEntityURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\GenerateEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\KeyTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NamespacesHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NodeSorter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NodeSortKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\OutputContextStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ProblemListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ProblemListenerDefault.cpp
# End Source File
# Begin Source File

SOURCE="C:\Program Files\Rational\Quantify\pure_api.c"
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultNamespacesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFragAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFragBaseNodeRefListBaseProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\SelectionEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\Stylesheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetConstructionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetConstructionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetExecutionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetRoot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TopLevelArg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TraceListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TraceListenerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TracerEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\VariablesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanAVTAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanAVTPartSimpleAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanAVTPartXPathAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemApplyTemplatesAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemAttributeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemAttributeSetAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemCallTemplateAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemElementAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemEmptyAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemLiteralResultAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemTemplateAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemTextAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemTextLiteralAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemValueOfAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemVariableAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanNumberingResourceBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanSourceTreeDocumentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanSourceTreeDocumentFragmentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XResultTreeFragAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTEngineImpl.cpp

!IF  "$(CFG)" == "XSLT - Win32 Release"

!ELSEIF  "$(CFG)" == "XSLT - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "XSLT - Win32 Release with symbols"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorEnvSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorEnvSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTResultTarget.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVT.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPart.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPartSimple.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPartXPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\Constants.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\CountersTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\DecimalToRoman.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemApplyImport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemApplyTemplates.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemAttribute.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemAttributeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCallTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemChoose.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCopy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCopyOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemDecimalFormat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemEmpty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElementFrameMarker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemExtensionCall.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemFallback.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemForEach.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemIf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemLiteralResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemOtherwise.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemPI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemPriv.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTemplateElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTextLiteral.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemUse.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemValueOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemVariable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemWhen.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemWithParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ExtensionFunctionHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionCurrent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionElementAvailable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionFormatNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionFunctionAvailable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionGenerateID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionSystemProperty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionUnparsedEntityURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\GenerateEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\KeyDeclaration.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\KeyTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NamespacesHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NodeSorter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NodeSortKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\OutputContextStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ProblemListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ProblemListenerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultNamespacesStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFragAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFragBaseNodeRefListBaseProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\SelectionEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\Stylesheet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetConstructionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetConstructionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetExecutionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetRoot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TopLevelArg.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TraceListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TraceListenerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TracerEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\VariablesStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanAVTAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanAVTPartSimpleAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanAVTPartXPathAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemApplyTemplatesAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemAttributeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemAttributeSetAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemCallTemplateAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemElementAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemEmptyAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemLiteralResultAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemTemplateAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemTextAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemTextLiteralAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemValueOfAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanElemVariableAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanNumberingResourceBundle.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanSourceTreeDocumentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanSourceTreeDocumentFragmentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XResultTreeFragAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTEngineImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorEnvSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorEnvSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTResultTarget.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Res\XSLT\resource.h
# End Source File
# Begin Source File

SOURCE=..\..\Res\XSLT\XSLT.rc
# End Source File
# End Group
# End Target
# End Project
