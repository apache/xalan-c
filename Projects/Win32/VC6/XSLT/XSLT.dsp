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
!MESSAGE "XSLT - Win64 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "XSLT - Win64 Debug" (based on "Win32 (x86) Dynamic-Link Library")
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

!ELSEIF  "$(CFG)" == "XSLT - Win64 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Release\XSLT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /c
# ADD CPP /D "WIN64" /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /FD /c
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
# ADD LINK32 xerces-c_2.lib /nologo /dll /pdb:none /machine:IA64 /out:"..\..\..\..\Build\Win64\VC6\Release/XSLT_1_5_0.dll" /implib:"..\..\..\..\Build\Win64\VC6\Release/XSLT_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Release"

!ELSEIF  "$(CFG)" == "XSLT - Win64 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Debug\XSLT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /D "WIN64" /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "_WINDOWS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /D "WIN32" /D "_DEBUG" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib /nologo /dll /debug /machine:IA64 /out:"..\..\..\..\Build\Win64\VC6\Debug/XSLT_1_5_0D.dll" /implib:"..\..\..\..\Build\Win64\VC6\Debug/XSLT_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Debug"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "XSLT - Win32 Release"
# Name "XSLT - Win32 Debug"
# Name "XSLT - Win32 Release with symbols"
# Name "XSLT - Win64 Release"
# Name "XSLT - Win64 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPart.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPartSimple.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPartXPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\Constants.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\CountersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemApplyImport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemApplyTemplates.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemAttribute.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemAttributeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCallTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemChoose.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCopy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCopyOf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemDecimalFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemEmpty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemExtensionCall.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemFallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemForEach.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemIf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemLiteralResult.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemOtherwise.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemSort.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTemplateElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTextLiteral.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemUse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemValueOf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemVariable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemWhen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemWithParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ExtensionFunctionHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ExtensionNSHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionCurrent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionElementAvailable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionFormatNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionFunctionAvailable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionGenerateID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionSystemProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionUnparsedEntityURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\GenerateEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\KeyTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NamespacesHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NodeSorter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NodeSortKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\OutputContextStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ProblemListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ProblemListenerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultNamespacesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultTreeFragAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultTreeFragBaseNodeRefListBaseProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\SelectionEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\Stylesheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetConstructionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetConstructionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetExecutionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetRoot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TopLevelArg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TraceListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TraceListenerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TracerEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\VariablesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTPartSimpleAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTPartXPathAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemApplyTemplatesAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemAttributeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemAttributeSetAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemCallTemplateAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemElementAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemEmptyAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemLiteralResultAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTemplateAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTextAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTextLiteralAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemValueOfAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemVariableAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanNumberingResourceBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSourceTreeDocumentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSourceTreeDocumentFragmentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XResultTreeFragAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTEngineImpl.cpp

!IF  "$(CFG)" == "XSLT - Win32 Release"

!ELSEIF  "$(CFG)" == "XSLT - Win32 Debug"

# ADD CPP /YX

!ELSEIF  "$(CFG)" == "XSLT - Win32 Release with symbols"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorEnvSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorEnvSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTResultTarget.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVT.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPart.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPartSimple.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPartXPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\Constants.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\CountersTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\DecimalToRoman.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemApplyImport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemApplyTemplates.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemAttribute.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemAttributeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCallTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemChoose.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCopy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCopyOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemDecimalFormat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemEmpty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemExtensionCall.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemFallback.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemForEach.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemIf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemLiteralResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemOtherwise.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemPI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTemplateElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTextLiteral.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemUse.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemValueOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemVariable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemWhen.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemWithParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ExtensionFunctionHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionCurrent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionElementAvailable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionFormatNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionFunctionAvailable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionGenerateID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionSystemProperty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionUnparsedEntityURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\GenerateEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\KeyDeclaration.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\KeyTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NamespacesHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NodeSorter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NodeSortKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\OutputContextStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ProblemListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ProblemListenerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultNamespacesStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultTreeFragAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultTreeFragBaseNodeRefListBaseProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\SelectionEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\Stylesheet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetConstructionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetConstructionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetExecutionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetRoot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TopLevelArg.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TraceListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TraceListenerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TracerEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\VariablesStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTPartSimpleAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTPartXPathAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemApplyTemplatesAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemAttributeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemAttributeSetAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemCallTemplateAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemElementAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemEmptyAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemLiteralResultAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTemplateAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTextAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTextLiteralAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemValueOfAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemVariableAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanNumberingResourceBundle.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSourceTreeDocumentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSourceTreeDocumentFragmentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XResultTreeFragAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTEngineImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorEnvSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorEnvSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTResultTarget.hpp
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
