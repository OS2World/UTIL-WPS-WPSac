# makefile
# Erstellt von IBM WorkFrame/2 MakeMake um 22:06:02 am 8 Jan 1999
#
# Diese Make-Datei enth„lt folgende Aktionen:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES:

.SUFFIXES: \
    .c .obj

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I..\include /DPURE_32 /Ss /Wdcleffparprorearetuse /Q /Ti /Gm /Ge- /G5 /C %s

{d:\work\c\wpsac\LSPrelog}.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I..\include /DPURE_32 /Ss /Wdcleffparprorearetuse /Q /Ti /Gm /Ge- /G5 /C %s

.\LSPrelog.dll: \
    .\LSPrelog.obj \
    .\PRINTF.obj \
    $(ODB) \
    {$(LIB)}CUtil.LIB \
    {$(LIB)}UPM32.LIB \
    {$(LIB)}LSPrelog.def
    @echo " Link::Linker "
    icc.exe @<<
     /Q /B"$(LDB)"
     /FeLSPrelog.dll
     CUtil.LIB
     UPM32.LIB
     NETAPI32.LIB
     LSPrelog.def
     .\LSPrelog.obj
     $(ODB)
<<

!include "makefile.DEP"
