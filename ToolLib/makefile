# makefile
# Erstellt von IBM WorkFrame/2 MakeMake um 0:21:12 am 28 Nov 1997
#
# Diese Make-Datei enth„lt folgende Aktionen:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I.. /I..\include /Q /Gm /Gd /G5 /C %s

.\SamUtil.lib: \
    .\ShareMem.obj \
    .\Profile.obj
      @echo " Lib::Object Library Manager "
      ilib.exe /nol SamUtil.lib -+.\ShareMem.obj -+Profile.obj;

!include "makefile.DEP"
