# makefile
# Erstellt von IBM WorkFrame/2 MakeMake um 1:53:49 am 6 Jan 1999
#
# Diese Make-Datei enth„lt folgende Aktionen:
#  Compile::Resource Compiler
#  Compile::C++ Compiler
#  Link::Linker
#  Bind::Resource Bind

.SUFFIXES:

.SUFFIXES: \
    .c .obj .ipf .scr .rc .res .exe

ALL: \
     Sammy$(PLANG).res \
     Sammy.exe \
     Sammy.hlp

.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe $(RDB) -r .\$*.RC

{d:\work\c\wpsac}.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe $(RDB) -r .\$*.RC

{d:\work\c\wpsac\sammy30}.rc.res:
    @echo " Compile::Resource Compiler "
    rc.exe $(RDB) -r .\$*.RC

# /I      Search path for include files
# /Ss     Allow "//" for comment lines
# /Q      Disable logo
# /W      Warning
#   eff   Statements with no effect
#   par   Unused parameters
#   pro   Missing function prototypes
#   ret   Consistency of statements
#   use   Unused "auto" and "static" variables
# /Ti     Include debug information
# /Ms     Use the system calling convention and not optlink as the default
# /Gm     Use the multi-threaded libraries
# /G4     Optimize code for 80486
# /C      Compile don't link

.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(SWM) $(CDB) /I.. /I..\include /Ss /Q /Weffparproretuse /Ms /Gm /G5 /C %s

{d:\work\c\wpsac}.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(SWM) $(CDB) /I.. /I..\include /Ss /Q /Weffparproretuse /Ms /Gm /G5 /C %s

{d:\work\c\wpsac\sammy30}.c.obj:
    @echo " Compile::C++ Compiler "
    icc.exe $(SWM) $(CDB) /I.. /I..\include /Ss /Q /Weffparproretuse /Ms /Gm /G5 /C %s

Sammy.hlp:  \
  Sammy$(PLANG).ipf  \
  MAKEFILE
    IPFC .\$*$(PLANG).ipf
    if exist Sammy.hlp del Sammy.hlp
    ren .\$*$(PLANG).hlp Sammy.hlp

{.}.scr.ipf:
   $(IPFCP)\IPFCPREP .\$*.SCR .\$*.IPF

.\sammy.exe: \
    .\sammy.obj \
    .\SamData.obj \
    .\SamDlg.obj \
    .\SacAlias.obj \
    .\SCenter.obj \
    .\Authorize.obj \
    .\bitmap.obj \
    .\LANDrive.obj \
    .\Environ.obj \
    .\SamMem.obj \
    .\rxout.obj \
    .\profile.obj \
    .\Prepare.obj \
    .\optsdlg.obj \
    .\loginout.obj \
    .\CreatePwd.obj \
    .\createObj.obj \
    .\ChangePwd.obj \
    .\MAKEINI.obj \
    .\MAGSUPP.obj \
    $(ODB) \
    .\Sammy.def \
    .\Sammy$(PLANG).res
    @echo " Link::Linker "
    @echo " Bind::Resource Bind "
    icc.exe @<<
     /B"$(LDB) /NOL /map"
     /Fesammy.exe cutil.lib rexx.lib Sammy.def
    .\SamData.obj
    .\SamDlg.obj
    .\SacAlias.obj
    .\SCenter.obj
    .\Authorize.obj
    .\bitmap.obj
    .\LANDrive.obj
    .\Environ.obj
    .\SamMem.obj
    .\rxout.obj
    .\profile.obj
    .\Prepare.obj
    .\optsdlg.obj
    .\loginout.obj
    .\CreatePwd.obj
    .\createObj.obj
    .\ChangePwd.obj
    .\MAKEINI.obj
    .\MAGSUPP.obj
    .\sammy.obj
     $(ODB)
<<
    rc.exe .\Sammy$(PLANG).res sammy.exe

!include "makefile.DEP"
