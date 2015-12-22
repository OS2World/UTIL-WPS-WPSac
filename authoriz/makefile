# makefile
# Created by IBM WorkFrame/2 MakeMake at 21:20:39 on 9 Feb 1997
#
# The actions included in this make file are:
#  Compile::C++ Compiler
#  Link::Linker

.SUFFIXES:

.SUFFIXES: .C .obj

.all: \
    .\authoriz.dll

# Compile switchs  that are enabled
# /I      Search path for include files
#(/Ti     Include debug information )  \  aus CDB-Makro
#(/Dxxx   Define xxx)                  /     (DEBUG)
# /Ss     Allow "//" for comment lines
# /Q      Disable logo
# /W      Warning
#   cnv   Conversions
#   par   Unused parameters
#   pro   Missing function prototypes
#   rea   Code that cannot be reached
#   ret   Consistency of statements
#   use   Unused "auto" and "static" variables
# /Ms     Use the system calling convention and not optilink as the default
# /Gm     Use the multi-threaded libraries
# /Ge-    Build DLL
# /G4     Optimize code for 80486
# /C      Compile don't link
.c.obj:
   ICC.EXE /I.. $(CDB) /Ss /Q /Wcnvparprorearetuse /Ms /Gm /Ge- /G4 /C  .\$*.c

.\authoriz.dll: \
    .\AUTHORIZ.obj \
    makefile
    @echo " Link::Linker "
   ICC.EXE @<<
 /B" $(LDB) /nologo /noi /map /exepack /packcode /packdata /align:16"
 /Fe"Authoriz.DLL" Authoriz.def
     .\AUTHORIZ.obj
<<

.\AUTHORIZ.obj: \
    .\AUTHORIZ.C \
    .\WP_Parse.h \
    .\NB_BTB.h \
    makefile
