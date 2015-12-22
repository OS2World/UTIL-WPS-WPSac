# makefile
# Erstellt von IBM WorkFrame/2 MakeMake um 23:00:37 am 26 Feb 1997
#
# Diese Make-Datei enth„lt folgende Aktionen:
#  Compile::Resource Compiler
#  Compile::SOM Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES: .C .c .idl .obj .rc .res

.all: \
    .\WPStrtUp.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

{d:\work\c\StartupFldr}.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe -r %s %|dpfF.RES

.idl.c:
    @echo " Compile::SOM Compiler "
    sc.exe -S 65535 %s

{d:\work\c\StartupFldr}.idl.c:
    @echo " Compile::SOM Compiler "
    sc.exe -S 65535 %s

.C.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I.. /I. /I..\include /Ss /Q /Wcnveffparprorearetuse /Gm /Ge- /G5 /Ms /C %s

{d:\work\c\StartupFldr}.C.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I.. /I. /I..\include /Ss /Q /Wcnveffparprorearetuse /Gm /Ge- /G5 /Ms /C %s

.\WPStrtUp.dll: \
    $(ODB) \
    $(DDB) \
    .\WPStrtUp.obj \
    .\WPStrtup.res \
    {$(LIB)}somtk.lib \
    {$(LIB)}WPStrtUp.def
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     /B"$(LDB) /de /pmtype:pm /packd /nologo"
     /FeWPStrtUp.dll
     somtk.lib
     WPStrtUp.def
     $(ODB)
     $(DDB)
     .\WPStrtUp.obj
<<
    rc.exe .\WPStrtup.res WPStrtUp.dll

!include "makefile.DEP"
