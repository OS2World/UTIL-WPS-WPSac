# makefile
# Erstellt von IBM WorkFrame/2 MakeMake um 13:06:46 am 13 Mai 1999
#
# Diese Make-Datei enth„lt folgende Aktionen:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES:

.SUFFIXES: \
    .cpp .obj 

.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I..\include /I..\ToolLib /Q /Ti /G5 /C %s

{d:\work\c\wpsac\cryptpasswd}.cpp.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(CDB) /I..\include /I..\ToolLib /Q /Ti /G5 /C %s

all: \
    .\encrypt.exe

.\encrypt.exe: \
    .\main.obj \
    ..\ToolLib\SamUtil.lib \
    {$(LIB)}cutil.lib
    @echo " Link::Linker "
    icc.exe @<<
     /B" /de $(LDB)"
     /Feencrypt.exe 
     ..\ToolLib\SamUtil.lib 
     cutil.lib 
     .\main.obj
<<

.\main.obj: \
    d:\work\c\wpsac\cryptpasswd\main.cpp \
    {d:\work\c\wpsac\cryptpasswd;..\include;..\ToolLib;$(INCLUDE);}samglob.h \
    {d:\work\c\wpsac\cryptpasswd;..\include;..\ToolLib;$(INCLUDE);}magpro2.h \
    {d:\work\c\wpsac\cryptpasswd;..\include;..\ToolLib;$(INCLUDE);}Profile.hpp
