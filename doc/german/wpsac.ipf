.*----------------------------------------------------------
.* WPSac -- Workplace Shell Access Control
.*
.* IPF Dokumentation
.*
.* Version 4.0 - 1999
.*----------------------------------------------------------
:userdoc.
:title.WPSac
:docprof toc=123.

.*----------------------------------------------------------
.*  WPSac (Einleitungsseite)
.*----------------------------------------------------------
:h1.WPSac
:lines align=center.
:artwork align=center name='WPSAC.BMP'.

:font facename=Helv size=24x24.
WPSac:font. Version 4.0

der :font facename=Helv size=24x24.Workplace Zugriffsschutz:font.

(C) :artwork align=center name='nb.BMP' runin. noller &amp. breining software, 1995 ... 1999

Internet&colon. klaus.breining@t-online.de
Fido-net&colon. 2&colon.246/1406
Telefon&colon. (49)7191-960345
Fax&colon.(49)7191-300472

    * * * * * * * * * * * * * * * * * * * * * * * * *
:elines.

.*----------------------------------------------------------
.*  Hinweis
.*----------------------------------------------------------
:h1 id=BUGBUG.ACHTUNG&colon. wichtiger Hinweis fÅr Warp-Benutzer
:p.:hp8.Die Workplace-Shell von OS/2 Warp 3.0/4.0 hat einen lÑstigen Fehler&colon.
:p.Werden die Namen der ArbeitsoberflÑche inklusive ihrer Pfade ab dem Stammverzeichnis zu lang, stÅrzt
die OberflÑche beim Starten ab.
Liegen beispielsweise die ArbeitsoberflÑchen unter :font facename=Courier size=14x14.\WPSAC\BENUTZER:font size=0x0., so darf
der Name einer ArbeitsoberflÑche in der deutschen Version nur noch maximal 10 Zeichen lang werden!
:ehp8.

.*----------------------------------------------------------
.*  Einleitung
.*----------------------------------------------------------
:h1.Einleitung

:p.:hp2.WPSac:ehp2. ist ein Zugriffsschutz fÅr die Workplace-Shell von OS/2.
Es gestattet die Verwaltung durch einen Administrator, der die einzelnen Berechtigungen vergibt und
einrichtet.
Jeder Benutzer des Rechners kann dazu eine eigene ArbeitsoberflÑche bekommen, es
kînnen jedoch verschiedene OberflÑchen von mehreren Benutzern gemeinsam genutzt werden.

:p.Anwendungen fÅr :hp2.WPSac:ehp2. gibt es viele. Im einfachsten Fall kann er dazu eingesetzt werden,
auf einem Einzelplatzsystem verschiedene ArbeitsoberflÑchen fÅr unterschiedliche Benutzer zur VerfÅgung
zu stellen. Hierzu ist hÑufig kein Zugriffsschutz fÅr die einzelnen Objekte notwendig.

:p.Die Zugriffsberechtigungen auf die Objekte auf den jeweiligen ArbeitsoberflÑchen kînnen individuell fÅr
jeden Benutzer mit Hilfe sogenannter :link reftype=hd refid=UPROFPRF.Zugriffsprofile:elink. verwaltet werden.
Eine Zusammenfassung mehrerer Benutzer zu Gruppen ist ebenfalls mîglich.

:p.Zweck der Zugriffsprofile ist es, einige Funktionen der Objekte auf den
ArbeitsoberflÑchen zu unterbinden, um ein versehentliches Zerstîren durch einen ungeÅbten Benutzer
wirksam zu verhindern.

:p.Schlie·lich kann in Netzwerken ein Administrator das gesamte Aussehen der ArbeitsoberflÑchen von
seinem eigenen Arbeitsplatz aus konfigurieren und Åber das Netz verteilen. énderungen an den
OberflÑchen oder Zugriffsprofilen kînnen auch jederzeit nachtrÑglich erfolgen.

:p.Die einzelnen Benutzer-IDs kînnen durch Kennwîrter geschÅtzt werden, die Kennworteingabe
kann alternativ Åber einen :link reftype=hd refid=MAGSUPP.Magnetkartenleser:elink. bzw.
:link reftype=hd refid=MAGSUPP.Chipkartenleser:elink. erfolgen.

:p.Einige wichtige Merkmale sind&colon.
:ul compact.
:li.Ein grafisches Administrationsprogramm, durch das die Benutzer-IDs, ArbeitsoberflÑchen und Zugriffsprofile
eingerichtet werden.
Jede Benutzer-ID kann ein Pa·wort, ein Zugriffsprofil und ein Gruppenzugriffsprofil besitzen.
:li.Zugriffsprofile kînnen alternativ Åber eine Beschreibungsdatei im Klartext erstellt und mit Hilfe eines speziellen
Compilers in ein maschinenlesbares Format konvertiert werden oder grafisch mit Hilfe der Administrationsprogrammes
erstellt werden.
:li.Die Administration des Benutzers kann vom Administrator Åber ein LAN durchgefÅhrt werden.
Notwendig ist dazu nur ein IBM LAN-Server oder ein Novell Server.
.*:li.Alle (erfolgreichen und erfolglosen) AnmeldevorgÑnge werden (unmanipulierbar) Åber Netzwerk auf
.*einem dafÅr vorgesehenen Rechner protokolliert.
:li.FÑllt das Netzwerk aus, ist jeder Rechner mit dem letzten Stand der Zugriffsrechte stand-alone bedienbar.
:li.Das Anmelden eines Benutzers kann gleichzeitig auch AnmeldevorgÑnge fÅr Netzwerke oder Datenbanken
auslîsen (:link reftype=hd refid=UPROFLOG.single signon:elink. feature).
:eul.

.*----------------------------------------------------------
.*  Lieferumfang
.*----------------------------------------------------------
:h1 id=LUMF.Lieferumfang

:p.Das gesamte Programmpaket besteht aus 5 Teilen, den Programmen :hp2.SAMMY.EXE:ehp2.,
:hp2.UPROFILE.EXE:ehp2. und :hp2.WP_PARSE.EXE:ehp2.,
sowie den DLL-Dateien :hp2.WPSAC.DLL:ehp2., :hp2.AUTHORIZ.DLL:ehp2. und :hp2.LSPRELOG.DLL:ehp2..
Der folgende Abschnitt beschreibt in kurzen Worten die Funktion. Das VerstÑndnis der Funktion
ist fÅr den Gebrauch von :hp2.WPSac:ehp2. nicht unbedingt erforderlich. Sie kînnen daher diesen
Abschnitt Åberspringen und sofort mit der :link reftype=hd refid=INSTALL.Installation:elink.
oder, falls :hp2.WPSac:ehp2. bereits installiert ist, mit der
:link reftype=hd refid=DESKTOP.Organisation der ArbeitsoberflÑchen:elink. fortfahren.

:p.:hp7.UPROFILE.EXE:ehp7.
:p.Dieses Programm dient der Administration von Benutzer-IDs, ArbeitsoberflÑchen, Zugriffsprofilen und
Anmeldeskripten.
Es ist ein PM-Programm, das beim Start einen Ordner îffnet, in dem jeder Benutzer durch ein
Symbol reprÑsentiert wird.
In den Einstellungen jedes Benutzers kînnen der Name, die Zugriffsprofile, 
die gewÅnschte ArbeitsoberflÑche sowie ein An- und Abmeldeskript ausgewÑhlt werden.

:p.:hp7.WP_PARSE.EXE:ehp7.
:p.Dieser Compiler kann Zugriffsprofile (:hp1.*.sac - Dateien:ehp1.) aus einer ASCII-Beschreibungsdatei
erzeugen. Zugriffsprofile kînnen auch grafisch mit Hilfe von :hp1.UPROFILE.EXE:ehp1. erstellt werden.

:p.:hp7.SAMMY.EXE:ehp7.
:p.Dieses Programm liefert den Anmeldedialog fÅr den Benutzer beim Start des Systems, sowie
nach einem Abmeldevorgang.
ZusÑtzlich kann von hier aus ein Systemabschlu· durchgefÅhrt werden, auch wenn kein
Benutzer angemeldet ist.

:p.:hp7.WPSac.DLL:ehp7.
:p.Diese DLL-Datei ist eine SOM-Workplace-Shell-Klasse, die die Zugriffsrechte auf die einzelnen Objekte auf
der ArbeitsoberflÑche verwaltet.
Eine korrekt installierte und registrierte Klasse ist erkennbar am zusÑtzlichen Eintrag
:hp1.Logout:ehp1. im SystemmenÅ der ArbeitsoberflÑche.

:p.:hp7.WPStrtup.DLL:ehp7.
:p.Diese DLL-Datei ist eine SOM-Workplace-Shell-Klasse, die einen modifzierten Systemstart-Ordner bereitstellt.
Der mit OS/2 standardmÑ·ig ausgelieferte Systemstart-Ordner hat den Nachteil, da· die in ihm enthaltenen Objekt
nur nach dem ersten Aufruf gestartet werden. Nach einem Logoff und erneutem Logon bleibt sein Inhalt unberÅcksichtigt.
Aus diesem Grund wird dieser modifizierte Systemstart-Ordner mitgeliefert, der seine enthaltenen Objekte nach jedem
Logon startet.

:p.:hp7.AUTHORIZ.DLL:ehp7.
:p.Diese DLL-Datei erhÑlt von :hp2.SAMMY.EXE:ehp2. die Zugriffsdateien in einer binÑren Form
und informiert Åber eine Schnittstelle :hp2.WPSac.DLL:ehp2. jedes Objekt bei einem Zugriff Åber
die Rechte des Benutzers.

:p.:hp7.LSPrelog.DLL:ehp7.
:p.Diese DLL-Datei ist fÅr die Kommunikation mit dem LAN-Server zustÑndig. Diese Kommunikation erfolgt
Åber ein spezielles Verzeichnis, das Åber seinen Alias-Namen angesprochen wird. 

.*----------------------------------------------------------
.*  Installation
.*----------------------------------------------------------
:h1 id=INSTALL.Installation
:p.Die Installation des Programmpaketes erfolgt mit Hilfe des OS/2-Installationsprogrammes :hp2.INSTALL.EXE:ehp2.

:p.Die Installation erfolgt vollstÑndig ohne notwendigen Benutzereingriff, es mÅssen lediglich die entsprechenden
Komponenten und Pfade ausgewÑhlt werden.
Folgende Komponenten stehen zur VerfÅgung&colon.
:ul.
:li.:hp4.WPSac Basiskomponente&colon.:ehp4. Installation aller Dateien von :hp2.WPSac:ehp2. einschlie·lich
des Administrationsprogrammes.
:li.:hp4.Systemstart-Ordner&colon.:ehp4. Installation des :hp2.WPSac:ehp2.-eigenen Systemstart-Ordners.
:li.:hp4.LAN Server support&colon.:ehp4. Installation der :hp2.LSPrelog.DLL:ehp2.-Datei fÅr die LAN Server-Anbindung.
:eul.

:p.Es werden die folgenden Verzeichnisse angelegt&colon.
:ul.
:li.:hp4.Installationsverzeichnis&colon.:ehp4. Das Verzeichnis, in dem das Installationsprogramm sowie die ausfÅhrbaren
Programmteile abgelegt werden.
:li.:hp4.lokale Konfigurationsdateien&colon.:ehp4. Ablageverzeichnis fÅr die INI-Datei des Administrationsprogrammes sowie
der Template-Dateien fÅr Logon-Skripte und der Zugriffsprofile.
:li.:hp4.Pfad fÅr ArbeitsoberflÑchen&colon.:ehp4. Ablageverzeichnis fÅr die ArbeitsoberflÑchen. Der Verzeichnisname sollte so kurz
wie mîglich gewÑhlt werden, da die WPS einen sehr lÑstigen Fehler hat (s. o.).
:eul.

:p.Die folgenden Informationen dienen ausschlie·lich interessierten Anwendern, die sich nÑher
fÅr die Funktion und das Zusammenspiel der einzelnen Programmteile interessieren. WÑhrend
der Installation laufen die folgenden Aktionen ab&colon.

:ul.
:li.Anlegen der Verzeichnisse
:li.Entpacken der Dateien
:li.Registrieren der WPS-Klasse :hp2.WPSac:ehp2.. Dies erfolgt durch ein kleines Programm :hp2.SacClass:ehp2.,
das die Klasse WPSac zuerst registriert, anschlie·end WPObject durch diese Klasse ersetzt.
(:hp2.SacClass:ehp2. kennt 4 Argumente. Eine Kurzanleitung erscheint bei Aufruf des Programmes
ohne Argument.)
:li.Registrieren der WPS-Klasse :hp2.WPStartup2:ehp2., die sich in der Datei :hp2.WPStrtup.dll:ehp2. befindet.
:li.éndern der CONFIG.SYS-Datei. Hier wird SAMMY.EXE in die RUNWORKPLACE-Umgebungsvariable eingetragen.
Der alte Wert von RUNWORKPLACE wird in einer Variable SAMWORKPLACE abgelegt, der von :hp2.SAMMY.EXE:ehp2.
benîtigt wird.
:li.Aufruf des Programmes :hp2.UPROFILE.EXE:ehp2.. :hp8.Wichtig&colon.:ehp8. Im MenÅ :hp1.Setup:ehp1. &cdqf. :hp1.Files:ehp1.
mÅssen die Pfade eingetragen werden. Diese Informationen werden auch von :hp2.SAMMY.EXE:ehp2. bei der Suche nach
ArbeitsoberflÑchen benutzt.
:eul.

:p.Nach der Installation von :hp2.WPSac:ehp2. steht ein Default-Benutzer mit dem Namen :hp1.Benutzer:ehp1. und dem Kennwort
:hp1.Kennwort:ehp1. zur VerfÅgung.

.*----------------------------------------------------------
.*  Deinstallation
.*----------------------------------------------------------
:h1 id=DEINST.Deinstallation

:p.Bei der Deinstallation von :hp2.WPSac:ehp2. sind einige Punkte zu beachten,
wenn die StammarbeitsoberflÑche hinterher weiterverwendet werden soll
und kein Workplaceshell-Backup vor der Installation des Paketes
gemacht wurde oder dieser nicht verwendet werden soll. Zur Deinstallation sollte das Installationsprogramm
verwendet werden. 
:p. Bei einer manuellen Deinstallation ist wie folgt vorzugehen&colon.
:ol.
:li.Mit der Installationsdiskette booten und die Datei :hp2.AUTHORIZ.DLL:ehp2. lîschen.
:li.Den Rechner erneut booten und einen Benutzer starten, der die StammarbeitsoberflÑche nutzt.
:li.Alle Ordner îffnen und wieder schlie·en. Dies entfernt die nicht mehr benîtigten Instanzvariablen, die fÅr
die Zugriffsprofile zustÑndig waren.
:li.Die Datei :hp2.CONFIG.SYS:ehp2. wiederherstellen (die Originaldatei wurde als :hp2.CONFIG.BAK:ehp2.
abgelegt).
:li.Das Programm :hp2.SacClass:ehp2. wie folgt aufrufen&colon.
:cgraphic.SACCLASS DEREGISTER:ecgraphic.
:li.Die Klasse :hp2.WPStartup2:ehp2. deregistrieren, z. B. mit folgender REXX-Datei&colon.
:cgraphic.
/* Register class WPMag */
Call RxFuncadd 'SysLoadFuncs', 'RexxUtil', 'SysLoadFuncs'
Call SysLoadFuncs
Call SysDeregisterObjectClass "WPStartup2"
:ecgraphic.
:li.Den Rechner neu booten und die Dateien :hp2.WPSAC.DLL:ehp2. und :hp2.WPSTRTUP.DLL:ehp2. lîschen.
:eol.
:p.Das Deinstallationsprogramm von :hp2.WPSac:ehp2. entfernt die Instanzvariable nicht. Sie stîren den Betrieb
von OS/2 jedoch in keiner Weise.

.*----------------------------------------------------------
.*  Organisation der ArbeitsoberflÑchen
.*----------------------------------------------------------
:h1 id=DESKTOP.Organisation der ArbeitsoberflÑchen
:p.Zuerst ein paar Worte, die die Organisation der ArbeitsoberflÑchen betreffen&colon.
Die ArbeitsoberflÑche, die der Rechner vor der Installation des Programmpaketes verwendete,
wird im folgenden :link reftype=hd refid=DEFS.StammarbeitsoberflÑche:elink. genannt.

:p.Weitere ArbeitsoberflÑchen werden bei Bedarf von :hp2.SAMMY.EXE:ehp2. automatisch
in das in :hp2.UPROFILE.EXE:ehp2. angegebene Verzeichnis angelegt.
Nach Registrieren der neuen Klasse WPSac erhÑlt das SystemmenÅ jeder ArbeitsoberflÑche
einen neuen Punkt Logout.

:p.Jedem Benutzer wird (in :hp2.UPROFILE.EXE:ehp2.) eine ArbeitsoberflÑche zugewiesen.
Jede ArbeitsoberflÑche kann von mehr als einem Benutzer benutzt werden.

:p.:link reftype=hd refid=DEFS.Zugriffsprofile:elink. regeln die Berechtigungen des einzelnen Benutzers.
Hier wird definiert, welche Aktionen er mit den Objekten durchfÅhren darf und welche nicht.
Jedes Zugriffsprofil kann von mehreren Benutzern verwendet werden.

:p.:hp1.ArbeitsoberflÑchen und Zugriffsprofile werden von :ehp1.:hp2.UPROFILE.EXE:ehp2.:hp1.
getrennt verwaltet und sind beliebig mischbar:ehp1., siehe folgendes Beispiel&colon.

:cgraphic.
 ⁄ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø      ⁄ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø      ⁄ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø
 ≥ArbeitsoberflÑche 1≥      ≥ArbeitsoberflÑche 2≥      ≥ArbeitsoberflÑche 3≥
 ¿ƒƒƒƒƒƒƒƒƒ¬ƒƒƒƒƒƒƒƒƒŸ      ¿ƒƒƒƒƒƒƒƒƒ¬ƒƒƒƒƒƒƒƒƒŸ      ¿ƒƒƒƒƒƒƒƒƒ¬ƒƒƒƒƒƒƒƒƒŸ
           ≥                          ≥                          ≥
      ⁄ƒƒƒƒ¡ƒƒƒƒƒƒƒƒƒƒƒø              √ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒø         ¿ƒƒƒƒø
                                                                  
 ⁄ƒƒƒƒƒƒƒƒƒƒø    ⁄ƒƒƒƒƒƒƒƒƒƒø    ⁄ƒƒƒƒƒƒƒƒƒƒø    ⁄ƒƒƒƒƒƒƒƒƒƒø    ⁄ƒƒƒƒƒƒƒƒƒƒø
 ≥Benutzer 1≥    ≥Benutzer 2≥    ≥Benutzer 3≥    ≥Benutzer 4≥    ≥Benutzer 5≥
 ¿ƒƒƒƒƒƒƒƒƒƒŸ    ¿ƒƒƒƒƒƒƒƒƒƒŸ    ¿ƒƒƒƒƒƒƒƒƒƒŸ    ¿ƒƒƒƒƒƒƒƒƒƒŸ    ¿ƒƒƒƒƒƒƒƒƒƒŸ
                                                                  
      √ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ¡ƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ⁄ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒŸ              ¿ø
 ⁄ƒƒƒƒ¡ƒƒƒø                      ⁄ƒƒƒƒƒ¡ƒƒø                        ⁄ƒƒƒ¡ƒƒƒƒø
 ≥Profil 1≥                      ≥Profil 2≥                        ≥Profil 3≥
 ¿ƒƒƒƒƒƒƒƒŸ                      ¿ƒƒƒƒƒƒƒƒŸ                        ¿ƒƒƒƒƒƒƒƒŸ
:ecgraphic.

.*----------------------------------------------------------
.*  NetzwerkunterstÅtzung
.*----------------------------------------------------------
:h1 id=NETWORK.NetzwerkunterstÅtzung
:p.:hp2.WPSac:ehp2. unterstÅtzt die Administration von ArbeitsoberflÑchen Åber
lokale Netzwerke. Dazu mu· vom benutzten Netzwerkprodukt (z. B. Novell Netware)
zur Bootzeit des Rechners ein (zumindest lesbares) Verzeichnis auf dem Server
zur VerfÅgung stehen. :hp2.WPSac:ehp2. versucht dann, alle fÅr die Konfiguration
notwendigen Dateien auf das lokale Verzeichnis umzukopieren. Dieses Verfahren hat
den Vorteil, da· im Falle eines Serverausfalles (oder auch bei vom Netzwerk abgehÑngten
Rechnern) trotzdem eine Anmeldung mit dem beim letzten Logon gÅltigen Administrationsstand
mîglich ist. Die Festlegung der Pfade erfolgt im
:link reftype=hd refid=UPROFFILES.MenÅ :hp1.Einstellung&cdqf.Dateien:ehp1.:elink..

.*----------------------------------------------------------
.*  Benutzung von UPROFILE
.*----------------------------------------------------------
:h1 id=UPROFILE.Benutzung von UPROFILE
:p.Das PM-Programm :hp2.UPROFILE.EXE:ehp2. dient zu Verwaltung von Benutzer-IDs, ArbeitsoberflÑchen,
Zugriffsprofilen und Logon-Prozeduren.

:p.Das Hauptfenster zeigt einen Ordner in der Symboldarstellung, der fÅr jeden eingerichteten
Benutzer ein Icon darstellt.

.*----------------------------------------------------------
.*  Einrichten von UPROFILE
.*----------------------------------------------------------
:h2 id=UPROFINST.Einrichten von UPROFILE
:p.Die nachfolgenden Seiten beschreiben die Konfiguration von UPROFILE, die fÅr dessen
Benutzung notwendig ist. Daher sollten die Angaben der im folgenden beschriebenen Einstellungs-Dialog
alle eingegeben werden.

.*  Starten von UPROFILE
:h3 clear scroll=none id=UPROFSTART.Starten von UPROFILE
:link reftype=hd res=002 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=001 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=002 group=11 hide.t1
:p.Wird :hp2.UPROFILE.EXE:ehp2. das erste Mal gestartet, fragt das System (in englisch) nach dem Pfad
fÅr die zugeordnete INI-Datei. In das Eingabefeld dieser Maske mu· ein vollstÑndiger Dateiname
(Laufwerk, Pfad und Dateiname) angegeben werden.

:p.Nach Eingabe von :hp1.OK:ehp1. erscheint das :link reftype=hd refid=UPROFMAIN.Hauptfenster:elink..

:h3 res=001 group=10 hide.t2
:artwork align=left name='wnd_path.BMP'.
:p.Auswahl der INI-Datei

.*  Hauptfenster
:h3 clear scroll=none id=UPROFMAIN.Hauptfenster
:link reftype=hd res=003 auto split group=11
  vpx=left vpy=bottom vpcx=100% vpcy=30%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=004 auto split group=10
  vpx=left vpy=top vpcx=100% vpcy=70%
  rules=border scroll=both titlebar=none.

:h3 res=003 group=11 hide.t1
:p.Nach dem Start von UPROFILE erscheint das Hauptfenster in Form eines Ordners in Symboldarstellung.
Alle Texte sind nach der Installation in englisch (oberes Bild). Die Umstellung auf die deutsche
Sprache wird auf der nÑchsten Seite erklÑrt.
Das untere Bild zeigt dasselbe Hauptfenster in deutscher Sprache.
:p.Bei der Erstinbetriebnahme sollten zu Beginn die Einstellungen in den Dialogfenstern
:link reftype=hd refid=UPROFOPT.Grundeinstellungen:elink. und
:link reftype=hd refid=UPROFFILES.Dateien:elink. vorgenommen werden.

:h3 res=004 group=10 hide.t2
:p. :artwork runin name='wnd_mainus.BMP'.
    :artwork runin name='wnd_maingr.BMP'.
:p.Hauptfenster (englisch und deutsch)

.* Anmeldefenster
:h3 clear scroll=none id=UPROFMLOG.Anmeldefenster
:link reftype=hd res=005 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=006 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=005 group=11 hide.t1
:p.Dieses Dialogfenster erscheint, wenn UPROFILE auf einem bereits fertig eingerichteten System
gestartet wird und der aktuelle Benutzer kein Administrator ist. In diesem Fall mu· der Benutzername
und das Kennwort eines Benutzers mit Administratorrechten eingegeben werden.

:h3 res=006 group=10 hide.t2
:artwork align=left name='wnd_mlog.bmp'.
:p.Anmeldefenster

.*  Grundeinstellungen
:h3 clear scroll=none id=UPROFOPT.Grundeinstellungen
:link reftype=hd res=007 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=008 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=007 group=11 hide.t1
:p.Dieses Dialogfenster wird geîffnet Åber den MenÅpunkt :hp1.Einstellung&cdqf.Grundeinstellung:ehp1. im
:link reftype=hd refid=UPROFMAIN.Hauptfenster:elink..
Hier werden systemweite Einstellungen fÅr das Produkt :hp2.WPSac:ehp2. vorgenommen.

:p.Dies sind im einzelnen&colon.
:ul compact.
:li.:hp5.Sprache&colon.:ehp5. Diese Auswahlknîpfe legen die Sprache fest, in der :hp2.UPROFILE:ehp2. nach
dem :hp4.nÑchsten:ehp4. Start erscheint.
:li.:hp5.Erzeugung von Duplikaten&colon.:ehp5. Der Systemadministrator ist in der Lage, einer ArbeitsoberflÑche
zusÑtzliche Symbole hinzuzufÅgen. Mit den Auswahlknîpfen in dieser Gruppe wird festgelegt, was passiert, wenn
ein angegebenes Symbol bereits existiert&colon.
  :ul compact.
  :li.:hp5.Objekt beibehalten:ehp5. ignoriert das neu angegebene Symbol.
  :li.:hp5.Objekt ersetzen:ehp5. lîscht das bereits existierende Objekt, bevor das neue angelegt wird.
  :li.:hp5.Objekt aktualisieren:ehp5. Åbernimmt die Einstellungen des neuen Objektes fÅr das alte.
  :eul.
:li.:hp5.Auf Netzwerkserver warten&colon.:ehp5. Wird der :hp1.Ja:ehp1.-Knopf gedrÅckt, legt die eingestellte
Zeit fest, wie lange :hp2.SAMMY.EXE:ehp2. wartet, bis eine Verbindung zum Netzwerkserver verfÅgbar ist, um
die :hp2.WPSac:ehp2.-Konfigurationsdatei zu lesen. Ist dies nicht innerhalb der angegebenen Zeit mîglich, wird
die lokale Kopie dieser Datei (:hp1.UPROFILE.INI:ehp1.) verwendet.
:li.:hp5.RC-Datei fÅr BasisoberflÑche&colon.:ehp5. Wird eine ArbeitsoberflÑche zum ersten Mal verwendet, mu·
sie zuerst generiert werden. Dies ist Aufgabe eines Programmes, das zum OS/2-Lieferumgang gehîrt&colon.
:hp2.MAKEINI:ehp2.. Dieses Programm verwendet eine sogenannte Resource-Datei (Endung :hp1..rc:ehp1.) als
Eingangsdatei und erzeugt daraus eine Profildatei (Endung :hp1..ini :ehp1.). StandardmÑ·ig wird hierfÅr
eine Datei verwendet, die nach einer Installation von OS/2 im Verzeichnis :hp1.OS2:ehp1. unter dem Namen
:hp1.INI.RC:ehp1. abgelegt wurde (bei leerem Eingabefeld). Diese Datei sollte (mit vollstÑndiger Pfadangabe) in diesem Feld
angegeben werden. SelbstverstÑndlich kînnen sich OS/2-Profis diese Datei an eigene Anforderungen anpassen!
:p.Beispiel&colon. :font facename=Courier size=14x14.C&colon.\OS2\INI.RC:font size=0x0.
:eul.

:h3 res=008 group=10 hide.t2
:artwork align=left name='wnd_setup.bmp'.
:p.Grundeinstellungen

.*  Dateien
:h3 clear scroll=none id=UPROFFILES.Dateien
:link reftype=hd res=009 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=010 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=009 group=11 hide.t1
:p.Dieses Dialogfenster wird geîffnet Åber den MenÅpunkt :hp1.Einstellung&cdqf.Dateien:ehp1. im
:link reftype=hd refid=UPROFMAIN.Hauptfenster:elink..
Hier werden die Pfade fÅr Dateien ausgewÑhlt, die fÅr den Betrieb von :hp2.WPSac:ehp2. notwendig sind.

:p.Dies sind im einzelnen&colon.
:ul compact.
:li.:hp5.Pfad fÅr Administrationszwecke&colon.:ehp5. Dieser Pfad definiert das Verzeichnis, in dem
die Administrationsdateien von :hp1.UPROFILE.EXE:ehp1. abgelegt werden. Dies sind die INI-Datei,
die :link reftype=hd refid=DEFS.Zugriffsprofile:elink., sowie die Dateien zur
:link reftype=hd refid=DEFS.énderungsanforderung:elink..
:note Text='Achtung:'.Wird UPROFILE in einer LAN-Umgebung auf einem lokalen Rechner verwendet, 
mu· an dieser Stelle der Pfad eingegeben werden, in dem die Konfigurationsdateien auf dem Server liegen.
Dies kann auch in der UNC-Notation 
(:font facename=Courier size=14x14.\\server\path:font size=0x0.) angegeben werden. Wichtig ist, da· die
Dateien in diesem Verzeichnis schreibbar ist. Daher sollte der Administrator vor dem Aufruf von 
UPROFILE sich selbst auf dem Netzwerk einloggen! 
:p.Beispiel&colon. :font facename=Courier size=14x14.C&colon.\WPSAC:font size=0x0.:p.
:li.:hp5.Pfad fÅr die ArbeitsoberflÑchen&colon.:ehp5. Hier wird angegeben, in welchem Verzeichnis
die ArbeitsoberflÑchen abgelegt werden. Bei der Auswahl eines Pfades ist jedoch der bereits erwÑhnte
:link reftype=hd refid=BUGBUG.Fehler in der WPS:elink. zu beachten!
:p.Beispiel&colon. :font facename=Courier size=14x14.C&colon.\WPSAC\DT:font size=0x0.:p.
:li.:hp5.Pfad fÅr die lokale Ablage&colon.:ehp5. Hier wird der Pfad angegeben, in dem :hp2.WPSac:ehp2.
Dateien (Zugriffsprofile, énderungsanforderungen, INI-Datei) lokal sucht.
:p.Beispiel&colon. :font facename=Courier size=14x14.C&colon.\WPSAC:font size=0x0.:p.
:li.:hp5.Pfad fÅr Ablage auf Server&colon.:ehp5. Dieser Pfad beschreibt den Netzwerk-Pfad,
in dem :hp2.WPSac:ehp2. die Dateien sucht.
:p.Beispiel&colon. :font facename=Courier size=14x14.N&colon.\WPSac:font size=0x0.:p.
:li.:hp5.Standard-Pfad fÅr Deskman-Icons&colon.:ehp5. Dieser Pfad gibt an, auf welchem Pfad auf der
:hp5.Administrationsmaschine:ehp5. :link reftype=fn refid=DESKMAN.DESKMAN/2:elink. seine Icons ablegt. Dieser
Pfad mu· auf der lokalen Maschine NICHT existieren!
:p.Beispiel&colon. :font facename=Courier size=14x14.N&colon.\Devtech\ico:font size=0x0.:p.
:eul.

:h3 res=010 group=10 hide.t2
:artwork align=left name='wnd_files.bmp'.
:p.Pfadeinstellungen

.*  Server-Zugang
:h3 clear scroll=none id=UPROFSERVACC.Server-Zugang
:link reftype=hd res=042 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=043 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=042 group=11 hide.t1
:p.Dieses Dialogfenster wird geîffnet Åber den MenÅpunkt :hp1.Einstellung&cdqf.Server-Zugang:ehp1. im
:link reftype=hd refid=UPROFMAIN.Hauptfenster:elink..
Hier wird der Zugang zum Server eingestellt. Voraussetzung ist, da· im Fenster
:link reftype=hd refid=UPROFFILES.Dateien:elink. das Eingabefeld :hp2.Pfad fÅr Ablage auf Server:ehp2.
ausgefÅllt ist.

:p.Im oberen Feld :hp2.Typ des Servers:ehp2. mu· zuerst der Netzwerktyp selektiert werden. Aktuell
wird 
:ul.
:li.:hp2.Novell Netware:ehp2. und 
:li.:hp2.IBM LAN Server:ehp2.
:eul.
:p.unterstÅtzt.

:p.Im zweiten Fall mÅssen die unteren 3 Eingabefelder ausgefÅllt werden. Im Feld :hp2.DomÑne:ehp2. 
mu· in diesem Fall der String :hp5.LS&colon.:ehp5. dem DomÑnennamen vorangestellt werden. 
Diese Buchstaben selektieren die korrekte Zugriffs-DLL (:hp1.LSPrelog.dll:ehp1.).

:h3 res=043 group=10 hide.t2
:artwork align=left name='wnd_lan.bmp'.
:p.Server-Zugang

.*  Pa·wort
:h3 clear scroll=none id=UPROFPWD.Pa·wort
:link reftype=hd res=030 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=031 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=030 group=11 hide.t1
:p.Dieses Dialogfenster wird geîffnet Åber den MenÅpunkt :hp1.Einstellung&cdqf.Pa·wort:ehp1. im
:link reftype=hd refid=UPROFMAIN.Hauptfenster:elink..
Hier werden systemweite Einstellungen fÅr das Verhalten der Pa·wîrter von :hp2.WPSac:ehp2. vorgenommen.

:p.Dies sind im einzelnen&colon.
:ul compact.
:li.:hp5.Pa·wortwechsel erzwingen&colon.:ehp5. Hier wird festgelegt, ob die Logon-Pa·wîrter unbegrenzte GÅltigkeit
besitzen, oder ob das System nach einer einstellbaren Zeit die Eingabe eines neuen Pa·wortes verlangt.
:li.:hp5.Kennwort und Karte erforderlich&colon.:ehp5. Dieser Knopf legt fest, wie bei Vorhandensein eines
:link reftype=hd refid=MAGSUPP.Magnetkarten-:elink. bzw. :link reftype=hd refid=MAGSUPP.Chipkartenlesers:elink.
die Authentisierung funktioniert. Ist der Knopf nicht ausgewÑhlt, kann das Pa·wort alternativ durch Tastatur oder
eine ID-Karte eingegeben werden, im anderen Fall sind beide Eingabemethoden erforderlich.
:eul.

:h3 res=031 group=10 hide.t2
:artwork align=left name='wnd_pwd.bmp'.
:p.Pa·wort


.*----------------------------------------------------------
.*  Anlegen der ArbeitsoberflÑchen
.*----------------------------------------------------------
:h2 clear scroll=none id=UPROFWPS.Anlegen der ArbeitsoberflÑchen
:link reftype=hd res=011 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=012 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h2 res=011 group=11 hide.t1
:p.Nach der Einrichtung von :hp2.UPROFILE:ehp2., so wie im vorhergehenden Abschnitt beschrieben wurde,
werden am sinnvollsten die ArbeitsoberflÑchen definiert, da diese vîllig unabhÑngig von der Definition
von Zugriffsprofilen, Benutzern und Logon-Prozeduren sind. Die Einrichtung von ArbeitsoberflÑchen ist
jedoch nur notwendig, wenn au·er der :link reftype=hd refid=DEFS.StammarbeitsoberflÑche:elink. weitere
OberflÑchen verwendet werden sollen (also die Benutzer :hp5.unterschiedliche:ehp5. ArbeitsoberflÑchen benÅtzen
sollen.

:p.Das Dialogfenster zum Anlegen und Lîschen von ArbeitsoberflÑchen wird geîffnet Åber den MenÅpunkt
:hp1.Einstellung&cdqf.ArbeitsoberflÑchen:ehp1. im
:link reftype=hd refid=UPROFMAIN.Hauptfenster:elink..

:p.Um eine neue ArbeitsoberflÑche anzulegen, wird im Feld :hp1.Neue OberflÑche:ehp1. ein (noch nicht
existierender) Name eingegeben. Im Feld :hp1.Benutzte dRC-Datei:ehp1. kann der Name einer
:link reftype=hd refid=DEFS.énderungsanforderungs-Datei:elink. angegeben werden (Dateien, die
von :link reftype=fn refid=DESKMAN.DESKMAN/2:elink. erzeugt wurden, haben die Endung :hp1..DRC:ehp1. ).
Ein Druck auf den Knopf :hp1.Einrichten>>:ehp1. fÅgt die neue ArbeitsoberflÑche der rechts danebenstehenden
Liste zu.

:p.Beim nÑchsten Start einer neu angelegten ArbeitsoberflÑche wird fÅr diese (von :hp2.SAMMY:ehp2.) ein
Unterverzeichnis in dem unter :link reftype=hd refid=UPROFFILES.Pfad fÅr ArbeitsoberflÑchen:elink.
angegebenen Pfad angelegt. Dieser enthÑlt die Benutzerprofildatei (:hp1.OS2.INI:ehp1.) und den
Ordner der ArbeitsoberflÑche.

:h2 res=012 group=10 hide.t2
:artwork align=left name='wnd_ao.BMP'.
:p.Anlegen der ArbeitsoberflÑchen

.*----------------------------------------------------------
.*  Anlegen der Zugriffsprofile
.*----------------------------------------------------------
:h2 id=UPROFPRF.Anlegen der Zugriffsprofile
:p.Zugriffsprofile legen fest, welche Aktionen ein Benutzer mit einem Symbol durchfÅhren darf.
Das Dialogfenster zur Verwaltung der Zugriffsprofile erreicht man Åber den MenÅpunkt
:hp1.Einstellung&cdqf.Zugriffsprofile:ehp1. im
:link reftype=hd refid=UPROFMAIN.Hauptfenster:elink..

.*  Auswahldialog
:h3 clear scroll=none.Auswahldialog
:link reftype=hd res=013 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=014 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=013 group=11 hide.t1
:p.Um ein neues Zugriffsprofil anzulegen, mu· in das Eingabefeld :hp1.Auswahl der Zugriffsprofile:ehp1.
ein Name angegeben und der :hp1.Neu:ehp1.-Knopf betÑtigt werden.
Es erscheint der im folgenden Abschnitt beschriebene Ordner.

:h3 res=014 group=10 hide.t2
:artwork align=left name='wnd_prof.bmp'.
:p.Auswahldialog

.*  Anlegen der Zugriffsprofile
:h3 clear scroll=none id=UPROFTITLE.Anlegen der Zugriffsprofile
:link reftype=hd res=015 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=016 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=015 group=11 hide.t1
:p.Der dargestellte Ordner zeigt die Detailanzeige der Liste von Objekten mit modifizierten Zugriffsrechten.
Sie enthÑlt die Spalten :hp1.Titel:ehp1., :hp1.Klasse:ehp1., :hp1.Typ:ehp1., :hp1.Default-Titel:ehp1.
und :hp1.Physikalischer Name:ehp1..
Im einzelnen bedeuten diese :link reftype=hd refid=TYPE.Begriffe:elink.&colon.
:dl tsize=15.
:dt.Titel
:dd.Bezeichnet den Titel eines Objektes. Dies ist der Text, der unter dem entsprechenden Symbol auf der
ArbeitsoberflÑche steht.
:dt.Klasse
:dd.Bezeichnet die Klasse, zu der das Objekt gehîrt. Hierzu gehîren Klassennamen wie :hp1.WPProgram:ehp1.,
:hp1.WPData:ehp1. oder :hp1.WPFolder:ehp1.. Klassennamen mÅssen den entsprechenden ProgrammierhandbÅchern
entnommen werden.
:dt.Typ
:dd.Bezeichnet den im :hp1.TYPE:ehp1.-Attribut des Objektes angegebenen String. Beispielsweise legt der
Systemeditor Dateiobjekte mit den Typen :hp1.Plain Text:ehp1., :hp1.OS/2 Command File:ehp1. und
:hp1.DOS Command File:ehp1. an. Weitere Typen sind beispielsweise :hp1.Binary Data:ehp1., :hp1.Bitmap:ehp1. oder
:hp1.Executable:ehp1.. Der Typ kann bei Dateiobjekten im Einstellungs-Notizbuch auf der Seite :hp1.Typ:ehp1.
abgefragt werden.
:dt.Default-Titel
:dd.Der Default-Titel ist ein fest in einer Klasse vordefinierter Titel, der vom System verwendet wird, wenn
kein anderer Titel angegeben wird. Beispielsweise ist der Default-Titel fÅr ein Druckersymbol
:hp1.Druckerobjekt:ehp1..
:dt.Physik. Name
:dd.Der physikalische Name eines Objektes entspricht dem vollstÑndigen Pfad der zugehîrigen Datei auf dem
DatentrÑger.
:edl.

:p.ProfileintrÑge kînnen auf zwei verschiedene Arten angelegt werden&colon.
:ol compact.
:li.Durch einen Klick mit der rechten Maustaste auf den Ordnerhintergrund erscheint ein KontextmenÅ mit dem
Eintrag :hp1.Neu:ehp1.. Dessen Auswahl îffnet das zugehîrige Einstellungsnotizbuch
(siehe :link reftype=hd refid=UPROFPRFSET.nÑchster Abschnitt:elink.). Die Eingabefelder mÅssen dann
von Hand gefÅllt werden.
:li.Durch Drag&amp.Drop eines Objektes auf den Ordner erscheint ein neuer Eintrag, dessen Felder
:hp1.Titel:ehp1. und :hp1.Physikalischer Name:ehp1. bereits gefÅllt sind. Durch Auswahl des MenÅpunktes
:hp1.ôffnen:ehp1. des zugehîrigen KontextmenÅs erscheint das Einstellungsnotizbuch.
:eol.

:h3 res=016 group=10 hide.t2
:artwork align=left name='cont_prof.bmp'.
:p.Anlegen der Zugriffsprofile

.*  Editieren eines Profileintrages
:h3 clear scroll=none id=UPROFPRFSET.Editieren eines Profileintrages
:link reftype=hd res=017 auto split group=11
  vpx=left vpy=top vpcx=35% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=018 auto split group=10
  vpx=right vpy=top vpcx=65% vpcy=100%
  rules=border scroll=both titlebar=none.

:h3 res=017 group=11 hide.t1
:p.Die folgenden Seiten zeigen die 4 verschiedenen Seiten des Einstellungsnotizbuches eines Profileintrages.

:p.:link reftype=hd res=018 split group=10
vpx=right vpy=top vpcx=65% vpcy=100%
rules=border scroll=both titlebar=none.Bild 1:elink. zeigt die Einstellungsseite fÅr den Objekt-Titel.
Auf dieser Seite gibt es fÅr jeden der im Kapitel
:link reftype=hd refid=UPROFTITLE.Anlegen der Zugriffsprofile:elink.
Begriffe ein Eingabefeld. In diesen Feldern kînnen auch die aus normalen Datei-Suchoperationen bekannten
Wildcards '?' und '*' verwendet werden.

:p.:link reftype=hd res=019 split group=10
vpx=right vpy=top vpcx=65% vpcy=100%
rules=border scroll=both titlebar=none.Bild 2:elink. zeigt die Einstellungsseite fÅr den Objekt-Stil. Jedes Stil-Flag
kann einen der drei Werte :hp1.Beibehalten:ehp1. (wird durch den Default der Workplace-Shell definiert),
:hp1.Verbieten:ehp1. oder :hp1.Erlauben:ehp1. annehmen.
Die mîglichen Flags sind in der :link reftype=hd refid=FLAGS.Tabelle:elink. erlÑutert. In diesem Bild
ist auch beispielhaft die Darstellung der Auswahlknîpfe im Fall :hp1.Erlauben:ehp1. und :hp1.Verbieten:ehp1.
dargestellt.

:p.:link reftype=hd res=020 split group=10
vpx=right vpy=top vpcx=65% vpcy=100%
rules=border scroll=both titlebar=none.Bild 3:elink. zeigt die Einstellungsseite fÅr das Objekt-Kontext-MenÅ.
Die mîglichen Flags sind in der :link reftype=hd refid=FLAGS.Tabelle:elink. erlÑutert.

:p.:link reftype=hd res=021 split group=10
vpx=right vpy=top vpcx=65% vpcy=100%
rules=border scroll=both titlebar=none.Bild 4:elink. zeigt die Einstellungsseite fÅr weitere Objekt-Flags.
Die mîglichen Flags sind in der :link reftype=hd refid=FLAGS.Tabelle:elink. erlÑutert. 

:h3 res=018 group=10 hide.t2
:p.Bild 1&colon. Objekt-Titel
:artwork align=left name='wnd_prf1.bmp'.

:h3 res=019 group=10 hide.t2
:p.Bild 2&colon. Objekt-Stil
:artwork align=left name='wnd_prf2.bmp'.

:h3 res=020 group=10 hide.t2
:p.Bild 3&colon. Objekt-Context
:artwork align=left name='wnd_prf3.bmp'.

:h3 res=021 group=10 hide.t2
:p.Bild 4&colon. Objekt-Spezial
:artwork align=left name='wnd_prf4.bmp'.

.*----------------------------------------------------------
.*  Flags
.*----------------------------------------------------------
:h3 id=FLAGS.Flags fÅr Zugriffsprofile
:p.Nachfolgend werden die Flags der Zugriffsprofildatei beschrieben&colon.

:p.:table cols='15 14 15 30'.
:row.
:c.Name
:c.Flag
:c.Negation
:c.Beschreibung
:row.
:c.Erstellen
:c.CREATEANOTHER
:c.NOCREATEANOTHER
:c.Objekt erstellen
:row.
:c.Kopieren
:c.COPY
:c.NOCOPY
:c.Kopieren mîglich
:row.
:c.Veschieben
:c.MOVE
:c.NOMOVE
:c.Verschieben mîglich
:row.
:c.Referenz erstellen
:c.LINK
:c.NOLINK
:c.Referenz erstellen mîglich
:row.
:c.Lîschen
:c.DELETE
:c.NODELETE
:c.Objekt lîschbar
:row.
:c.Drucken
:c.PRINT
:c.NOPRINT
:c.Drucken mîglich
:row.
:c.Ziehen (Drag)
:c.DRAG
:c.NODRAG
:c.Ziehen mîglich
:row.
:c.Sichtbarkeit
:c.VISIBLE
:c.NOTVISIBLE
:c.Objekt ist sichtbar
:row.
:c.Einstellung îffnen
:c.SETTINGS
:c.NOSETTINGS
:c.Einstellungsseite aufrufbar
:row.
:c.Umnennen
:c.RENAME
:c.NORENAME
:c.Objekt umbenennbar
:row.
:c.Doppelklick
:c.DBLKLICK
:c.NODBLKLICK
:c.Objekt reagiert auf Doppelklick
:row.
:c.ôffnen
:c.OPEN
:c.NOOPEN
:c.Objekt kann geîffnet werden
:row.
:c.Hilfe
:c.HELP
:c.NOHELP
:c.Online-Hilfe aufrufbar
:row.
:c.Suchen
:c.FIND
:c.NOFIND
:c.Suchprogramm ist anwÑhlbar
:row.
:c.AuswÑhlen
:c.SELECT
:c.NOSELECT
:c.AuswÑhlen ist mîglich
:row.
:c.Sortieren
:c.SORT
:c.NOSORT
:c.Sortieren ist mîglich
:row.
:c.Anordnen
:c.ARRANGE
:c.NOARRANGE
:c.Anordnen ist mîglich
:row.
:c.Fenster
:c.WINDOW
:c.NOWINDOW
:c.Fensterliste aufrufbar
:row.
:c.Schlie·en
:c.CLOSE
:c.NOCLOSE
:c.Objekt kann geschlossen werden.
:row.
:c.Symbolanzeige
:c.ICONVIEW
:c.NOICONVIEW
:c.Symbolanzeige ist mîglich
:row.
:c.Strukturanzeige
:c.TREEVIEW
:c.NOTREEVIEW
:c.Strukturanzeige ist mîglich
:row.
:c.Detailanzeige
:c.DETAILSVIEW
:c.NODETAILSVIEW
:c.Detailanzeige ist mîglich
:row.
:c.Default-Icon
:c.DEFAULTICON
:c.NODEFAULTICON
:c.Default-Icon auswÑhlbar
:row.
:c.Schablone
:c.TEMPLATE
:c.NOTEMPLATE
:c.Objekt kann Schablone werden
:row.
:c.Programm
:c.PROGRAM
:c.NOPROGRAM
:c.Programmzuordnung mîglich
:row.
:c.Palette
:c.PALETTE
:c.NOPALETTE
:c.Paletteneinstellung mîglich
:row.
:c.Sperren
:c.LOCKUP
:c.NOLOCKUP
:c.Sperren ist anwÑhlbar
:row.
:c.Systemabschlu·
:c.SHUTDOWN
:c.NOSHUTDOWN
:c.Systemabschlu· ist anwÑhlbar
:row.
:c.Drop
:c../.
:c../.
:c.Drop des Objektes ist mîglich
:row.
:c.Als Drop-Ziel
:c../.
:c../.
:c.Objekt kann Drop-Ziel werden
:row.
:c.Aufnehmen
:c../.
:c../.
:c.Objekt kann aufgenommen werden
:row.
:c.Ablegen
:c../.
:c../.
:c.Objekt kann abgelegt werden
.*öbrig: ???
:row.
:c.Neu
:c.NEW
:c.NONEW
:c.Neues Objekt anlegbar
:etable.

.*----------------------------------------------------------
.*  Anlegen der Ein-/Auslog-Prozeduren
.*----------------------------------------------------------
:h2 id=UPROFLOG.Anlegen der Ein-/Auslog-Prozeduren
:p.In einer heterogenen LAN-Umgebung ergibt sich heute oft das Problem, da· ein Benutzer
Netzwerkresourcen benîtigt, auf die er erst nach unterschiedlichen Logon-Prozeduren zugreifen
kann. Um diesen Vorgang zu vereinfachen, ermîglicht :hp2.WPSac:ehp2., verschiedene Ein- und
Auslog-VorgÑnge in Prozeduren fÅr jeden Benutzer zu hinterlegen (:hp1."Single Signon":ehp1.).

:p.:hp2.WPSac:ehp2. benutzt fÅr diese Prozeduren REXX-Scripte, die
in den Profildaten von :hp2.WPSac:ehp2. abgelegt werden. Um den Codierungsaufwand bei grî·eren
Installationen gering zu halten, wurde eine zweistufige Technik gewÑhlt&colon.
:ol compact.
:li.Jedem LAN-Produkt (oder auch lokal installierten Produkten, die einen Logon benîtigen, wie
z. B. die Einzelplatzversion von DB2/2) werden zwei REXX-Prozeduren (eine fÅr Logon, eine fÅr Logoff) 
zugeordnet, die im folgenden :hp1.Logon-Prozeduren:ehp1. genannt werden. 
Zur UnterstÅtzung sind hierfÅr drei Variablen definiert&colon.
  :ol compact.
  :li. :hp3.username&colon.:ehp3. wird Åblicherweise fÅr den Benutzernamen verwendet.
  :li. :hp3.password&colon.:ehp3. enthÑlt normalerweise das Pa·wort.
  :li. :hp3.domain&colon.:ehp3. steht bei Multidomain-Installationen fÅr den DomÑnennamen, bei Novell
beispielsweise fÅr den Servernamen.
  :li. :hp3.wpsac_username&colon.:ehp3. steht fÅr den Namen des :hp2.WPSac:ehp2.-Benutzers.
  :eol.
:li.Jeder Benutzer besitzt ein :hp1.Logon-Script:ehp1., das aus mehreren dieser :hp1.Logon-Prozeduren:ehp1. 
bestehen kann.
Es wird jeweils zwischen dem Ein- und Auslogvorgang unterschieden.
:eol.

:p.Das Dialogfenster zur Verwaltung dieser Logon-Prozeduren erreicht man Åber den MenÅpunkt
:hp1.Einstellung&cdqf.Logon-Prozeduren:ehp1. im
:link reftype=hd refid=UPROFMAIN.Hauptfenster:elink..

.*  Auswahldialog
:h3 id=LOGDLG clear scroll=none.Auswahldialog
:link reftype=hd res=120 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=121 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=120 group=11 hide.t1
:p.Dieses Dialogfenster dient zur Auswahl und zur Neuanlage von Logon-Prozeduren. Es sind normalerweise
immer zwei Prozeduren zu erstellen&colon. eine fÅr den Logon-, die andere fÅr den Logoff-Vorgang.
Die Benennung sollte daher so erfolgen, da· die Zuordnung eindeutig ist.

:p.Die Anwahl des :hp1.Neu-:ehp1. oder :hp1.éndern-:ehp1.Knopfes îffnet einen Editor zur Erstellung
der Prozeduren.

:h3 res=121 group=10 hide.t2
:artwork align=left name='wnd_log1.bmp'.
:p.Auswahldialog fÅr Logon-Prozeduren

.*  Prozeduren-Editor
:h3 clear scroll=none.Prozeduren-Editor
:link reftype=hd res=122 auto split group=11
  vpx=left vpy=top vpcx=40% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=123 auto split group=10
  vpx=right vpy=top vpcx=60% vpcy=100%
  rules=border scroll=horizontal titlebar=none.

:h3 res=122 group=11 hide.t1
:p.Der Prozeduren-Editor ist ein einfacher Editor, vergleichbar mit dem Systemeditor. Er dient
zur Erstellung der Logon-/Logoff-Prozeduren, die in REXX formuliert werden. Die Dateien werden
jedoch aus SicherheitsgrÅnden nicht als Befehlsdateien auf der Festplatte abgelegt, sondern
in der INI-Datei von :hp2.UPROFILE:ehp2. abgelegt. (Daher ist die
Kommentarzeile am Beginn der REXX-Datei optional.)

:p.:link reftype=hd res=123 split group=10
vpx=right vpy=top vpcx=60% vpcy=100%
rules=border scroll=horizontal titlebar=none.Bild 1:elink. zeigt beispielhaft die Logon- bzw. Logoff-Prozeduren
fÅr eine lokale DB2/2-Installation. FÅr den Logon ist die Userid und das Pa·wort zu Åbergeben, die
Logoff-Prozedur kommt ohne Parameter aus.

:p.:link reftype=hd res=124 split group=10
vpx=right vpy=top vpcx=60% vpcy=100%
rules=border scroll=horizontal titlebar=none.Bild 2:elink. zeigt die Logon- bzw. Logoff-Prozeduren fÅr
eine LAN-Server Installation mit mehreren DomÑnen. Aus diesem Grund ist beim Logon
ein DomÑnenname mit zu Åbergeben, fÅr den Fall, da· die Anmeldung nicht an der HauptdomÑne erfolgen
soll.

:p.:link reftype=hd res=125 split group=10
vpx=right vpy=top vpcx=60% vpcy=100%
rules=border scroll=horizontal titlebar=none.Bild 3:elink. zeigt schlie·lich ein Beispiel fÅr eine Logon-Prozedur
fÅr Novell Netware. Es ist jedoch zu beachten, da· hier sehr unterschiedliche Logon-Programme von Seiten Novell
existieren, die nicht alle kommandozeilenfÑhig sind.

:h3 res=123 group=10 hide.t2
:artwork align=left name='wnd_log_db2.bmp'.
:artwork align=left name='wnd_loff_db2.bmp'.
:p.Beispielhafte Prozeduren fÅr eine lokale DB2/2-Installation.

:h3 res=124 group=10 hide.t2
:artwork align=left name='wnd_log_ls.bmp'.
:artwork align=left name='wnd_loff_ls.bmp'.
:p.Beispielhafte Prozeduren fÅr eine LAN-Server Installation.

:h3 res=125 group=10 hide.t2
:artwork align=left name='wnd_log_n.bmp'.
:p.Beispielhafte Logon-Prozedur fÅr eine Novell Netware Installation.

.*  Pa·wort-Wechsel
:h3.Pa·wort-Wechsel
:p.Mit Hilfe der Logon-Prozeduren lÑ·t sich der Pa·wortwechsel fÅr die verschiedenen Netzwerkprodukte
automatisieren. Hierzu gibt es einige Zusatzfunktionen als REXX-Erweiterungen&colon.
:ul.
:li.:hp4.SacPasswdAge():ehp4. Diese Funktion gibt das Alter des Pa·wortes in Tagen zurÅck.
Durch einen einfachen Vergleich lÑ·t sich ÅberprÅfen, ob das Alter eine bestimmte Anzahl von Tagen
Åberschritten hat.
:li.:hp4.SacCreatePwd([<pwdlen>[, <alpha>[, <num>]]]):ehp4. Diese Funktion generiert ein neues Pa·wort.
Wird kein Argument angegeben, besteht das Pa·wort aus 6 alphanumerischen Zeichen. Diese Pa·wortlÑnge lÑ·t
sich durch das erste, optionale Argument einstellen. Der zweite Parameter gibt an, wieviele der Zeichen
mindestens Buchstaben sein mÅssen, der dritte gibt die Mindestzahl der Ziffern an. So erzeugt
:cgraphic.newpwd = SacCreatePwd(8, 3, 2):ecgraphic.
ein Pa·wort mit einer LÑnge von 8 Zeichen, das mindestens 3 Buchstaben und 2 Ziffern enthÑlt.
:li.:hp4.SacConfirmPwd():ehp4. Diese Funktion bestÑtigt :hp2.WPSac:ehp2., da· der Pa·wortwechsel im entsprechenden
Netzwerkprodukt erfolgreich vollzogen wurde und da· sich :hp2.WPSac:ehp2. daher das alte Pa·wort nicht mehr merken mu·.
:eul.

:p.Die Strategie eines Pa·wortwechsels ist folgende&colon.
:ol compact.
:li.:hp2.WPSac:ehp2. loggt sich im entsprechenden Netzwerkprodukt ein.
:li.:hp2.WPSac:ehp2. bestimmt das Alter des aktuellen Pa·wortes. Ist die Nutzungsdauer nicht Åberschritten,
ist die Logon-Aktion beendet.
:li.Im anderen Fall wird ein neues Pa·wort bestimmt.
:li.Das alte Pa·wort wird im Netzwerkprodukt durch das neue ersetzt. Die entsprechende Syntax hÑngt von der
verwendeten Netzwerksoftware ab und ist im entsprechenden Benutzerhandbuch nachzulesen.
:li.Ist der Pa·wortwechsel erfolgreich gewesen, wird das neue Pa·wort bestÑtigt. :hp2.Sammy:ehp2. kann damit
das alte Pa·wort entfernen. Vor dieser BestÑtigung wird :hp2.Sammy:ehp2. immer versuchen, beim Logon das alte
Pa·wort zu verwenden.
:eol.

Beispiel&colon.
:cgraphic.
'logon 'username password
if SacPasswdAge() > 60 then
do
  newpassword = SacCreatePwd(8,6)
  'setpassword ' password newpassword
  if rc=0 then
    SacConfirmPwd()
end
:ecgraphic.

.*----------------------------------------------------------
.*  Anlegen der Benutzer
.*----------------------------------------------------------
:h2.Anlegen der Benutzer
:p.Die Benutzer-ID dient der Identifikation und Authentifizierung eines Benutzers im System.
Innerhalb der Konfiguration einer solchen ID mÅssen daher alle Angaben zu Benutzername,
Pa·wort, dem Zugriffsprofil, der ArbeitsoberflÑche sowie einer eventuellen Anmeldung an
einem LAN oder einer Datenbank gemacht werden.

:p.Um einen neuen Benutzer anzulegen, mu· im Hauptfenster der MenÅpunkt
:hp1.Benutzer:ehp1. &cdqf. :hp1.Neu:ehp1. ausgewÑhlt oder auf den Ordnerhintergrund mit der
rechten Maustaste geklickt werden.

.*  Einstellungsseite 1: User-ID und Pa·wort
:h3 clear scroll=none.User-ID und Pa·wort
:link reftype=hd res=022 auto split group=11
  vpx=left vpy=top vpcx=30% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=023 auto split group=10
  vpx=right vpy=top vpcx=70% vpcy=100%
  rules=border scroll=both titlebar=none.

:h3 res=022 group=11 hide.t1
:p.Diese Seite des Einstellungsnotizbuches beschreibt die Logon-Daten eines Benutzers. Dazu
gehîren der Benutzername, das dazugehîrige Pa·wort sowie die Information, ob eine Identifikationskarte
fÅr den Einlogvorgang verwendet werden kann.
:p.Ist keine :link reftype=hd refid=MAGSUPP.MAGCALLS.DLL:elink. kompatible Schnittstelle vorhanden,
ist von den drei Auswahlknîpfen nur der Knopf :hp1.Keine:ehp1. aktiv.
Mit vorhandener KartenunterstÅtzung gibt es folgende Auswahlmîglichkeiten&colon.
:ul compact.
:li.Der Knopf :hp1.Keine:ehp1. besagt, da· keine Kartendaten mit abgespeichert werden sollen.
:li.:hp1.Neu:ehp1. Åbernimmt die Daten einer neu durchgezogenen Karte in die Benutzerdaten.
:li.:hp1.Behalten:ehp1. bedeutet, da· die aktuell gespeicherten Daten beibehalten werden sollen.
:eul.

:h3 res=023 group=10 hide.t2
:artwork align=left name='wnd_user1.bmp'.
:p.1. Einstellungsseite aus dem Benutzer-Notizbuch

.*  Einstellungsseite 2: Zugriffsprofile
:h3 clear scroll=none.Zugriffsprofile
:link reftype=hd res=024 auto split group=11
  vpx=left vpy=top vpcx=30% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=025 auto split group=10
  vpx=right vpy=top vpcx=70% vpcy=100%
  rules=border scroll=both titlebar=none.

:h3 res=024 group=11 hide.t1
:p.Auf dieser Einstellungsseite wird das zum Benutzer gehîrige
:link reftype=hd refid=DEFS.Zugriffsprofil:elink. ausgewÑhlt, das die Aktionen festlegt,
die der Benutzer auf seiner ArbeitsoberflÑche benutzen darf. Zwei dieser Profile kînnen
hier angegeben werden&colon.
:ol compact.
:li.Das Benutzer-Profil ist ein Profil, das zu einem spezifischen Benutzer gehîrt.
:li.Weiterhin kînnen die Benutzer in verschiedene Gruppen aufgeteilt werden,
wobei jede dieser Gruppen durch ein Gruppen-Profil reprÑsentiert wird.
:eol.
Bei widersprÅchlichen Angaben in den beiden Profilen hat das Benutzer-Profil die hîhere
PrioritÑt.

:h3 res=025 group=10 hide.t2
:artwork align=left name='wnd_user2.bmp'.
:p.2. Einstellungsseite aus dem Benutzer-Notizbuch

.*  Einstellungsseite 3: ArbeitsoberflÑche
:h3 clear scroll=none.ArbeitsoberflÑche
:link reftype=hd res=026 auto split group=11
  vpx=left vpy=top vpcx=30% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=027 auto split group=10
  vpx=right vpy=top vpcx=70% vpcy=100%
  rules=border scroll=both titlebar=none.

:h3 res=026 group=11 hide.t1
:p.Die dritte Einstellungsseite im Einstellungsnotizbuch des Benutzers dient der Auswahl
der :link reftype=hd refid=DEFS.ArbeitsoberflÑche:elink..
Dies erfolgt durch Selektieren einer der aufgelisteten Zeilen.

:h3 res=027 group=10 hide.t2
:artwork align=left name='wnd_user3.bmp'.
:p.3. Einstellungsseite aus dem Benutzer-Notizbuch

.*  Einstellungsseite 4: Login
:h3 clear scroll=none.Login
:link reftype=hd res=028 auto split group=11
  vpx=left vpy=top vpcx=30% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=029 auto split group=10
  vpx=right vpy=top vpcx=70% vpcy=100%
  rules=border scroll=both titlebar=none.

:h3 res=028 group=11 hide.t1
:p.Diese Einstellungsseite dient der Angabe der fÅr den Benutzer gÅltigen Ein-/Auslog-Prozeduren.
Jedes hier angegebene Skript definiert sowohl den Ein- wie auch den Auslog-Proze· zu einem spezifischen
LAN-Produkt. Es kînnen beliebig viele solcher Skripte angegeben werden.
Diese Einstellungsseite besitzt 3 verschiedene Aktionen&colon.
:ul compact.
:li.:hp1.Neu:ehp1. legt ein neues Skript unter dem Namen an, der in der nebenstehenden
Combobox eingegeben wurde.
:li.:hp1.éndern:ehp1. erlaubt eine énderung der Parameter eines bereits erstellten Skriptes, wie z. B.
die User-ID oder das Pa·wort zu einem LAN-Produkt oder auch die Auswahl anderer
:link reftype=hd refid=LOGDLG.Prozeduren:elink..
:li.:hp1.Lîschen:ehp1. lîscht das ausgewÑhlte Logon-Skript aus der Liste.
:eul.

:h3 res=029 group=10 hide.t2
:artwork align=left name='wnd_user4.bmp'.
:p.4. Einstellungsseite aus dem Benutzer-Notizbuch

.*  Festlegen der Login-Parameter
:h3 clear scroll=none.Festlegen der Login-Parameter
:link reftype=hd res=040 auto split group=11
  vpx=left vpy=top vpcx=30% vpcy=100%
  rules=border scroll=vertical titlebar=none.
:link reftype=hd res=041 auto split group=10
  vpx=right vpy=top vpcx=70% vpcy=100%
  rules=border scroll=both titlebar=none.

:h3 res=040 group=11 hide.t1
:p.Dieser Dialog legt die Parameter fest, die fÅr einen Logon (bzw. Logoff) in einem LAN-Produkt
notwendig sind. Die erste Gruppe beinhaltet den Benutzernamen (entspricht der REXX-Variablen
:font facename=Courier size=12x12.username:font facename=default.), das Pa·wort (entspricht der REXX-Variablen
:font facename=Courier size=12x12.password:font facename=default.) sowie die DomÑne (entspricht der REXX-Variablen
:font facename=Courier size=12x12.domain:font facename=default.). 
:note text='Tip:'.Der Wert in diesen Eingabefeldern wird lediglich in die entsprechenden REXX-Variablen kopiert.
Sie kînnen daher problemlos auch fÅr andere Zwecke verwendet werden, wenn dies ein spezieller Logon-Vorgang erfordern
sollte.

:p.Die zweite Gruppe :hp1.Prozeduren:ehp1. legt fest, welche Logon- bzw. Logoff-Prozedur verwendet werden soll.
Diese Prozeduren mÅssen daher bereits vorher eingegeben werden (siehe
:link reftype=hd refid=UPROFLOG.Logon-Prozeduren:elink im HauptmenÅ).

:h3 res=041 group=10 hide.t2
:artwork align=left name='wnd_lslog.bmp'.
:p.Einstelldialog fÅr Logon-Parameter

.*----------------------------------------------------------
.*  Benutzung von WP_PARSE
.*----------------------------------------------------------
:h1 id=WPPARSE.Benutzung von WP_PARSE
:p.Zugriffsprofile kînnen auf 2 verschiedene Arten angelegt werden. Die
:link reftype=hd refid=UPROFPRF.erste Methode:elink. wurde bereits
im vorhergehenden Kapitel gezeigt: das Anlegen mit graphischen Hilfsmitteln im Programm UPROFILE.

:p.Es gibt jedoch FÑlle, in denen es einfacher ist, solche Zugriffsprofile als ASCII-Dateien mit
Hilfe eines einfachen Texteditors zu erstellen. FÅr diesen Anwendungszweck wurde das Programm
:hp2.WP_PARSE:ehp2. geschaffen.

.*----------------------------------------------------------
.*  Anlegen der Zugriffsprofile
.*----------------------------------------------------------
:h2.Anlegen der Zugriffsprofile
:p.Die Zugriffsprofile werden im Klartext angelegt und mit Hilfe des Programmes :hp2.WP_PARSE.EXE:ehp2.
in ein maschinenlesbares Format umgewandelt, das von :hp2.AUTHORIZE.DLL:ehp2. benutzt werden kann.
GrundsÑtzlich gibt es 2 Profiltypen: Benutzerprofile sind benutzerspezifisch (ein Profil kann jedoch auch fÅr mehrere
Benutzer verwendet werden), Gruppenprofile gelten fÅr eine ganze Gruppe von Benutzern.

:p.Die Dateien sind eine Sammlung von Objektbeschreibungen.
Jede Objektbeschreibung beginnt mit :font facename=Courier size=12x12.OBJECT (<type>='<string>'):font size=0x0.
und endet mit :font facename=Courier size=12x12.ENDOBJECT:font size=0x0..
Zwischen diesen SchlÅsselworten werden :link reftype=hd refid=FLAGS.Flags:elink. aufgezÑhlt.
:font facename=Courier size=12x12.(<:link reftype=hd refid=TYPE.type:elink.>='<string>'):font size=0x0. bezeichnet das angesprochene Objekt.

:p.Damit lautet die vollstÑndige Syntax&colon.
:cgraphic.
OBJECT (<type>='<string>')
<flag>
[<flag>]*
ENDOBJECT
:ecgraphic.
:p.Das :link reftype=hd refid=BEISPIEL.Beispiel:elink. zeigt das Aussehen einer solchen Profildatei.
Diese werden mit Hilfe von :hp2.WP_PARSE:ehp2. compiliert&colon.
:cgraphic.WP_PARSE <infile>:ecgraphic.
:p.Die Ausgabedatei erhÑlt bei gleichem Grundnamen die Endung :hp2..SAC:ehp2..

.*----------------------------------------------------------
.*  Objekttyp
.*----------------------------------------------------------
:h3 id=TYPE.Objekttyp
:p.Nachfolgend werden alle zulÑssigen Objekttypen beschrieben. In den Namen sind Wildcards '*' und '?'
zulÑssig.

:p.:table cols='15 18 30'.
:row.
:c.Typ
:c.Beschreibung
:c.Beispiel
:row.
:c.CLASS
:c.Klassenname
:c.CLASS='WPProgram'
:row.
:c.TITLE
:c.Objekttitle
:c.TITLE='*Gesamtbildschirm'
:row.
:c.TYPE
:c.Objekttyp
:c.TYPE='Plain Text'
:row.
:c.DEFTITLE
:c.Default-Titel
:c.DEFTITLE='Druckerobjekt'
:row.
:c.PHYSNAME
:c.Physikalischer Name
:c.PHYSNAME='config.sys'
:etable.


.*----------------------------------------------------------
.*  Beispiel
.*----------------------------------------------------------
:h3 id=BEISPIEL.Beispiel
:p.:cgraphic.
OBJECT ( TITLE="*ArbeitsoberflÑche*" )
NOARRANGE
ENDOBJECT

OBJECT ( TITLE = "*Gesamtbildschirm" )
NODELETE
NOMOVE
ENDOBJECT

OBJECT ( TITLE = "Postscript")
NODELETE
ENDOBJECT

OBJECT (TITLE = "*ditor")
NODELETE
NOMOVE
NOCOPY
ENDOBJECT

OBJECT (CLASS = "WPFolder")
NOSETTINGS
ENDOBJECT

OBJECT (TITLE = "Lauf*")
NOCLOSE
MOVE
NORENAME
NOSETTINGS
ENDOBJECT
:ecgraphic.

.*----------------------------------------------------------
.*  Benutzung von SAMMY
.*----------------------------------------------------------
:h1 id=SAMMY.Benutzung von SAMMY
:p.Das PM-Programm :hp2.SAMMY.EXE:ehp2. liefert den Einlog-Dialog fÅr den Benutzer.
Es wird nach dem PM vom System geladen und ist fÅr den Start der individuellen ArbeitsoberflÑche
zustÑndig.

:p.Die Bedienung des Programmes ist sehr einfach. Es hat auf dem Hauptfenster 2 Eingabezeilen,
in die der Benutzer seine User-ID und das Pa·wort einzutragen hat. Sind die Angaben korrekt,
wird nach BetÑtigen der Return-Taste die ArbeitsoberflÑche gestartet, zugeordnete
:link reftype=hd refid=UPROFLOG.Logon-Prozeduren:elink. gestartet und die
:link reftype=hd refid=DEFS.Zugriffsprofile:elink. geladen.

:p.Das OptionenmenÅ (erreichbar Åber den Knopf :hp1.Optionen:ehp1.) bietet darÅber hinaus noch folgende
Mîglichkeiten&colon.
:ul compact.
:li.:hp3.Systemabschlu·&colon.:ehp3. Hier kann ein Systemabschlu· ausgelîst werden, ohne da· eine
ArbeitsoberflÑche vorher gestartet werden mu·.
:li.:hp3.Kennwort Ñndern&colon.:ehp3. Hier kann das Pa·wort eines Benutzers geÑndert werden, ohne
da· :hp2.UPROFILE.EXE:ehp2. aufgerufen werden mu·. Dieser Dialog wird auch automatisch gestartet, wenn
der GÅltigkeitszeitraum eines Pa·wortes abgelaufen ist.
:li.:hp3.Ansicht&colon.:ehp3. Hier kann die Hintergrundfarbe sowie das Hintergrundbild des
:hp2.SAMMY:ehp2.-Hauptfensters eingestellt werden.
:eul.

.*----------------------------------------------------------
.*  Benutzung der Systemstart-Ordner
.*----------------------------------------------------------
:h1 id=STARTUP.Benutzung der Systemstart-Ordner
:p.OS/2 bietet standardmÑ·ig einen Systemstart-Ordner an. Dieser startet jedoch die ihm zugeordneten Objekte
nur einmal pro Systemstart und nicht, wie fÅr manche Programme gefordert, einmal pro Benutzer-Logon.
Aus diesem Grund wird mit :hp2.WPSac:ehp2. ein eigener :hp1.Benutzer-Systemstart:ehp1.-Ordner mitgeliefert.
Dieser startet die ihm zugeordneten Objekte nach :hp1.jedem:ehp1. Logon.

Folgende Hinweise sind bei der Benutzung dieses Ordners zu beachten&colon.
:ul compact.
:li.Dieser Ordnertyp besitzt eine eigene Schablone im Schablonenordner. Daher kann er durch Ziehen einer Schablone
von dort beliebig dupliziert werden. Das hei·t, jeder Benutzer kann mehrere Benutzer-Systemstart-Ordner besitzen.
Diese werden nach einem Logon in einer :hp1.beliebigen:ehp1. Reihenfolge gestartet.
:li.:hp2.WPSac:ehp2. unterstÅtzt maximal 10 solcher Ordner pro Benutzer. Die Schablone im
Schablonenordner zÑhlt dazu. Jeder weitere Benutzer-Systemstart-Ordner kann beim Starten der Objekte nicht berÅcksichtigt
werden. Der Grund dafÅr liegt darin, da· Sammy diese Ordner Åberwacht, damit nach einem Absturz der ArbeitsoberflÑche
und deren Neustart die Benutzer-Systemstart-Ordner nicht noch einmal gestartet werden.
:eul.

.*----------------------------------------------------------
.*  Generieren der ArbeitsoberflÑchen
.*----------------------------------------------------------
:h1.Generieren der ArbeitsoberflÑchen
:p.ArbeitsoberflÑchen werden mit Hilfe des Programmes :hp2.UPROFILE.EXE:ehp2. erzeugt und verwaltet.
Sie werden dort Åber das MenÅ
:hp1.:link reftype=hd refid=UPROFWPS.Einstellung &cdqf. ArbeitsoberflÑchen:elink.:ehp1. angelegt.

:p.Die ArbeitsoberflÑchen werden bei ihrer ersten Benutzung von :hp2.SAMMY.EXE:ehp2. automatisch generiert.
Dieser Vorgang entspricht dem Anlegen einer OberflÑche beim ersten Systemstart nach der Installation von OS/2.
Wurde zur Generierung die Resource-Datei :font facename=Courier size=12x12.INI.RC:font size=0x0. verwendet,
entspricht das Aussehen der neuen OberflÑche auch diesem Grundzustand.

.*----------------------------------------------------------
.*  Benutzeridentifikation durch Id-Karte
.*----------------------------------------------------------
:h1 id=MAGSUPP.Benutzeridentifikation durch Id-Karte
:p.Die Kennworteingaben des Systems kînnen alternativ Åber einen Id-Kartenleser (z. B. Magnet-
oder Chipkartenleser) erfolgen. Voraussetzung ist das Vorhandensein einer DLL-Datei :hp1.MAGCALLS.DLL:ehp1..
Diese gehîrt zum Lieferumfang der OS/2-UnterstÅtzung der Kartenleser.
Getestet wurden die GerÑte der Firmen IBM (4717) und DESKO (MSL4717-x).
Geeignet sind alle Magnetkarten, bei denen die Spur 2 beschrieben ist.
HierfÅr kînnen spezielle Karten der Firma DESKO verwendet werden, es reicht jedoch
jede handelsÅbliche Karte, wie EC-Karte oder eine Kreditkarte.
Werden andere Karten (wie Kantinenkarten o. Ñ.) eingesetzt, ist sicherzustellen, da· der Inhalt
von Spur 2 nie verÑndert wird.
In den Systemeinstellungen von :hp2.WPSac:ehp2. kann gewÑhlt werden, ob
:ol compact.
:li.die Pa·worteingabe alternativ Åber Tastatur ODER Karte erfolgen kann, oder
:li.das Pa·wort Åber Tastatur UND Karte eingegeben werden mu·.
:eol.
:p.Id-Kartenzuordnungen zu bestimmten Benutzern kînnen aus SicherheitsgrÅnden nicht im Optionen-MenÅ
des Programmes :hp2.SAMMY.EXE:ehp2. vorgenommen werden. Dies mu· in :hp2.UPROFILE.EXE:ehp2.
geschehen.

.*----------------------------------------------------------
.*  Begriffe
.*----------------------------------------------------------
:h1 id=DEFS.Begriffe
:p.:hp7.Benutzer:ehp7.
:lm margin=3.:p.Ein :hp1.Benutzer:ehp1. besitzt fÅr einen PC oder mehreren PCs in einem Netzwerk eine
:hp1.Benutzer-ID:ehp1. und optional ein Pa·wort. Die Verwendung von Identifikationskarten
(Magnet- oder Chipkarten) kann die Pa·worteingabe ersetzen oder als zusÑtzliche Sicherheit
ergÑnzen. Im ersten Fall ist die Eingabe der Benutzer-ID optional. Eine :hp1.Benutzer-ID:ehp1.
ist damit im Normalfall immer an eine :hp1.Person:ehp1. gebunden.

:lm margin=1.:p.:hp7.ArbeitsoberflÑche:ehp7.
:lm margin=3.:p.Eine :hp1.ArbeitsoberflÑche:ehp1. reprÑsentiert das Erscheinungsbild der gesamten
objektorientierten BenutzeroberflÑche. Die Administration des Aussehens von ArbeitsoberflÑchen
kann nach ergonomischen oder Design-Gesichtspunkten erfolgen und ist stark abhÑngig vom Einsatzort
des Systems.
GeÅbten Benutzern kann die selbstÑndige Anordnung bzw. die Reorganisation des Administratorvorschlages
erlaubt werden.
Es mu· darauf hingewiesen werden, da· ein vollstÑndiges Entfernen aller nicht benîtigten Objekte von
der ArbeitsoberflÑche nocht nicht fÅr einen hundertprozentigen Schutz des Systems ausreicht, da viele
Programme in ihren MenÅs Ausstiegsmîglichkeiten auf die Kommandozeile besitzen. Solche Gesichtspunkte
sind bei der Definition eines Sicherheitskonzeptes zu bedenken.

:lm margin=1.:p.:hp7.Objekte:ehp7.
:lm margin=3.:p.Die Objekte der objektorientierten ArbeitsoberflÑche(n) von OS/2 werden durch Bildsymbole (Icons)
reprÑsentiert. Jedes dieser Objekte kann mehrere AktivitÑten auslîschen. Ein Klick mit der rechten
Maustaste îffnet das :hp1.KontextmenÅ:ehp1. eines Objektes. Dessen erster MenÅpunkt ist die
Aktion :hp1.ôffnen:ehp1.. Die AktivitÑten beim ôffnen einen Objektes hÑngen vom Typ des Objektes ab.
Ein :hp1.Ordner:ehp1. zeigt in einem Fenster seinen Inhalt in einer von 3 verschiedenen Darstellungsformen,
Programmobjekte werden standardmÑ·ig gestartet.
Eine weitere wichtige Aktion ist das ôffnen des :hp1.Einstellungsnotizbuches:ehp1. des Objektes. Hier kînnen
bestimmte Eigenschaften des Objektes eingestellt werden.
DarÅber hinaus existieren weitere Aktionen, wie z. B. das Kopieren des Objektes, das Erstellen eines neuen
Objektes, das Verschieben des Objektes oder das Erstellen einer Referenz.

:lm margin=1.:p.:hp7.Zugriffsprofile:ehp7.
:lm margin=3.:p.Aktionen, die fÅr die einzelnen Objekte einer ArbeitsoberflÑche mîglich sind, sind bei
Fehlbedienungen stets die Ursache fÅr zerstîrte Umgebungen (von mutwilliger Zerstîrung ganz zu schweigen).
Daher ist es die Aufgabe eines geeigneten Sicherheitskonzeptes, einzelnen Benutzern nur die Aktionen
von Objekten zu erlauben, die fÅr seine Arbeit notwendig sind. Diese Aktionen sind im Extremfall fÅr
jedes Objekt der ArbeitsoberflÑche einzeln zu definieren, Sammelbegriffe erleichtern das Zusammenfassen
von Objektgruppen. Die Verwaltung solcher Zugriffsmîglichkeiten auf Objekte wird Åber Listen,
sogenannte :hp1.Zugriffsprofile:ehp1. geregelt, die der Systemadministrator zu erstellen hat.

:lm margin=1.:p.:hp7.énderungsanforderungen:ehp7.
:lm margin=3.:p.Im Normalfall werden alle ArbeitsoberflÑchen aus einer Resourcedatei, Åblicherweise
die Datei :hp1.INI.RC:ehp1. generiert. Abweichungen von dieser ArbeitsoberflÑche werden in einer
Datei beschrieben, deren Format-Syntax eine Untermenge von :hp1.INI.RC:ehp1. ist.
Sie wird als :hp1.énderungsanforderung:ehp1. bezeichnet. Auf eine ArbeitsoberflÑche kînnen nacheinander
beliebig viele énderungsanforderungen angewandt werden. Dadurch hat ein Administrator die Mîglichkeit,
mehrmals énderungen an einer ArbeitsoberflÑche im Laufe deren Lebenszyklus vorzunehmen.
Diese énderungsanforderungen werden mit Hilfe des Programmpaketes
:link reftype=fn refid=DESKMAN.DESKMAN/2:elink. erzeugt und erhalten die Dateiendung :hp1..drc:ehp1..
DESKMAN/2 gehîrt nicht zum Lieferumfang von :hp2.WPSac:ehp2..

:lm margin=1.:p.:hp1.Benutzer-IDs:ehp1., :hp1.ArbeitsoberflÑchen:ehp1. und :hp1.Zugriffsprofile:ehp1. sind
bei :hp2.WPSac:ehp2. im wesentlichen unabhÑngig voneinander. Das hei·t, einem Benutzer wird unabhÑngig
voneinander eine ArbeitsoberflÑche und ein Zugriffsprofil zugewiesen, die wiederum mehrfach, das hei·t
auch fÅr andere Benutzer, verwendet werden kînnen.

:lm margin=1.:p.:hp7.StammarbeitsoberflÑche:ehp7.
:lm margin=3.:p.Die ArbeitsoberflÑche, die vor der Installation von :hp2.WPSac:ehp2. aktiv war, wird als
:hp1.StammarbeitsoberflÑche:ehp1. bezeichnet. Sie kann bei der Auswahl von ArbeitsoberflÑchen mit dem
Wort :hp1.root:ehp1. ausgewÑhlt werden. Die StammarbeitsoberflÑche wird normalerweise reprÑsentiert durch das
Verzeichnis :font facename=Courier size=18x18.ArbeitsoberflÑche:font size=0x0. im Stammverzeichnis
des Bootlaufwerkes. Die zugehîrigen Profildateien
:font facename=Courier size=18x18.OS2.INI:font size=0x0. und
:font facename=Courier size=18x18.OS2SYS.INI:font size=0x0. befinden sich im
:font facename=Courier size=18x18.\OS2:font size=0x0.-Verzeichnis.

:fn id=DESKMAN.
:hp2.DESKMAN/2:ehp2. unterliegt dem Copyright der Firma :hp1.Development Technologies, Inc.:ehp1.
und teilweise :hp1.Gregory Czajy:ehp1..
:efn.

.*----------------------------------------------------------
.*  Updates
.*----------------------------------------------------------
:h1.Updates
:p.Updates fÅr das Programm sind grundsÑtzlich kostenlos. Sie kînnen Åber unsere Mailbox
(Tel.&colon. 07191/300472) bezogen werden.
Ein Versand per Diskette ist ebenfalls mîglich.
Bitte legen Sie Ihrem Anforderungsschreiben DM 5.- bei.

:p.Die Anfrage bitte richten an&colon.
:p.:lines align=left.
  noller &amp. breining software
  Lauffener Str. 4
  D-71522 Backnang
:elines.

.*----------------------------------------------------------
.*  Versionen
.*----------------------------------------------------------
:h1.Versionen
:xmp.
   1.0  Erste îffentliche Version von WPSac  
        fÅr OS/2 2.x und OS/2 Warp 3.0

   2.0  Grafische Administration mit UPROFILE  

   3.0  Erstellung der Zugriffsprofile mit UPROFILE  

   4.0  UnterstÅtzung von IBM LAN-Server, Erweiterung von SAMMY, UnterstÅtzung von OS/2 Warp 4.0

:exmp.

:euserdoc.
