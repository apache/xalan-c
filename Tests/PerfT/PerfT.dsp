# Microsoft Developer Studio Project File - Name="PerfT" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=PerfT - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PerfT.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PerfT.mak" CFG="PerfT - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PerfT - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PerfT - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE "PerfT - Win32 Release with symbols" (based on "Win32 (x86) Console Application")
!MESSAGE "PerfT - Win64 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "PerfT - Win64 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PerfT - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\Win32\VC6\Release"
# PROP Intermediate_Dir "..\..\Build\Win32\VC6\Release\PerfT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 xerces-c_3.lib /nologo /stack:0x1f4000 /subsystem:console /pdb:none /machine:I386 /libpath:"..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\lib"

!ELSEIF  "$(CFG)" == "PerfT - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Build\Win32\VC6\debug"
# PROP Intermediate_Dir "..\..\Build\Win32\VC6\debug\PerfT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /GR /GX /Od /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_3D.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:I386 /pdbtype:sept /libpath:"..\..\..\..\xml-xerces\c\Build\Win32\VC6\Debug" /libpath:"$(XERCESCROOT)\Build\Win32\VC6\Debug" /libpath:"$(XERCESCROOT)\lib"

!ELSEIF  "$(CFG)" == "PerfT - Win32 Release with symbols"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "PerfT___Win32_Release_with_symbols"
# PROP BASE Intermediate_Dir "PerfT___Win32_Release_with_symbols"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\Win32\VC6\Release.symbols"
# PROP Intermediate_Dir "..\..\Build\Win32\VC6\Release.symbols\PerfT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I "..\harness\\" /D "NDEBUG" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /Zi /O2 /Ob2 /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 ..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release\xerces-c_1.lib /nologo /subsystem:console /machine:I386 /out:"..\..\Build\Win32\VC6\Release/perft.exe"
# ADD LINK32 xerces-c_3.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:I386 /libpath:"..\..\..\..\xml-xerces\c\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\Build\Win32\VC6\Release" /libpath:"$(XERCESCROOT)\lib" /fixed:no
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "PerfT - Win64 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\Build\Win64\VC6\Release"
# PROP Intermediate_Dir "..\..\Build\Win64\VC6\Release\PerfT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "NDEBUG" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MD /W4 /GR /GX /O2 /Ob2 /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "NDEBUG" /D "_MBCS" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 xerces-c_3.lib /nologo /stack:0x1f4000 /subsystem:console /pdb:none /machine:IX86 /libpath:"..\..\..\..\xml-xerces\c\Build\Win64\VC6\Release" /libpath:"$(XERCESCROOT)\Build\Win64\VC6\Release" /libpath:"$(XERCESCROOT)\lib" /machine:IA64

!ELSEIF  "$(CFG)" == "PerfT - Win64 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\Build\Win64\VC6\debug"
# PROP Intermediate_Dir "..\..\Build\Win64\VC6\debug\PerfT"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W4 /GR /GX /Od /I "..\..\..\..\xml-xerces\c\src" /I "..\..\src\\" /I ".." /I "$(XERCESCROOT)\src" /I "$(XERCESCROOT)\include" /D "_MBCS" /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 xerces-c_3D.lib /nologo /stack:0x1f4000 /subsystem:console /debug /machine:IX86 /pdbtype:sept /libpath:"..\..\..\..\xml-xerces\c\Build\Win64\VC6\Debug" /libpath:"$(XERCESCROOT)\Build\Win64\VC6\Debug" /libpath:"$(XERCESCROOT)\lib" /machine:IA64

!ENDIF 

# Begin Target

# Name "PerfT - Win32 Release"
# Name "PerfT - Win32 Debug"
# Name "PerfT - Win32 Release with symbols"
# Name "PerfT - Win64 Release"
# Name "PerfT - Win64 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\perft.cpp

!IF  "$(CFG)" == "PerfT - Win32 Release"

# ADD CPP /MD /Zi

!ELSEIF  "$(CFG)" == "PerfT - Win32 Debug"

# PROP Intermediate_Dir "..\..\Build\Win32\VC6\Debug\PerfT"
# ADD CPP /MDd /Zi

!ELSEIF  "$(CFG)" == "PerfT - Win32 Release with symbols"

# ADD BASE CPP /MD /Zi
# ADD CPP /MD /Zi

!ELSEIF  "$(CFG)" == "PerfT - Win64 Release"

!ELSEIF  "$(CFG)" == "PerfT - Win64 Debug"

!ENDIF 

# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
