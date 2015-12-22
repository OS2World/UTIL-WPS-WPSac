# makefile
# Erstellt von IBM WorkFrame/2 MakeMake um 17:20:11 am 25 Feb 1997
#
# Diese Make-Datei enth„lt folgende Aktionen:
#  Compile::Resource Compiler
#  Compile::SOM Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES:

.SUFFIXES: .c .idl .obj .rc .res

.ALL: \
    .\WPSac$(PLANG).res \
    .\WPSac.dll

.rc.res:
    @echo " Compile::Resource Compiler "
    echo $(PLANG)
    rc.exe $(RDB) -r .\$*.RC

{d:\work\c\wpsac\som2}.rc.res:
    @echo " Compile::Resource Compiler "
    echo $(PLANG)
    rc.exe $(RDB) -r .\$*.RC

.idl.c:
    @echo " Compile::SOM Compiler "
    sc.exe %s

{d:\work\c\wpsac\wpsac3}.idl.c:
    @echo " Compile::SOM Compiler "
    sc.exe %s

.C.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I..\include /I.. /I. /Ss /Wcnveffparprorearetuse /Gm /Ge- /G5 /Ms /C %s

{d:\work\c\wpsac\wpsac3}.C.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I..\include /I.. /I. /Ss /Wcnveffparprorearetuse /Gm /Ge- /G5 /Ms /C %s

.\WPSac.dll: \
    $(DDB) \
    $(ODB) \
    .\wpsac.obj \
    .\WPSac$(PLANG).res \
    {$(LIB)}somtk.lib \
    {$(LIB)}WPSac.def
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     /B"$(LDB) /pmtype:pm /packd /nologo /map"
     /FeWPSac.dll
     somtk.lib
     WPSac.def
     $(DDB)
     $(ODB)
     .\wpsac.obj
<<
    rc.exe .\WPSac$(PLANG).res WPSac.dll

!include "makefile.DEP"
