/*
  �������������������������������������������������������������Ŀ
  �  Installieren der MAG-PROtect/2 Version 3.0                 �
  �  Die Datei WPSAC mu� sich standardm��ig in A:               �
  �  befinden, andere Laufwerke k�nnen durch                    �
  �  �nderung der Variablen 'SrcDrive' benutzt                  �
  �  werden (es kann auch ein Pfad angegeben werden)            �
  �  Dieses Installationsprogramm �ndert die                    �
  �  Datei CONFIG.SYS in den Zeilen:                            �
  �  - LIBPATH                                                  �
  �  - SET RUNWORKPLACE                                         �
  �  - SET SAMWORKPLACE                                         �
  �  - SET HELP                                                 �
  �  Die alte Version wird als CONFIG.BAK gesichert             �
  �  Es wird eine INI-Datei f�r UPROFILE.INI und                �
  �  SAMMY.EXE mit einem Default-Benutzer ROOT                  �
  �  (Pa�wort ROOT) angelegt                                    �
  �                                                             �
  �  Aufruf:                                                    �
  �                                                             �
  �                  ���������������������������Ŀ              �
  �                  V                           �              �
  �  ��� sacinst���������������������������������������������>  �
  �                � ��� /D:bootlaufwerk �������Ĵ �            �
  �                � ��� /P:installpfad ��������Ĵ �            �
  �                � ��� /I:deskmanicons �������Ĵ �            �
  �                � ��� /S:sourcepfad ���������Ĵ �            �
  �                � ��� /U ���������������������� �            �
  �                ����� /? ������������������������            �
  �                                                             �
  � Parameter /U steht f�r Update v. WPSAC                      �
  � Parameter /? zeigt diese Zeilen an                          �
  �                                                             �
  � Beispiel:                                                   �
  � SACINST /D:C: /P:C:\WPSac /I:C:\DEVTECH\ICO /S:C:\INSTALL   �
  �                                                             �
  ���������������������������������������������������������������
*/
echo off

/* Dateien in WPSAC */
/* \OS2\DLL-Verzeichnis */
os.1 = wpsac.dll
os.2 = authoriz.dll
os.3 = wpstrtup.dll
os.0 = 3
/* installpfad-Verzeichnis */
d.1  = cppoor3u.dll
d.2  = uprofile.exe
d.3  = uprofile.hlp
d.4  = uprofilg.dll
d.5  = script.tpl
d.6  = wpsac.inf
d.7  = sammy.exe
d.8  = sammy.hlp
d.9  = wp_parse.exe
d.10 = dummy.dat
d.11 = dummy.sac
d.12 = magclass.exe
d.0  = 12

/* REXX-Dienstprogrammfunktionen */
Call RxFuncadd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs

/* Parameter abfragen */
parse upper arg all
if all <>'' then
do i=1 to words(all)
  var=word(all,i)
  select
    when pos('/?',var)<>0 then call Hilfe
    when pos('/D:',var)<>0 then drive=substr(var,4)
    when pos('/P:',var)<>0 then QualPath=substr(var,4)
    when pos('/I:',var)<>0 then DeskmanIcons=substr(var,4)
    when pos('/S:',var)<>0 then SrcDrive=substr(var,4)
    when pos('/U',var)<>0 then Update=1
    otherwise
      NOP
  end
end

/* Auflisten aller vorhandenen lokalen Laufwerke ab Laufwerk C: */
LWLocal        = SysDriveMap( 'C:', 'Local' )

/* Abfrage: Bootlaufwerk */
boot:
if drive='DRIVE' then
  do
  Say 'Bootlaufwerk (Bsp.: C:): '
  pull drive
  if pos(drive, LWLocal)=0
    then
    do
      Say "Laufwerksbuchstabe nicht korrekt eingegeben, bitte wiederholen!"
      signal boot
    end
  end
else
  do
  if pos(drive, LWLocal)=0
    then
    do
      Say "Laufwerksbuchstabe nicht korrekt eingegeben, bitte wiederholen!"
      exit
    end
  end

/* Variablen / Defaultwerte */
IniFile        = 'UPROFILE.INI'
AppName        = 'WPSac'
if QualPath='QUALPATH' then
  QualPath     = drive'\'AppName
if SrcDrive='SRCDRIVE' then
  SrcDrive     = 'a:'
if DeskmanIcons='DESKMANICONS' then
  DeskmanIcons = 'c:\devtech\ico'
infile = drive'\config.bak'
tmpfile= drive'\config.tmp'
outfile= drive'\config.sys'


/* Verzeichnis f�r WPSac und uprofile anlegen */
Say ""
Say "MAG-PROtect Verzeichnis anlegen"
say""
call SysMkDir QualPath
call SysMkDir QualPath'\help'
call SysMkDir QualPath'\usr'

/* Untersuchung, ob WPSac bereits installiert ist */
OldPath = SysIni(SYSTEM,AppName,'INI_File')
if length(OldPath) = 0
  then
  do
    if OldPath <> QualPath
      then
      do
        Say "WPSac wurde bereits auf diesem PC installiert!"
        Say "Die neue INI-Datei hei�t:"
        Say QualPath'\'IniFile
        SysIni(SYSTEM,AppName,'DELETE:')
      end
  end

/* Pfade in OS2SYS.INI eintragen */
Say "Anlegen der INI-Datei "QualPath"\"IniFile
err = 0
rc = SysIni(SYSTEM,AppName,'UprofileINIPath',QualPath)
err = err + length(rc)
rc = SysIni(SYSTEM,AppName,'UprofileINIFile',IniFile)
err = err + length(rc)
rc = SysIni(SYSTEM,AppName,'DesktopPath',QualPath'\usr')
err = err + length(rc)
rc = SysIni(SYSTEM,AppName,'ClientPathLocal',QualPath)
err = err + length(rc)
if err > 0
  then
  do
    Say "*** Es ist ein Fehler beim Schreiben der OS2SYS.INI-Datei aufgetreten."
    Say "    Die Installation wird beendet"
    Exit
  end

/* Neue INI-Datei anlegen und Default-Benutzer anlegen */
err = 0
rc = SysIni(QualPath'\'IniFile,'ROOT','PassWord','ROOT')
err = err + length(rc)
rc = SysIni(QualPath'\'IniFile,'_Files','IconSource',DeskmanIcons)
err = err + length(rc)

if err > 0
  then
  do
    Say "*** Es ist ein Fehler beim Schreiben der UPROFILE.INI-Datei aufgetreten."
    Say "    Die Installation wird beendet"
    Exit
  end

/* Dateien entpacken */
Say ""
Say "Dateien entpacken"
Say ""
/* Test, ob mreplace vorhanden */
'rxqueue /clear'
'mreplace | rxqueue'
if if queued()=0 then
  do
  Say "MREPLACE.EXE nicht gefunden! Programm beenden (J/N)?"
  pull answer
  if translate(answer)='J' then
    Exit
  end
do i=1 to os.0      /* ins Verzeichnis \OS2\DLL */
  'mreplace' drive'\OS2\DLL\'os.i '>NUL'
  'unpack 'SrcDrive'\wpsac' drive '/N:'os.i '>NUL'
end
do i=1 to d.0       /* ins Verzeichnis installpfad */
  if d.i='sammy.exe' & Update=1 then
    do
      /* WPStartup2 deregistrieren */
       RetCode = SysDeregisterObjectClass( "WPStartup2" )
      if RetCode then
        say 'Deregister successfully completed'
      else
        say 'Could not deregister classes'
      /* WPSac deregistrieren */
      QualPath'\MAGclass DEREGISTER'
      if RC>0 then
        do
        Say "Fehler beim Registrieren der Klasse WPSac! Programm beenden (J/N)?"
        pull answer
        if translate(answer)='J' then
          Exit
        end
    end
  'unpack 'SrcDrive'\wpsac' QualPath '/N:'d.i '>NUL'
end

/* WPSac registrieren */
Say ""
Say "WPS-Klasse registrieren"
Say ""

QualPath'\MAGclass REPLACE'
if RC>0 then
  do
  Say "Fehler beim Registrieren der Klasse WPSac! Programm beenden (J/N)?"
  pull answer
  if translate(answer)='J' then
    Exit
  end

/* WPStartup2 registrieren */
say ""
say "WPS-Klasse registrieren"
say ""

do until SysRegisterObjectClass("WPStartup2", WPStrtup)
   beep(1890,100)
   call SysSleep 1
   end

say ""
say "WPStartup2-Klasse registriert"


beep(3000,100)
beep(2832,100)
beep(3000,100)

/* SET RUNWORKPLACE suchen */
flag1 = 0
flag2 = 0
call SysFileSearch "SET RUNWORKPLACE=", outfile, "Run"
if Run.0<>0 then
  do i=1 to Run.0
    if pos('SET RUNWORKPLACE=', run.i)=1 then flag1=1
  end
if Run.0=0 | flag1<>1 then
  do
  say "'SET RUNWORKPLACE=' nicht gefunden!"
  signal Unstimmig
  end
/* "SET SAMWORKPLACE="-Zeile gefunden? */
call SysFileSearch "SET SAMWORKPLACE=", outfile, "Sam"
  do i=1 to Sam.0
    if pos('SET SAMWORKPLACE=', Sam.i)=1 then flag2=1
  end

/* config.sys �ndern */
Say "CONFIG.SYS �ndern; Originaldatei wird in CONFIG.BAK abgelegt"
'copy' outfile infile
Say "Es wird eine CONFIG.TMP angelegt."
'del' tmpfile '2>nul'
signal on NOTREADY
result = linein(infile, 1, 0)
result = lineout(tmpfile, , 1)
signal off NOTREADY

do until lines(infile)=0
  line = linein(infile)
  select
    when Pos("SET RUNWORKPLACE=", (TRANSLATE(line)))=1 then
      do
      /* Ist RUNWORKPLACE nicht SAMMY.EXE, mu� installiert werden */
      select
        when Pos('sammy.exe',line)=0 & flag2=1 then
          do
          result = lineout(tmpfile, 'REM 'line)
          result = lineout(tmpfile, 'SET RUNWORKPLACE='QualPath'\sammy.exe')
          end
        when Pos('sammy.exe',line)=0 & flag2=0 then
          do
          result = lineout(tmpfile, 'REM 'line)
          result = lineout(tmpfile,overlay('SAM', line, 5))
          result = lineout(tmpfile, 'SET RUNWORKPLACE='QualPath'\sammy.exe')
          end
        when Pos('sammy.exe',line)<>0 & flag2=1 then
          do
          result = lineout(tmpfile, 'SET RUNWORKPLACE='QualPath'\sammy.exe')
          end
        otherwise
          signal Unstimmig
        end
      end
    when Pos("SET SAMWORKPLACE=", (TRANSLATE(line)))=1 then
      do
      result = lineout(tmpfile,line)
      end
    when Pos("SET HELP=", (TRANSLATE(line)))=1 then
      do
      /* Help-Path mu� m�glicherweise erg�nzt werden */
      if Pos(QualPath,line)=0 then
        result = lineout(tmpfile, insert(QualPath";",line,9))
      else
        result = lineout(tmpfile,line)
      end
    when Pos("LIBPATH=", (TRANSLATE(line)))=1 then
      do
      /* Eintragen des DLL-Pfades auf die WPS-Klasse */
      if Pos(QualPath,line)=0 then
        result = lineout(tmpfile, insert(QualPath";",line,8))
      else
        result = lineout(tmpfile,line)
      end
    otherwise
      result = lineout(tmpfile,line)
    end
end

result=lineout(tmpfile)
Say "CONFIG.SYS wird nun aus CONFIG.TMP erzeugt!"
'copy' tmpfile outfile
Say "Die CONFIG.TMP wird nun gel�scht!"
'del' tmpfile

Say ""
Say "Vor der Benutzung von WPSac den Rechner neu starten"
Say ""

Ende:
Exit
/* Fehlerbehandlung */

NOTREADY:
parse value sourceline(sigl) with . "(" file ","
Say "Fehler beim �ffnen von "value(file)
Exit

Unstimmig:
Say "Der Inhalt der Datei CONFIG.SYS ist nicht konsistent. Es sollte"
Say "die Backup-Datei CONFIG.BAK-Datei zur�ckgesichert werden!"
Exit

Hilfe:
call RxFuncAdd 'SysTextScreenSize', 'RexxUtil', 'SysTextScreenSize'
parse value SysTextScreenSize() with zeile spalte
j=-1
do i=2
  line = sourceline(i)
  if pos( '*/', line)<>0 then leave
  j=j+1
  if j=zeile-2 then
  do
    j=0
    say 'Weiter --> eine Taste dr�cken . . .'
    rc=SysCurPos( zeile-2, 0 )
    rc=SysGetKey( 'Noecho' )
  end
  say left( line, spalte-1 )
end
Exit
