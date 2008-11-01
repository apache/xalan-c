# Microsoft Developer Studio Project File - Name="Localization" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) External Target" 0x0106

CFG=Localization - Win32 Release
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Localization.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Localization.mak" CFG="Localization - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Localization - Win32 Release" (based on "Win32 (x86) External Target")
!MESSAGE "Localization - Win32 Debug" (based on "Win32 (x86) External Target")
!MESSAGE "Localization - Win64 Release" (based on "Win64 (x86) External Target")
!MESSAGE "Localization - Win64 Debug" (based on "Win64 (x86) External Target")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""

!IF  "$(CFG)" == "Localization - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Cmd_Line "NMAKE /f BuildMessages.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Cmd_Line "nmake /f "BuildMessages.mak"  TYPE=inmem LOCALE=en_US"
# PROP Rebuild_Opt "/a"
# PROP Target_File "Localization"

!ELSEIF  "$(CFG)" == "Localization - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Cmd_Line "NMAKE /f BuildMessages.mak"
# PROP BASE Rebuild_Opt "/a"
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Cmd_Line "nmake    /f "BuildMessages.mak" /NOLOGO CFG=Debug TYPE=inmem LOCALE=en_US"
# PROP Rebuild_Opt "/a"
# PROP Target_File "Localization"

!ELSEIF  "$(CFG)" == "Localization - Win64 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Cmd_Line "nmake /f "BuildMessages.mak"  TYPE=inmem LOCALE=en_US"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Localization"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Cmd_Line "nmake /f "BuildMessages.mak"  TYPE=inmem BITS=Win64 LOCALE=en_US"
# PROP Rebuild_Opt "/a"
# PROP Target_File "Localization"

!ELSEIF  "$(CFG)" == "Localization - Win64 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Cmd_Line "nmake    /f "BuildMessages.mak" /NOLOGO CFG=Debug TYPE=inmem LOCALE=en_US"
# PROP BASE Rebuild_Opt "/a"
# PROP BASE Target_File "Localization"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Cmd_Line "nmake    /f "BuildMessages.mak" /NOLOGO CFG=Debug TYPE=inmem BITS=Win64 LOCALE=en_US"
# PROP Rebuild_Opt "/a"
# PROP Target_File "Localization"

!ENDIF 

# Begin Target

# Name "Localization - Win32 Release"
# Name "Localization - Win32 Debug"
# Name "Localization - Win64 Release"
# Name "Localization - Win64 Debug"

!IF  "$(CFG)" == "Localization - Win32 Release"

!ELSEIF  "$(CFG)" == "Localization - Win32 Debug"

!ENDIF 

# Begin Source File

SOURCE=..\..\..\..\..\src\xalanc\NLS\en_US\XalanMsg_en_US.xlf
# End Source File
# End Target
# End Project
