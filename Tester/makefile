# IBM Developer's Workframe/2 Make File Creation run at 16:39:37 on 02/19/94

.SUFFIXES:
.SUFFIXES: .c .obj .h .rc .res

ALL: tester.exe \
     tester.res

tester.exe:  \
  tester.OBJ \
  loginout.obj \
  createpwd.obj \
  sammem.obj \
  profile.obj \
  rxout.obj \
  $(ODB) \
  tester.res \
  MAKEFILE
   ICC.EXE @<<
 /B" $(LDB) /NOL /map"
 /Fe"tester.exe" rexx.lib cutil.lib tester.def
createpwd.obj
tester.OBJ
profile.obj
$(ODB)
sammem.obj
rxout.obj
loginout.obj
<<
   RC tester.res tester.exe

{.}.rc.res:
   RC $(RDB) -r .\$*.RC

{.}.c.obj:
   ICC.EXE /I.. /I..\include /I. $(CDB) /Ss /Q /Wcnvparprorearetuse /Ms /Gm /G4 /C  .\$*.c

!include MAKEFILE.DEP
