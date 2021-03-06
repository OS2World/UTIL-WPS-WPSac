/*
  旼컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
  �  Deinstallieren der MAG-PROtect/2 Version 3.0               �
  �  Dieses Deinstallationsprogramm 꼗dert die                  �
  �  Datei CONFIG.SYS in den Zeilen:                            �
  �  - LIBPATH                                                  �
  �  - SET RUNWORKPLACE                                         �
  �  - SET SAMWORKPLACE                                         �
  �  - SET HELP                                                 �
  �  Die alte Version wird als CONFIG.BAK gesichert             �
  �  Es wird eine INI-Datei f걊 UPROFILE.INI entfernt           �
  �                                                             �
  �  Aufruf:                                                    �
  �                                                             �
  �                  旼컴컴컴컴컴컴컴컴컴컴컴컴컴�              �
  �                  V                           �              �
  �  쳐� deinst 컴쩡컫컴컴컴컴컴컴컴컴컴컴컴컴컴컵컴쩡컴컴컴�>  �
  �               �  쳐� /D:bootlaufwerk 컴컴컴컴�  �           �
  �               �  쳐� /P:installpfad 컴컴컴컴캑  �           �
  �               �  읕� /I:deskmanicons 컴컴컴컴�  �           �
  �               읕컴컴 /? 컴컴컴컴컴컴컴컴컴컴컴컴�           �
  �                                                             �
  � Parameter /? zeigt diese Zeilen an                          �
  읕컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴컴�
*/
"@echo off"

/* Dateien in WPSAC */
/* \OS2\DLL-Verzeichnis */
os.1 = wpsac.dll
os.2 = authoriz.dll
os.0 = 2

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
if DeskmanIcons='DESKMANICONS' then
  DeskmanIcons = 'c:\devtech\ico'
infile = drive'\config.old'
tmpfile= drive'\config.tmp'
outfile= drive'\config.sys'

/* Dateien l봲chen */
trace ?i
Say ""
Say "Dateien l봲chen"
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
  say drive'\OS2\DLL\'os.i 'l봲chen!'
  'mreplace' drive'\OS2\DLL\'os.i '>NUL'
  '@delete' drive'\OS2\DLL\'os.i '>NUL'
end

/* WPStartup2 deregistrieren */
Say ""
Say "WPS-Klasse deregistrieren"
Say ""
 
RetCode = SysDeregisterObjectClass( "WPStartup2" )
if RetCode then
  say 'Deregister successfully completed'
else
  say 'Could not deregister classes'

/* WPSac deregistrieren */
Say ""
Say "WPS-Klasse deregistrieren"
Say ""
 
QualPath'\MAGclass DEREGISTER'
if RC>0 then
  do
  Say "Fehler beim Registrieren der Klasse WPSac! Programm beenden (J/N)?"
  pull answer
  if translate(answer)='J' then
    Exit
  end

beep(3000,100)
beep(2832,100)
beep(3000,100)

'@delete 'QualPath'\SAMMY.EXE >NUL'

/* WPSac aus der OS2SYS.INI l봲chen */
OldPath = SysIni(SYSTEM,AppName,'DELETE:')

/* config.sys 꼗dern */
Say "CONFIG.SYS 꼗dern; Originaldatei wird in CONFIG.BAK abgelegt"
'copy' outfile infile
Say "Es wird eine CONFIG.TMP angelegt."
'del' tmpfile
signal on NOTREADY
result = linein(infile, 1, 0)
result = lineout(tmpfile, , 1)
signal off NOTREADY

Merk1 = ''

do until lines(infile)=0
  line = linein(infile)
  select
    when Pos("REM SET RUNWORKPLACE=", TRANSLATE(line))=1 then
      do
      /* Ist RUNWORKPLACE nicht vorhanden, mu� REM entfernt werden */
      if Pos('sammy.exe',line)=0 & Merk1<>'MERK1' then
      Merk1 = substr(line, 5)     /* Originalzeile merken */
      end
    when Pos("SET RUNWORKPLACE=", TRANSLATE(line))=1 then
      do
      /* Ist RUNWORKPLACE nicht SAMMY.EXE, mu� installiert werden */
      if Pos('sammy.exe',line)=0 then
        do
        if Merk1='MERK1' then
          do
          say 'RUNWORKPLACE nicht ber갷ksichtigt, da bereits vorhanden!'
          iterate
          end
        else
          do
          result = lineout(tmpfile,line)
          drop Merk1              /* wird nicht mehr ben봳igt */
          end
        end
      else
        iterate                   /* nicht schreiben / n꼊hste Zeile */
      end
    when Pos("SET SAMWORKPLACE=", TRANSLATE(line)) then
      do
      if Merk1='MERK1' then
        do
        say 'SAMWORKPLACE nicht ber갷ksichtigt, da RUNWORKPLACE bereits vorhanden!'
        iterate
        end
      else
        do
        result = lineout(tmpfile,overlay('RUN', line, 5))
        drop Merk1                /* wird nicht mehr ben봳igt */
        end
      end
    when Pos("SET HELP=", TRANSLATE(line))=1 then
      do
      /* Help-Path mu� evtl. Eintrag entfernt werden */
      loc = Pos(QualPath,line)
      if loc=0 then
        result = lineout(tmpfile,line)
      else
        result = lineout(tmpfile, delstr(line, loc, length(QualPath||';')))
      end
    when Pos("LIBPATH=", TRANSLATE(line))=1 then
      do
      /* Entfernen des DLL-Pfades auf die WPS-Klasse */
      loc = Pos(QualPath,line)
      if loc=0 then
        result = lineout(tmpfile,line)
      else
        result = lineout(tmpfile, delstr(line, loc, length(QualPath||';')))
      end
    otherwise
      result = lineout(tmpfile,line)
  end
end
/* RUNWORKPLACE und SAMWORKPLACE nicht vorhanden */

if Merk1<>'' & Merk1<>'MERK1' then
  do
  result = lineout(tmpfile,Merk1)
  Say 'RUNWORKPLACE und SAMWORKPLACE waren nicht vorhanden. CONFIG.SYS 갶erpr갽en.'
  Say "Der Inhalt der Datei CONFIG.SYS ist nicht konsistent. Es sollte gegebenenfalls"
  Say "die Backup-Datei CONFIG.OLD-Datei zur갷kgesichert werden!"
  Say "Deinstallation beenden? (J/N)"
  pull answer
  if translate(answer)='J' then
    Exit
  end

Say "CONFIG.SYS wird nun aus CONFIG.TMP erzeugt!"
'copy' tmpfile outfile
Say "Die CONFIG.TMP wird nun gel봲cht!"
'del' tmpfile

Say ""
Say "Vor der Benutzung von WPSac den Rechner neu starten"
Say ""

Exit
/* Fehlerbehandlung */
NOTREADY:
parse value sourceline(sigl) with . "(" file ","
Say "Fehler beim 셟fnen von "value(file)
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
    say 'Weiter --> eine Taste dr갷ken . . .'
    rc=SysCurPos( zeile-2, 0 )
    rc=SysGetKey( 'Noecho' )
  end
  say left( line, spalte-1 )
end
Exit
