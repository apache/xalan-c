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
!MESSAGE "AllInOneWithICU - Win32 Release with symbols" (based on "Win32 (x86) Dynamic-Link Library")
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
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /D "NDEBUG" /D "XALAN_DOMSUPPORT_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /D "XALAN_PLATFORMSUPPORT_BUILD_DLL" /D "XALAN_DOM_BUILD_DLL" /D "XALAN_XALANEXTENSIONS_BUILD_DLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "XALAN_TRANSFORMER_BUILD_DLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /D "XALAN_XMLSUPPORT_BUILD_DLL" /D "XALAN_XPATH_BUILD_DLL" /D "XALAN_XPATHCAPI_BUILD_DLL" /D "XALAN_ALLINONE_BUILD_DLL" /D "XALAN_EXSLT_BUILD_DLL" /D "XALAN_ICUBRIDGE_BUILD_DLL" /FD /c
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
# ADD LINK32 xerces-c_2.lib icuuc.lib icuin.lib /nologo /dll /pdb:none /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release/Xalan-C_1_4_0.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release/Xalan-C_1.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"..\..\..\..\..\..\icu\lib"

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
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XSLT_EXPORTS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /Gm /GR /GX /Zi /Od /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /D "_DEBUG" /D "XALAN_DOMSUPPORT_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /D "XALAN_PLATFORMSUPPORT_BUILD_DLL" /D "XALAN_DOM_BUILD_DLL" /D "XALAN_XALANEXTENSIONS_BUILD_DLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "XALAN_TRANSFORMER_BUILD_DLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /D "XALAN_XMLSUPPORT_BUILD_DLL" /D "XALAN_XPATH_BUILD_DLL" /D "XALAN_XPATHCAPI_BUILD_DLL" /D "XALAN_ALLINONE_BUILD_DLL" /D "XALAN_EXSLT_BUILD_DLL" /D "XALAN_ICUBRIDGE_BUILD_DLL" /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_2D.lib icuucd.lib icuind.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Debug/Xalan-C_1_4_0D.dll" /implib:"..\..\..\..\Build\Win32\VC6\Debug/Xalan-C_1D.lib" /pdbtype:sept /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug" /libpath:"..\..\..\..\..\..\icu\lib"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "AllInOneWithICU - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ReleaseWithSymbols"
# PROP BASE Intermediate_Dir "ReleaseWithSymbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\..\..\Build\Win32\VC6\Release.symbols\AllInOne"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W4 /GR /GX /O2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /D "XALAN_XERCES" /FD /c
# SUBTRACT BASE CPP /YX
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\..\..\xml-xerces\c\src" /I "..\..\..\..\src\\" /I "..\..\..\..\..\..\icu\include" /D "NDEBUG" /D "XALAN_DOMSUPPORT_BUILD_DLL" /D "WIN32" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "XALAN_XSLT_BUILD_DLL" /D "XALAN_PLATFORMSUPPORT_BUILD_DLL" /D "XALAN_DOM_BUILD_DLL" /D "XALAN_XALANEXTENSIONS_BUILD_DLL" /D "XALAN_XALANSOURCETREE_BUILD_DLL" /D "XALAN_TRANSFORMER_BUILD_DLL" /D "XALAN_XERCESPARSERLIAISON_BUILD_DLL" /D "XALAN_XMLSUPPORT_BUILD_DLL" /D "XALAN_XPATH_BUILD_DLL" /D "XALAN_XPATHCAPI_BUILD_DLL" /D "XALAN_ALLINONE_BUILD_DLL" /D "XALAN_EXSLT_BUILD_DLL" /D "XALAN_ICUBRIDGE_BUILD_DLL" /FD /c
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
# ADD LINK32 xerces-c_2.lib icuuc.lib icuin.lib /nologo /dll /debug /machine:I386 /out:"..\..\..\..\Build\Win32\VC6\Release.symbols/Xalan-C_1_4_0S.dll" /implib:"..\..\..\..\Build\Win32\VC6\Release.symbols/Xalan-C_1S.lib" /libpath:"..\..\..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"..\..\..\..\..\..\icu\lib"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "AllInOneWithICU - Win32 Release"
# Name "AllInOneWithICU - Win32 Debug"
# Name "AllInOneWithICU - Win32 Release with symbols"
# Begin Group "Include"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\Include\PlatformDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Include\STLHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Include\VCPPDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Include\XalanArrayKeyMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Include\XalanAutoPtr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\Include\XalanObjectCache.hpp
# End Source File
# End Group
# Begin Group "XalanDOM"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanAttr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanCDataSection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanCDATASection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanCharacterData.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanCharacterData.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocumentFragment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocumentType.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDocumentType.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMImplementation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMImplementation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanDOMString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEmptyNamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEmptyNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEntity.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEntity.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEntityReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanEntityReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNode.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeListDummy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeListDummy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeListSurrogate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNodeListSurrogate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNotation.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanNotation.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanDOM\XalanText.hpp
# End Source File
# End Group
# Begin Group "PlatformSupport"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\ArenaAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\ArenaBlock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\AttributeListImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\AttributeListImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\AttributesImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\AttributesImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\AttributeVectorEntry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\AttributeVectorEntryExtended.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\Cloneable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\Cloneable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\DirectoryEnumerator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\DOMStringHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\DOMStringHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\DOMStringPrintWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\DOMStringPrintWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\DoubleSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\DoubleSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\ExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\ExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\FormatterListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\FormatterListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\NamedNodeMapAttributeList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\NamedNodeMapAttributeList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\NullPrintWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\NullPrintWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PlatformSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PlatformSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PlatformSupportDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PlatformSupportInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PlatformSupportInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PrefixResolver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PrefixResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PrintWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\PrintWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\Resettable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\Resettable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\ReusableArenaAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\ReusableArenaBlock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\StdBinInputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\StdBinInputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\StringTokenizer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\StringTokenizer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\URISupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\URISupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\Writer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\Writer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanBitmap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanBitmap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDecimalFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDecimalFormat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDecimalFormatSymbols.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDecimalFormatSymbols.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDOMStringAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDOMStringAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDOMStringCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDOMStringCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDOMStringHashTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDOMStringHashTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDOMStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanDOMStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanEncodingPropertyCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanEncodingPropertyCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanFileOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanFileOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanNullOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanNullOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanNumberFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanNumberFormat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanOutputStreamPrintWriter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanOutputStreamPrintWriter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanReferenceCountedObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanReferenceCountedObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanSimplePrefixResolver.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanSimplePrefixResolver.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanStdOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanStdOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanToXercesTranscoderWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanToXercesTranscoderWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanTranscodingServices.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanTranscodingServices.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanUnicode.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanUTF16Transcoder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanUTF16Transcoder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanXMLChar.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XalanXMLChar.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XSLException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\PlatformSupport\XSLException.hpp
# End Source File
# End Group
# Begin Group "DOMSupport"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMServices.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMServices.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupportDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupportException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupportException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupportInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\DOMSupportInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\TreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\TreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\XalanNamespacesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\DOMSupport\XalanNamespacesStack.hpp
# End Source File
# End Group
# Begin Group "XPath"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ElementPrefixResolverProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ElementPrefixResolverProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FormatterStringLengthCounter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FormatterStringLengthCounter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FoundIndex.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FoundIndex.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\Function.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\Function.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionBoolean.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionBoolean.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionCeiling.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionCeiling.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionConcat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionConcat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionContains.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionContains.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionCount.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionCount.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionFalse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionFalse.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionFloor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionFloor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLang.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLang.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLast.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLast.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLocalName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionLocalName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNamespaceURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNamespaceURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNormalizeSpace.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNormalizeSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionPosition.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionPosition.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionRound.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionRound.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionStartsWith.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionStartsWith.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionStringLength.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionStringLength.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstring.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstring.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstringAfter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstringAfter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstringBefore.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSubstringBefore.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSum.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionSum.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionTranslate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionTranslate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionTrue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\FunctionTrue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\MutableNodeRefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\MutableNodeRefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NameSpace.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeListImplSurrogate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeListImplSurrogate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeRefList.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeRefList.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeRefListBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\NodeRefListBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ResultTreeFragBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\ResultTreeFragBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XalanQName.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XalanQName.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XalanQNameByReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XalanQNameByReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XalanQNameByValue.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XalanQNameByValue.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XalanXPathException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XalanXPathException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XBoolean.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XBoolean.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XLocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XLocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSetAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSetAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSetBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSetBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSetResultTreeFragProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNodeSetResultTreeFragProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNull.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNull.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumberAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumberAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumberBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XNumberBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObject.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObject.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectFactoryDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectFactoryDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectResultTreeFragProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectResultTreeFragProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectResultTreeFragProxyBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectResultTreeFragProxyBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectResultTreeFragProxyText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectResultTreeFragProxyText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectTypeCallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XObjectTypeCallback.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEnvSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEnvSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEnvSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEnvSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEvaluator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathEvaluator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExecutionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExecutionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExpression.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathExpression.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactoryBlock.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactoryBlock.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactoryDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFactoryDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFunctionTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathFunctionTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathParserException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathParserException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathProcessorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XPathProcessorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XSpan.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XSpan.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringBase.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringBase.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringCached.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringCached.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringCachedAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringCachedAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringReference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringReference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringReferenceAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XStringReferenceAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XToken.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XToken.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XTokenNumberAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XTokenNumberAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XTokenNumberAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XTokenNumberAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XTokenStringAdapter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XTokenStringAdapter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XTokenStringAdapterAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XTokenStringAdapterAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XUnknown.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPath\XUnknown.hpp
# End Source File
# End Group
# Begin Group "XPathCAPI"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XPathCAPI\XPathCAPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XPathCAPI\XPathCAPI.h
# End Source File
# End Group
# Begin Group "XMLSupport"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToDOM.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToDOM.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToHTML.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToHTML.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToNull.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToNull.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToXML.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterToXML.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterTreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\FormatterTreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XalanHTMLElementsProperties.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XalanHTMLElementsProperties.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XMLParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XMLParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XMLSupportDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XMLSupportException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XMLSupportException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XMLSupportInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XMLSupport\XMLSupportInit.hpp
# End Source File
# End Group
# Begin Group "XercesParserLiaison"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttributeBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttributeBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesAttrWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeCommon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesBridgeNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCDATASectionWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesCommentWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentFragmentBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentFragmentBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentTypeBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentTypeBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentTypeWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentTypeWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDocumentWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOM_NodeHack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOM_NodeHack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMImplementationWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMWrapperException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesDOMWrapperException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesElementWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityReferenceWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesEntityWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesLiaisonXalanDOMStringPool.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesLiaisonXalanDOMStringPool.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeListCache.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeListCache.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNamedNodeMapWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNodeListWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesNotationWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesParserLiaisonDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesProcessingInstructionWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridgeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextBridgeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextWrapper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextWrapper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextWrapperAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTextWrapperAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesToXalanNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesToXalanNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTreeWalker.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesTreeWalker.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperNavigator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperNavigator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperToXalanNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XercesParserLiaison\XercesWrapperToXalanNodeMap.hpp
# End Source File
# End Group
# Begin Group "XalanSourceTree"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\FormatterToSourceTree.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\FormatterToSourceTree.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttr.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttr.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttributeAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttributeAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttributeNSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttributeNSAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttributesVector.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttributesVector.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttrNS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeAttrNS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeCommentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeCommentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeContentHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeContentHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeDocumentFragment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeDocumentFragment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeDOMSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeDOMSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeElementAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeElementAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeElementNS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeElementNS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeElementNSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeElementNSAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeHelper.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeHelper.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeNamedNodeMap.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeNamedNodeMap.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeParserLiaison.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeParserLiaison.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeProcessingInstruction.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeProcessingInstruction.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeProcessingInstructionAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeProcessingInstructionAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeTextAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeTextAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeTextIWS.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeTextIWS.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeTextIWSAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanSourceTree\XalanSourceTreeTextIWSAllocator.hpp
# End Source File
# End Group
# Begin Group "XSLT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVT.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVT.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPart.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPart.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPartSimple.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPartSimple.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPartXPath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\AVTPartXPath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\Constants.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\Constants.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\CountersTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\CountersTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\DecimalToRoman.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemApplyImport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemApplyImport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemApplyTemplates.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemApplyTemplates.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemAttribute.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemAttribute.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemAttributeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemAttributeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCallTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCallTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemChoose.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemChoose.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemComment.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemComment.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCopy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCopy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCopyOf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemCopyOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemDecimalFormat.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemDecimalFormat.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemEmpty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemEmpty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemExtensionCall.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemExtensionCall.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemFallback.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemFallback.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemForEach.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemForEach.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemIf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemIf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemLiteralResult.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemLiteralResult.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemMessage.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemMessage.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemOtherwise.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemOtherwise.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemPI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemSort.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemSort.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTemplate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTemplateElement.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTemplateElement.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemText.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemText.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTextLiteral.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemTextLiteral.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemUse.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemUse.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemValueOf.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemValueOf.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemVariable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemVariable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemWhen.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemWhen.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemWithParam.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ElemWithParam.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ExtensionFunctionHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ExtensionFunctionHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ExtensionNSHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ExtensionNSHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionCurrent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionCurrent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionDocument.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionDocument.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionElementAvailable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionElementAvailable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionFormatNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionFormatNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionFunctionAvailable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionFunctionAvailable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionGenerateID.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionGenerateID.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionSystemProperty.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionSystemProperty.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionUnparsedEntityURI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\FunctionUnparsedEntityURI.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\GenerateEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\GenerateEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\KeyDeclaration.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\KeyTable.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\KeyTable.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NamespacesHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NamespacesHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NodeSorter.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NodeSorter.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NodeSortKey.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\NodeSortKey.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\OutputContextStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\OutputContextStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ProblemListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ProblemListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ProblemListenerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ProblemListenerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultNamespacesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultNamespacesStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFragAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFragAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFragBaseNodeRefListBaseProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\ResultTreeFragBaseNodeRefListBaseProxy.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\SelectionEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\SelectionEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StackEntry.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StackEntry.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\Stylesheet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\Stylesheet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetConstructionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetConstructionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetConstructionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetConstructionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetExecutionContext.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetExecutionContext.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetExecutionContextDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetExecutionContextDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetHandler.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetHandler.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetRoot.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\StylesheetRoot.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TopLevelArg.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TopLevelArg.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TraceListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TraceListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TraceListenerDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TraceListenerDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TracerEvent.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\TracerEvent.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\VariablesStack.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\VariablesStack.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanNumberingResourceBundle.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanNumberingResourceBundle.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanSourceTreeDocumentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanSourceTreeDocumentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanSourceTreeDocumentFragmentAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanSourceTreeDocumentFragmentAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XalanTemplate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XResultTreeFrag.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XResultTreeFrag.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XResultTreeFragAllocator.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XResultTreeFragAllocator.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTEngineImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTEngineImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTInit.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTInit.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTInputSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTInputSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorEnvSupport.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorEnvSupport.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorEnvSupportDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorEnvSupportDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorException.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTProcessorException.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTResultTarget.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XSLT\XSLTResultTarget.hpp
# End Source File
# End Group
# Begin Group "ICUBridge"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\FunctionICUFormatNumber.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\FunctionICUFormatNumber.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridge.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridge.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridgeCleanup.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridgeCleanup.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridgeCollationCompareFunctor.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridgeCollationCompareFunctor.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridgeCollationCompareFunctorImpl.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridgeCollationCompareFunctorImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUBridgeDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUXalanNumberFormatFactory.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUXalanNumberFormatFactory.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUXalanNumberFormatProxy.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\ICUBridge\ICUXalanNumberFormatProxy.hpp
# End Source File
# End Group
# Begin Group "XalanExtensions"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionDifference.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionDifference.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionDistinct.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionDistinct.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionEvaluate.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionEvaluate.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionHasSameNodes.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionHasSameNodes.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionIntersection.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionIntersection.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionNodeSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\FunctionNodeSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\XalanExtensions.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\XalanExtensions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanExtensions\XalanExtensionsDefinitions.hpp
# End Source File
# End Group
# Begin Group "XalanEXSLT"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTCommon.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTCommon.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTCommonImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTMath.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTMath.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTMathImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTSet.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTSet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTSetImpl.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTString.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTString.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanEXSLT\XalanEXSLTStringImpl.hpp
# End Source File
# End Group
# Begin Group "XalanTransformer"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanCAPI.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanCAPI.h
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanCompiledStylesheet.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanCompiledStylesheetDefault.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanCompiledStylesheetDefault.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanDefaultDocumentBuilder.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanDefaultDocumentBuilder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanDefaultParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanDefaultParsedSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanDocumentBuilder.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanParsedSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanSourceTreeWrapperParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanSourceTreeWrapperParsedSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanTransformer.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanTransformer.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanTransformerDefinitions.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanTransformerOutputStream.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanTransformerOutputStream.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanTransformerProblemListener.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XalanTransformerProblemListener.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XercesDOMParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XercesDOMParsedSource.hpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XercesDOMWrapperParsedSource.cpp
# End Source File
# Begin Source File

SOURCE=..\..\..\..\src\XalanTransformer\XercesDOMWrapperParsedSource.hpp
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
# End Target
# End Project
