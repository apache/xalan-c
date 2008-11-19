# Microsoft Developer Studio Project File - Name="AllInOneWithICU" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=AllInOneWithICU - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "AllInOneWithICU.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "AllInOneWithICU.mak" CFG="AllInOneWithICU - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "AllInOneWithICU - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AllInOneWithICU - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AllInOneWithICU - Win64 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "AllInOneWithICU - Win64 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "AllInOneWithICU - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release\AllInOne"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\Build\Win32\VC6\Release\Nls\Include" /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /I "$(ICUROOT)\include" /D "NDEBUG" /D "XALAN_BUILD_DLL" /D "_MBCS" /D "XALAN_USE_ICU" /D "XALAN_ICU_MSG_LOADER" /FD /Zm250 /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 xerces-c_3.lib XalanMessages_1_11.lib icuuc.lib icuin.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release/Xalan-C_1_11.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release/Xalan-C_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"..\..\..\..\..\..\icu\lib" /libpath:"..\..\..\..\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\lib" /libpath:"$(ICUROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AllInOneWithICU - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Debug\AllInOne"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\Build\Win32\VC6\Debug\Nls\Include" /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /I "$(ICUROOT)\include" /D "XALAN_BUILD_DLL" /D "_MBCS" /D "XALAN_USE_ICU" /D "XALAN_ICU_MSG_LOADER" /YX /FD /GZ /Zm250 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_3D.lib XalanMessages_1_11D.lib icuucd.lib icuind.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Debug/Xalan-C_1_11D.dll" /implib:"..\..\..\..\Build\Win32\VC6\Debug/Xalan-C_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug" /libpath:"..\..\..\..\..\..\icu\lib" /libpath:"..\..\..\..\Build\Win32\VC6\Debug" /libpath:"$(XERCESCROOT)\Build\Win32\VC6\Debug" /libpath:"$(XERCESCROOT)\lib" /libpath:"$(ICUROOT)\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AllInOneWithICU - Win64 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Release"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Release\AllInOne"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "NDEBUG" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\Build\Win64\VC6\Release\Nls\Include" /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /I "$(ICUROOT)\include" /D "NDEBUG" /D "XALAN_BUILD_DLL" /D "_MBCS" /D "XALAN_USE_ICU" /D "XALAN_ICU_MSG_LOADER" /D "ITERATIVE_EXECUTION" /FD /c
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
# ADD LINK32 xerces-c_3.lib XalanMessages_1_11.lib icuuc.lib icuin.lib /nologo /dll /pdb:none /machine:IX86 /nodefaultlib:"libmmd.lib" /out:"..\..\..\..\Build\Win64\VC6\Release/Xalan-C_1_11.dll" /implib:"..\..\..\..\Build\Win64\VC6\Release/Xalan-C_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Release" /libpath:"..\..\..\..\..\..\icu\lib" /libpath:"$(XERCESCROOT)\Build\Win64\VC6\Release" /libpath:"$(XERCESCROOT)\lib" /libpath:"$(ICUROOT)\lib" /machine:IA64

!ELSEIF  "$(CFG)" == "AllInOneWithICU - Win64 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\Build\Win64\VC6\Debug"
# PROP Intermediate_Dir "..\..\..\..\Build\Win64\VC6\Debug\AllInOne"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\Build\Win64\VC6\Debug\Nls\Include" /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /I "$(ICUROOT)\include" /D "XALAN_BUILD_DLL" /D "_MBCS" /D "XALAN_USE_ICU" /D "XALAN_ICU_MSG_LOADER" /D "ITERATIVE_EXECUTION" /YX /FD /GZ /Zm125 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_3D.lib XalanMessages_1_11D.lib icuucd.lib icuind.lib /nologo /dll /debug /machine:IX86 /nodefaultlib:"libmmdd.lib" /out:"..\..\..\..\Build\Win64\VC6\Debug/Xalan-C_1_11D.dll" /implib:"..\..\..\..\Build\Win64\VC6\Debug/Xalan-C_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win64\VC6\Debug" /libpath:"..\..\..\..\..\..\icu\lib" /libpath:"$(XERCESCROOT)\Build\Win64\VC6\Debug" /libpath:"$(XERCESCROOT)\lib" /libpath:"$(ICUROOT)\lib" /machine:IA64
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "AllInOneWithICU - Win32 Release"
# Name "AllInOneWithICU - Win32 Debug"
# Name "AllInOneWithICU - Win64 Release"
# Name "AllInOneWithICU - Win64 Debug"
# Begin Group "Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\PlatformDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\STLHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\VCPPDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanAutoPtr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanDeque.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanMemMgrAutoPtr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanMemoryManagement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanObjectCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanObjectStackCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Include\XalanVersion.hpp
# End Source File
# End Group
# Begin Group "XalanDOM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanAttr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanCDataSection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanCDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanCharacterData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDocumentFragment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDocumentType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMImplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanDOMString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanEmptyNamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanEmptyNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanEntity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanEntityReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanEntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNodeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNodeListDummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNodeListDummy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNotation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanNotation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanDOM\XalanText.hpp
# End Source File
# End Group
# Begin Group "PlatformSupport"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ArenaAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ArenaBlock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ArenaBlockBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\AttributeListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\AttributeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\AttributesImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\AttributesImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\AttributeVectorEntry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\AttributeVectorEntryExtended.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\DirectoryEnumerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\DOMStringHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\DOMStringHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\DOMStringPrintWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\DOMStringPrintWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\DoubleSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\DoubleSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\FormatterListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\FormatterListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\NamedNodeMapAttributeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\NamedNodeMapAttributeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\NullPrintWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\NullPrintWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\PlatformSupportDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\PlatformSupportInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\PlatformSupportInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\PrefixResolver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\PrefixResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\PrintWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\PrintWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ProblemListenerBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ProblemListenerBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ReusableArenaAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\ReusableArenaBlock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\StdBinInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\StdBinInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\StringTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\StringTokenizer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\URISupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\URISupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\Writer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\Writer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanArrayAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanBitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanBitmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanCollationServices.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDecimalFormatSymbols.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDecimalFormatSymbols.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringHashTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringHashTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringReusableAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanDOMStringReusableAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanEncodingPropertyCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanEncodingPropertyCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanFileOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanFileOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanFStreamOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanFStreamOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanICUMessageLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanICUMessageLoader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanInMemoryMessageLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanInMemoryMessageLoader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanLocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanMemoryManagement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanMemoryManagerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanMemoryManagerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanMessageLoader.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanMessageLoader.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanNamespace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanNullOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanNullOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanNumberFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanNumberFormat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanOutputStreamPrintWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanOutputStreamPrintWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanParsedURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanParsedURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanReferenceCountedObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanReferenceCountedObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanSimplePrefixResolver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanSimplePrefixResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanStdOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanStdOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanToXercesTranscoderWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanToXercesTranscoderWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanTranscodingServices.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanTranscodingServices.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanUnicode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanUTF16Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanUTF16Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanXMLChar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XalanXMLChar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XSLException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\PlatformSupport\XSLException.hpp
# End Source File
# End Group
# Begin Group "DOMSupport"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMServices.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMServices.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupportDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupportException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupportException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupportInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\DOMSupportInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\TreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\XalanDocumentPrefixResolver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\XalanDocumentPrefixResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\XalanNamespacesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\DOMSupport\XalanNamespacesStack.hpp
# End Source File
# End Group
# Begin Group "XPath"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\ElementPrefixResolverProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\ElementPrefixResolverProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FormatterStringLengthCounter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FormatterStringLengthCounter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\Function.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\Function.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionConcat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionConcat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionContains.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionContains.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionLang.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionLang.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionNamespaceURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionNamespaceURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionNormalizeSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionNormalizeSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionStartsWith.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionStartsWith.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstring.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstring.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstringAfter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstringAfter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstringBefore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionSubstringBefore.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionTranslate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\FunctionTranslate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\MutableNodeRefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\MutableNodeRefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NameSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NodeRefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NodeRefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NodeRefListBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\NodeRefListBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanDocumentFragmentNodeRefListBaseProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanDocumentFragmentNodeRefListBaseProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByValue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByValue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByValueAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanQNameByValueAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanXPathException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XalanXPathException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XBoolean.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XBoolean.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetNodeProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetNodeProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetNodeProxyAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetNodeProxyAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetResultTreeFragProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNodeSetResultTreeFragProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumberAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumberAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumberBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XNumberBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectFactoryDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectFactoryDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxyBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxyBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxyText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectResultTreeFragProxyText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectTypeCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XObjectTypeCallback.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathConstructionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathConstructionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathConstructionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathConstructionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEnvSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEnvSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEnvSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEnvSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEvaluator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathEvaluator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExecutionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExecutionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExpression.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathExpression.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactoryBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactoryBlock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactoryDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFactoryDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFunctionTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathFunctionTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathParserException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathParserException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathProcessorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XPathProcessorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringCached.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringCached.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringCachedAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringCachedAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringReferenceAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XStringReferenceAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenNumberAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenNumberAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenNumberAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenNumberAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenStringAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenStringAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenStringAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XTokenStringAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XUnknown.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPath\XUnknown.hpp
# End Source File
# End Group
# Begin Group "XPathCAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPathCAPI\XPathCAPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XPathCAPI\XPathCAPI.h
# End Source File
# End Group
# Begin Group "XMLSupport"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToHTML.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToHTML.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToNull.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToNull.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToXML.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToXML.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterToXMLUnicode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterTreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\FormatterTreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanDummyIndentWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanFormatterWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanHTMLElementsProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanHTMLElementsProperties.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanIndentWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanOtherEncodingWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanUTF16Writer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanUTF16Writer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanUTF8Writer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanUTF8Writer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanXMLSerializerBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanXMLSerializerBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanXMLSerializerFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XalanXMLSerializerFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XMLParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XMLParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XMLSupportDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XMLSupportException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XMLSupportException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XMLSupportInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XMLSupport\XMLSupportInit.hpp
# End Source File
# End Group
# Begin Group "XalanSourceTree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\FormatterToSourceTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\FormatterToSourceTree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttributeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttributeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttributeNSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttributeNSAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttrNS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeAttrNS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeCommentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeCommentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeContentHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeContentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDocumentFragment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeDOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementAAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementAAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementANS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementANS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementANSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementANSAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNA.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNA.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNAAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNAAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNANS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNANS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNANSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeElementNANSAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeProcessingInstructionAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeProcessingInstructionAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextIWS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextIWS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextIWSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanSourceTree\XalanSourceTreeTextIWSAllocator.hpp
# End Source File
# End Group
# Begin Group "XSLT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVT.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPart.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPart.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPartSimple.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPartSimple.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPartXPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\AVTPartXPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\Constants.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\Constants.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\CountersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\CountersTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\DecimalToRoman.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemApplyImport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemApplyImport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemApplyTemplates.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemApplyTemplates.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemAttribute.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemAttribute.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemAttributeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemAttributeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCallTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCallTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemChoose.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemChoose.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCopy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCopy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCopyOf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemCopyOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemDecimalFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemDecimalFormat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemEmpty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemEmpty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemExtensionCall.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemExtensionCall.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemFallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemFallback.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemForEach.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemForEach.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemForwardCompatible.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemForwardCompatible.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemIf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemIf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemLiteralResult.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemLiteralResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemOtherwise.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemOtherwise.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemPI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemSort.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTemplateElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTemplateElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTextLiteral.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemTextLiteral.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemUse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemUse.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemValueOf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemValueOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemVariable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemVariable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemWhen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemWhen.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemWithParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ElemWithParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ExtensionFunctionHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ExtensionFunctionHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ExtensionNSHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ExtensionNSHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionCurrent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionCurrent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionElementAvailable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionElementAvailable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionFormatNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionFormatNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionFunctionAvailable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionFunctionAvailable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionGenerateID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionGenerateID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionSystemProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionSystemProperty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionUnparsedEntityURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\FunctionUnparsedEntityURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\GenerateEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\GenerateEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\KeyDeclaration.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\KeyTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\KeyTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NamespacesHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NamespacesHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NodeSorter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NodeSorter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NodeSortKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\NodeSortKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\OutputContextStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\OutputContextStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ProblemListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ProblemListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ProblemListenerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ProblemListenerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultNamespacesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\ResultNamespacesStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\SelectionEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\SelectionEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\Stylesheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\Stylesheet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetConstructionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetConstructionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetConstructionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetConstructionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetExecutionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetExecutionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetRoot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\StylesheetRoot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TopLevelArg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TopLevelArg.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TraceListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TraceListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TraceListenerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TraceListenerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TracerEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\TracerEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\VariablesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\VariablesStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTPartSimpleAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTPartSimpleAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTPartXPathAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanAVTPartXPathAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemApplyTemplatesAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemApplyTemplatesAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemAttributeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemAttributeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemAttributeSetAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemAttributeSetAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemCallTemplateAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemCallTemplateAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemElementAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemElementAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemEmptyAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemEmptyAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemLiteralResultAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemLiteralResultAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTemplateAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTemplateAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTextAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTextAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTextLiteralAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemTextLiteralAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemValueOfAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemValueOfAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemVariableAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanElemVariableAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanMatchPatternData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanMatchPatternData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanMatchPatternDataAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanMatchPatternDataAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanNumberingResourceBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanNumberingResourceBundle.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanParamHolder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSourceTreeDocumentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSourceTreeDocumentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSourceTreeDocumentFragmentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSourceTreeDocumentFragmentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSpaceNodeTester.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XalanSpaceNodeTester.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XResultTreeFragAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XResultTreeFragAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTEngineImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTEngineImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorEnvSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorEnvSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorEnvSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorEnvSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTProcessorException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTResultTarget.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XSLT\XSLTResultTarget.hpp
# End Source File
# End Group
# Begin Group "ICUBridge"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCleanup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCleanup.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCollationCompareFunctor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCollationCompareFunctor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCollationCompareFunctorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeCollationCompareFunctorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUBridgeDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUFormatNumberFunctor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUFormatNumberFunctor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUXalanNumberFormatFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUXalanNumberFormatFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUXalanNumberFormatProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\ICUBridge\ICUXalanNumberFormatProxy.hpp
# End Source File
# End Group
# Begin Group "XalanExtensions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionDifference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionDifference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionDistinct.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionDistinct.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionEvaluate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionEvaluate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionHasSameNodes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionHasSameNodes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionIntersection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionIntersection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionNodeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\FunctionNodeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\XalanExtensions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\XalanExtensions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanExtensions\XalanExtensionsDefinitions.hpp
# End Source File
# End Group
# Begin Group "XalanEXSLT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTCommon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTCommonImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTDateTime.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTDateTime.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTDateTimeImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTDynamic.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTDynamic.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTDynamicImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTMath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTMath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTMathImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTSetImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanEXSLT\XalanEXSLTStringImpl.hpp
# End Source File
# End Group
# Begin Group "XalanTransformer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanCAPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanCAPI.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanCompiledStylesheet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanCompiledStylesheetDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanCompiledStylesheetDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanDefaultDocumentBuilder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanDefaultDocumentBuilder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanDefaultParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanDefaultParsedSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanDocumentBuilder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanParsedSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanSourceTreeWrapperParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanSourceTreeWrapperParsedSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanTransformer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanTransformer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanTransformerDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanTransformerOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanTransformerOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanTransformerProblemListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XalanTransformerProblemListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XercesDOMParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XercesDOMParsedSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XercesDOMWrapperParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XalanTransformer\XercesDOMWrapperParsedSource.hpp
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=..\..\Res\AllInOne\AllInOne.rc
# End Source File
# Begin Source File

SOURCE=..\..\Res\AllInOne\resource.h
# End Source File
# End Group
# Begin Group "XercesParserLiaison"

# PROP Default_Filter ""
# Begin Group "Deprecated"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\FormatterToDeprecatedXercesDOM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\FormatterToDeprecatedXercesDOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesAttrBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesAttrBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesAttributeBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesAttributeBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesBridgeHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesBridgeHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesBridgeNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesBridgeNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesBridgeTypes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesCDATASectionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesCDATASectionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesCommentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesCommentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDocumentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDocumentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDocumentFragmentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDocumentFragmentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDocumentTypeBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDocumentTypeBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDOM_NodeHack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDOM_NodeHack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDOMImplementationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesDOMImplementationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesElementBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesElementBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesElementBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesElementBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesEntityBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesEntityBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesEntityReferenceBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesEntityReferenceBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesNamedNodeMapBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesNamedNodeMapBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesNodeListBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesNodeListBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesNotationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesNotationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesProcessingInstructionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesProcessingInstructionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesTextBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesTextBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesTextBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesTextBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesToXalanNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesToXalanNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesTreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\Deprecated\XercesTreeWalker.hpp
# End Source File
# End Group
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\FormatterToXercesDOM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\FormatterToXercesDOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesAttrWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCDATASectionWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCDATASectionWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCommentWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesCommentWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentTypeWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentTypeWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDocumentWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMFormatterWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMFormatterWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMImplementationWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMImplementationWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMWrapperException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesDOMWrapperException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesElementWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityReferenceWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityReferenceWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesEntityWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesLiaisonXalanDOMStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesLiaisonXalanDOMStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapAttributeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapAttributeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNamedNodeMapWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNodeListWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNodeListWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNotationWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesNotationWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesParserLiaisonDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesProcessingInstructionWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesProcessingInstructionWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesTextWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperNavigatorAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperNavigatorAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperToXalanNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperToXalanNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\XercesParserLiaison\XercesWrapperTypes.hpp
# End Source File
# End Group
# Begin Group "Harness"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Harness\XalanDiagnosticMemoryManager.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Harness\XalanDiagnosticMemoryManager.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Harness\XalanFileUtility.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Harness\XalanFileUtility.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Harness\XalanHarnessDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Harness\XalanXMLFileReporter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\xalanc\Harness\XalanXMLFileReporter.hpp
# End Source File
# End Group
# End Target
# End Project
