# Microsoft Developer Studio Project File - Name="llexec" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=llexec - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "llexec.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "llexec.mak" CFG="llexec - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "llexec - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "llexec - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "llexec - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\..\obj\llexec\Release"
# PROP Intermediate_Dir "..\..\..\..\obj\llexec\Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /I "../../../src/hdrs" /I "../../../src/hdrs/win32" /I "../../../intl" /I "." /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /D "HAVE_CONFIG_H" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib version.lib /nologo /subsystem:console /machine:I386 /out:"..\..\..\..\bin\llines\Release/llexec.exe"

!ELSEIF  "$(CFG)" == "llexec - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\..\obj\llexec\Debug"
# PROP Intermediate_Dir "..\..\..\..\obj\llexec\Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "../../../src/hdrs" /I "../../../src/hdrs/win32" /I "../../../intl" /I "." /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /D "HAVE_CONFIG_H" /FD /GZ /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib version.lib /nologo /subsystem:console /debug /machine:I386 /out:"..\..\..\..\bin\llines\Debug/llexec.exe" /pdbtype:sept

!ENDIF 

# Begin Target

# Name "llexec - Win32 Release"
# Name "llexec - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\..\..\src\btree\addkey.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\alloc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\arch\alphasort.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\appendstr.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\bfs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\btree\block.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\brwslist.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\builtin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\charmaps.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\choose.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\date.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\dirs.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\dispfmt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\double.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\environ.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\equaliso.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\liflines\error.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\eval.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\btree\file.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\fpattern.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\functab.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\gedcom.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\gengedc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\getopt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\gstrings.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\heapused.c
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\iconvshim.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\icvt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\btree\index.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\indiseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\init.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\interp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\win32\intlshim.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\intrface.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\intrpseq.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\keytonod.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\arch\langinfo.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\lex.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\listener.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\lldate.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\liflines\llexec.c
# End Source File
# Begin Source File

SOURCE=.\llexec.rc
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\lloptions.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\llstrcmp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\locales.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\memalloc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\messages.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\misc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\more.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\mystring.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\names.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\node.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\nodeutls.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\btree\opnbtree.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\path.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\place.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\property.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\pvalmath.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\pvalue.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\rassa.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\btree\record.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\refns.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\remove.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\replace.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\arch\scandir.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\sequence.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\signals.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\arch\sleep.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\spltjoin.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\sprintpic.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\stdstrng.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\stralloc.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\strapp.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\strcvt.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\strutf8.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\strwhite.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\symtab.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\table.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\translat.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\btree\traverse.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\btree\utils.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\valid.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\valtable.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\arch\vsnprintf.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\warehouse.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\write.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\xreffile.c
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\yacc.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\..\..\src\hdrs\arch.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\bfs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\btree.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\btree\btreei.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\cache.h
# End Source File
# Begin Source File

SOURCE=.\config.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\date.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\feedback.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\fpattern.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\gedcheck.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\gedcom.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\gedlib\gedcomi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\gengedc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\win32\getopt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\win32\iconv.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\win32\iconvshim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\icvt.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\indiseq.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\interp.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\interpi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\win32\intlshim.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\isolangs.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\langinfz.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\liflines\llexec.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\lloptions.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\llstdlib.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\metadata.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\msvc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\mystring.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\parse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\sequence.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\standard.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\stdlib\stdlibi.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\sys_inc.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\table.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\translat.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\hdrs\warehouse.h
# End Source File
# Begin Source File

SOURCE=..\..\..\src\interp\yacc.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\llines.ico
# End Source File
# Begin Source File

SOURCE=..\llines.ico
# End Source File
# End Group
# End Target
# End Project