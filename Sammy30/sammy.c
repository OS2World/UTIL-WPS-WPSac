/***************************************************************************
 *
 * PROGRAM NAME: SAMMY.C
 * -------------
 *
 * REVISION LEVEL: 3.0
 * ---------------
 *
 * WHAT THIS PROGRAM DOES:
 * -----------------------
 *  OS/2 Workplace Shell Access Manager:
 *  Umschalten zwischen den Shells fÅr WPSam und WPSac
 *
 * ROUTINES:
 * ---------
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *  IBM C/C++ Set/2 Compiler Version 2.0
 *  IBM OS/2 2.1 Programmers Toolkit
 *
 * REQUIRED FILES:
 * ---------------
 *  SAMMY.C       -   Quelldatei
 *  SAMMY.RC      -   Resource-Datei
 *  SAMMY.DLG     -   Resource-Datei
 *  SAMMY.H       -   Allgemeine Definitionen
 *  SAMMYDLG.H    -   Definitionen fÅr Resourcen
 *  SAMMYPAN.H    -   Definitionen fÅr Hilfe-Panels
 *  SAMGLOB.H     -   Gemeinsame Definitionen fÅr InitOS2, Sammy, WPSam, WPSac
 *
 * REQUIRED LIBRARIES:
 * -------------------
 *  OS2386.LIB    -   OS/2 32-Bit import library
 *  CUTIL.LIB     -   Hilfsroutinen
 *
 * CHANGE LOG:
 * -----------
 *
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *  3.00    11-12-94  Version mit WPSac-UnterstÅtzung
 *
 *  Copyright (C) 1994 Noller & Breining Software
 *
 ******************************************************************************/
/*
       Beschreibung der Semaphoren:

Sam benutzt drei Semaphoren: HEV_LOGOUT, HEV_SAMMY und HEV_PRFRESETLOCK (siehe SamGlob.h)

Der Ablauf beim Wechseln der Profiles ist folgenderma·en:

Sammy wartet, da· HEV_SAMMY zurÅckgesetzt wird.
Sodann bereitet er alles fÅr einen prfReset vor. (Environment, Schlie·en der ArbeitsoberflÑche etc.)
Unmittelbar vor dem eigentlichen prfReset wartet er auf das RÅcksetzen von HEV_PRFRESETLOCK.

HEV_PRFRESETLOCK wird ausschlie·lich von WPSamF manipuliert:
In der _wpOpen-Methode wird (falls es sich um einen aktiven Desktop handelt) die Semaphore gesetzt
und in der SaveState-Methode wird sie wieder gelîscht.
Bei einem neuen Desktop wird die SaveState-Methode erst ganz am Schlu· aufgerufen.
D.h. ein prfReset wird verhindert, bis alle Objekte ihre Instanzdaten geschrieben haben.

Ist Sammy nicht installiert, so wird HEV_SAMMY nicht verwendet, sondern die WPSam-eigene Funktion thChangeWPS aufgerufen.
Auch diese Routine wartet auf das RÅcksetzen von HEV_PRFRESETLOCK.

Die Semaphore HEV_SAMMY wird von Sammy verwendet, um zu testen, ob er bereits aufgerufen ist.
Wird er nÑmlich ein zweites mal aufgerufen, so reagiert er folgenderma·en:
Er beauftragt den bereits installierten Sammy, auf den Rootdesktop zurÅckzuschalten.

Ein-/Ausloggen:

ZustÑndig hierfÅr ist der Thread thLogInOut:
Solange kein Benutzer eingeloggt ist, wird der Start der Shell durch suspendieren von thLoopStartShell unterdrÅckt.
Aus- und Einloggen wird durch RÅcksetzen von HEV_LOGOUT initiiert.
Um dieses Feature zu entfernen, braucht der Thread lediglich die Semaphore HEV_LOGOUT zu schlie·en und sich anschlie·end zu beenden.
*/

#define INCL_WIN
#define INCL_PM
#define INCL_DOS            /* ?? */
#define INCL_DOSSESMGR      /* Session Manager values */
#define INCL_DOSQUEUES      /* Queue values */
#define INCL_DOSERRORS

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include <cutil.h>

#include "samglob.h"
#include "authoriz.h"
#include "Authorize.h"
#include "ChangePwd.h"
#include "debug.h"
#include "LANDrive.h"
#include "LogInOut.h"
#include "magpro2.h"
#include "MagSupp.h"
#include "prepare.h"
#include "profile.h"
#include "SacAlias.h"
#include "SamDlg.h"
#include "SamData.h"
#include "SamMem.h"
#include "Sammy.h"
#include "SammyDlg.h"
#include "SammyPan.h"

/* Key fÅr die VerschlÅsselung */
ULONG key[2] = {ULKEY1, ULKEY2};

/* Globale Variablen */
HMTX     hmtxPrfReset     = NULLHANDLE;
HEV      hevPrfResetLock   = 0;
ULONG    ulOldFlag = 0;             // Nach erfolgr. Umschalten steht hier ulFlag
                                    // vom aktiven Benutzer

#ifdef DEBUG
ULONG   ulDebugMask = -1;
#endif // DEBUG

/* Modulinterne Funktionen */
MRESULT EXPENTRY LoginDlgProc  (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
BOOL             ChangeWPS (VOID);
BOOL             Marry (PCHAR szUserIni, PCHAR szSystemIni);

/***********************************************************************
    Initialisiert die Semaphoren.
    hevPrfResetLock (HEV_PRFRESETLOCK):
    return: Fehlercode von Semaphor-Aufrufen
 ***********************************************************************/
APIRET InitSem (VOID)       // fÅr WPSAC notwendig???
    {
    APIRET rc;

    if ((rc = DosOpenEventSem (HEV_PRFRESETLOCK, &hevPrfResetLock)) != 0)
        {
        if ((rc = DosCreateEventSem( HEV_PRFRESETLOCK, &hevPrfResetLock, 0, TRUE)) != 0)
            return rc;
        }
    else        // ??? was soll das?
        DosPostEventSem(hevPrfResetLock);

    /* Semaphore, die ein zu dichtes Aufeinanderfolgen von Umschaltungen verhindert */
    rc = DosCreateMutexSem (NULL, &hmtxPrfReset, 0, TRUE);

    return rc;
    }

/***********************************************************************
    U n t e r p r o g r a m m e
 ***********************************************************************/

/***********************************************************************
    Diese Prozedur untersucht, ob SystemIni und UserIni zusammengehîren.
    Ist das nicht der Fall, wird eine WM_MESSAGEBOX-Message gepostet.
    Aus KompatibilitÑtsgrÅnden mit alten Versionen wird bei
    INI-Dateien ohne EAT_MARRY-Attribut dieses geschrieben.

    return:  True -> Ini-Dateien sind zusammengehîrig
 ***********************************************************************/
BOOL Marry (PCHAR szUserIni, PCHAR szSystemIni)
    {
    ULONG  ulMarrySystem[2] = {0, 0};
    ULONG  ulMarryUser[2]   = {0, 0};
    STRUC_EAT_DATA ar_Value[2];

    memset (ar_Value, '\0', 2*sizeof (STRUC_EAT_DATA));

    /*  Lesen des EAT_MARRY-EAs der System-Ini */
    ar_Value[0].usEAType = EAT_BINARY;
    ar_Value[0].uscValue = 2*sizeof (ULONG);
    ar_Value[0].pValue   = (PBYTE)ulMarrySystem;
    EARead (szSystemIni, EA_MARRY, ar_Value);

    /*  Falls es das EA noch nicht gibt, wird es jetzt geschrieben */
    if (ar_Value[0].uscValue == 0)
        {
        DosQuerySysInfo(QSV_TIME_LOW, QSV_TIME_HIGH, ulMarrySystem, 2*sizeof (ULONG));
        ar_Value[0].usEAType = EAT_BINARY;
        ar_Value[0].uscValue = 2*sizeof (ULONG);
        ar_Value[0].pValue   = (PBYTE)ulMarrySystem;
        EAWrite (szSystemIni, EA_MARRY, ar_Value);
        }

    /*  Lesen des EAT_MARRY-EAs der User-Ini */
    ar_Value[0].usEAType = EAT_BINARY;
    ar_Value[0].uscValue = 2*sizeof (ULONG);
    ar_Value[0].pValue   = (PBYTE)ulMarryUser;
    EARead (szUserIni, EA_MARRY, ar_Value);

    /* Falls es das EA noch nicht gibt, wird es jetzt geschrieben */
    if (ar_Value[0].uscValue == 0)
        {
        ar_Value[0].usEAType = EAT_BINARY;
        ar_Value[0].uscValue = 2*sizeof (ULONG);
        ar_Value[0].pValue   = (PBYTE)ulMarrySystem;
        EAWrite (szUserIni, EA_MARRY, ar_Value);
        }
    else
        {
        /* Gehîren die INI-Dateien zusammen? */
        if ((ulMarrySystem[0] != ulMarryUser[0]) || (ulMarrySystem[1] != ulMarryUser[1]))
            {
            WinPostMsg (hwndLogin, WM_MESSAGEBOX, (MPARAM)IDS_NOTMARRIED, MPVOID);
            return FALSE;
            }
        }

    return TRUE;
    }

/***********************************************************************
    Schaltet den Desktop um. Dazu wird zuerst eine PRFPROFILE-Struktur
    ausgefÅllt, anschlie·end der PrfReset durchgefÅhrt.
    Eingang: pszUserIni:   Zeiger auf Name der User-Ini-Datei.   '\0' => Root
             pszSystemIni: Zeiger auf Name der System-Ini-Datei. '\0' => Root
    return:  FALSE: Fehler aufgetreten.
 ***********************************************************************/
BOOL ChangeWPS (VOID)
    {
    HINI        hiniUser;
    APIRET      rc;
    ULONG       ulSize;
    BOOL        bSuccess;
    PCHAR       pszTempUser, pszTempSystem;
    PCHAR       pszIniData;
    PPRFPROFILE pprfProfile;

    PCHAR pszUserIni =    pShareMem->szUserIni;
    PCHAR pszSystemIni =  pShareMem->szSystemIni;

    /** User-Ini fÅr den neuen Desktop bestimmen **/
    pszTempUser = pszUserIni;

    /* kein Benutzer angegeben => Root-Desktop starten */
    if (*pszTempUser == '\0')
        {
        pszTempUser       = pShareMem->szRootUserIni;
        pShareMem->ulFlag = pShareMem->ulFlag_Root;         // ulFlag fÅr Root-Desktop
        }

    /* Shared-Memory nicht korrekt initialisiert => Root-Desktop aus Environment holen */
    if (*pszTempUser == '\0')
        if (DosScanEnv (ENV_USER_INI, &pszTempUser))
            /* Fehler in Environment => Leerstring fÅr Root-Desktop */
            pszTempUser = "";

    /** aktuelle System-Ini bestimmen **/
    pszTempSystem = pShareMem->szRootSystemIni;

    /* Shared-Memory nicht korrekt initialisiert => Dateiname aus Environment holen */
    if (*pszTempSystem == '\0')
        if (DosScanEnv (ENV_SYSTEM_INI, &pszTempSystem))
            /* Fehler in Environment => Leerstring fÅr Root-Desktop */
            pszTempSystem = "";

    /* Stimmt System-Ini des Benutzers mit aktueller System-Ini Åberein? */
    if ((pszSystemIni != NULL) && (*pszSystemIni != '\0'))
        if (strcmpi (pszTempSystem, pszSystemIni))
            {
            WinPostMsg (hwndLogin, WM_MESSAGEBOX, (MPARAM)IDS_USERWITHSYSTEMINI, MPVOID);
            return FALSE;
            }

    /** Struktur fÅr PrfReset erzeugen und ausfÅllen */
    pprfProfile = AllocMem (sizeof (PRFPROFILE) + strlen (pszTempUser) + strlen (pszTempSystem) + 2);
    if (pprfProfile == NULL)
        return FALSE;

    pprfProfile->cchUserName = strlen (pszTempUser) + 1;
    pprfProfile->pszUserName = (PSZ)&pprfProfile[1];
    strcpy (pprfProfile->pszUserName, pszTempUser);

    pprfProfile->cchSysName = strlen (pszTempSystem) + 1;
    pprfProfile->pszSysName = &pprfProfile->pszUserName[strlen (pprfProfile->pszUserName) + 1];
    strcpy (pprfProfile->pszSysName, pszTempSystem);

    /** Gehîren User-Ini und System-Ini zusammen? **/
    if (!Marry (pprfProfile->pszUserName, pprfProfile->pszSysName))
        bSuccess = FALSE;
    else
        {
        /** Desktop umschalten **/
        WinSetObjectData (WinQueryObject ("<WP_DESKTOP>"),
                          ulOldFlag & CLOSEAPPL ? "WORKAREA=YES" : "WORKAREA=NO");

//???     WinPostMsg (WinQueryWindow (HWND_DESKTOP, QW_BOTTOM), WM_CLOSE, 0, 0);

        /* Warten, bis ausgewÑhlter Desktop aktiv ist */ // nur bei WPSam ???
        DosWaitEventSem (hevPrfResetLock, SEM_INDEFINITE_WAIT);
        DosSleep (1000);

        /* Klasse WPObject durch WPSac ersetzen. Da SOM nicht gestartet ist, mu· */
        /* dies durch direkten Eingriff in das User-Profile geschehen!           */

        /* hier notfalls eingreifen: WPSac nur registrieren, wenn nîtig, d. h. SAC-User ??? */
        hiniUser = PrfOpenProfile (hab, pprfProfile->pszUserName);
        if (hiniUser)
            {
            bSuccess = PrfQueryProfileSize (hiniUser, INI_REPL_APP, INI_REPL_KEY, &ulSize);
            if (bSuccess && ulSize)
                {
                DebugE (D_SAM, "ChangeWPS", "PM_Workplace:ReplaceList lesen");
                pszIniData = AllocMem (ulSize + 2);
                if (pszIniData)
                    {
                    ulSize = PrfQueryProfileString (hiniUser, INI_REPL_APP, INI_REPL_KEY, NULL, pszIniData, ulSize+2);
                    pszIniData[ulSize] = '\0';
                    if (ulSize == sizeof (INI_REPL_DATA) - 1)
                        bSuccess = strcmp (pszIniData, INI_REPL_DATA) == 0;
                    else
                        bSuccess = FALSE;
                    }
                }
            if (!bSuccess)
                PrfWriteProfileString (hiniUser, "PM_Workplace:ReplaceList", "WPObject", "WPSac");
            if (rc == 0)
                {
                DebugE (D_SAM, "FreeMem", "ChwWPS1");
                FreeMem ((PPVOID)&pszIniData);
                DebugE (D_SAM, "FreeMem", "ChwWPS2");
                }
            PrfCloseProfile (hiniUser);
            }

        /* Profile umschalten */
        bSuccess = PrfReset (hab, pprfProfile);

        /* Beim Umschalten auf den Root wird die Shell vom PrfReset nicht vollstÑndig */
        /* geschlossen.                                                               */
        /* Solange die Ursache nicht bekannt ist, bleibt nichts anderes Åbrig,        */
        /* als an dieser Stelle die Shell zu schlie·en.                               */
        if (*pszUserIni == '\0')
            WinTerminateApp (happShell);                    // sog. Budnik'scher Arschtritt

        if (!bSuccess)
            {
            // Problem bei PrfReset aufgetreten
            WinSetObjectData (WinQueryObject ("<WP_DESKTOP>"), "OPEN=ICON;WORKAREA=YES");
            lGlobalRetCode = SAMERR_PRFRESET;
            WinPostQueueMsg (hmq, WM_QUIT, 0L, 0L);
            }
        else
            ulOldFlag = pShareMem->ulFlag;
    }

    DebugE (D_SAM, "FreeMem", "SwProf1");
    FreeMem ((PPVOID)&pprfProfile);
    DebugE (D_SAM, "FreeMem", "SwProf2");
    return bSuccess;
    }

   /*
    *  Diese Routine versucht, eine Shell zu starten.
    */
HAPP StartShell ()
    {
    PCHAR       pszEnvironment;
    PCHAR       pszEnvString;
    APIRET      rc;
    PROGDETAILS Details;
    HAPP        happ = NULLHANDLE;

    pszEnvironment = MakeEnv (pShareMem->szEnvironment);

    Details.Length          = sizeof (PROGDETAILS);
    Details.progt.progc     = PROG_DEFAULT;
    Details.progt.fbVisible = SHE_VISIBLE;
    Details.pszTitle        = "";
    Details.pszParameters   = "";
    Details.pszStartupDir   = "";
    Details.pszIcon         = "";
    Details.pszEnvironment  = pszEnvironment;
    Details.swpInitial.fl   = SWP_ACTIVATE;     /* window positioning   */
    Details.swpInitial.cy   = 0;                /* width of window      */
    Details.swpInitial.cx   = 0;                /* height of window     */
    Details.swpInitial.y    = 0;                /* lower edge of window */
    Details.swpInitial.x    = 0;                /* left edge of window  */
    Details.swpInitial.hwndInsertBehind = HWND_TOP;
    Details.swpInitial.hwnd             = hwndLogin;
    Details.swpInitial.ulReserved1      = 0;
    Details.swpInitial.ulReserved2      = 0;

    /* Shell aus Environment starten */
    rc = (DosScanEnv (ENV_SAMWORKPLACE, &pszEnvString) != 0) ? ERROR_FILE_NOT_FOUND : NO_ERROR;
    if (rc == 0)
        {
        Details.pszExecutable = pszEnvString;
        happ = WinStartApp (hwndLogin, &Details, NULL, NULL, SAF_STARTCHILDAPP);
        if (happ)
            goto Exit;
        }

    /* PMSHELL.EXE starten */
    Details.pszExecutable = APP_PMSHELL;
    happ = WinStartApp (hwndLogin, &Details, NULL, NULL, SAF_STARTCHILDAPP);
    if (happ)
        goto Exit;

    /* CMD.EXE starten */
    Details.pszExecutable = APP_CMD;
    happ = WinStartApp (hwndLogin, &Details, NULL, NULL, SAF_STARTCHILDAPP);

Exit:
    if (pszEnvironment)
        DosFreeMem (pszEnvironment);

    return happ;
    }

VOID MessageBox (HWND hwndOwner, ULONG idText, ULONG idTitle, ULONG flStyle)
    {
    CHAR  szText[CCHMAXMSG];
    CHAR  szTitle[CCHMAXMSG];
    PCHAR pszTitle;

   /* idTitle = 0             => kein Titel     */
   /* idTitle = MBTITLE_ERROR => Titel "Fehler" */
   /* idTitle   sonst         => Text ID        */

    WinLoadString (WinQueryAnchorBlock (HWND_DESKTOP), (HMODULE)0,
        idText, CCHMAXMSG, szText);

    switch (idTitle)
        {
        case MBTITLE_ERROR:
            pszTitle = NULL;
            break;
        case 0:
            pszTitle = "";
            break;
        default:
            WinLoadString (WinQueryAnchorBlock (HWND_DESKTOP), (HMODULE)0,
                idTitle, CCHMAXMSG, szTitle);
            pszTitle = szTitle;
        }

    WinMessageBox (HWND_DESKTOP, hwndOwner, szText,
        pszTitle, IDD_MESSAGEBOX, flStyle);

    return;
    }

/***********************************************************************
    H a u p t p r o g r a m m
    Eingang: argc, argv: Parameter fÅr Default-Shell
    return:  Fehlercode
 ***********************************************************************/
LONG main (void)
    {
    BOOL        bIsWPSac;
    QMSG        qmsg;
    HELPINIT    hiHelp;
    ULONG       ulc;
    ULONG       ulErrId;
    static CHAR szWindowTitle[TITLELEN];
    static CHAR szLibName[LIBNAMELEN];

    lGlobalRetCode = SAMERR_OK;

    /* Sammy ist ein PM-Programm */
    hab = WinInitialize (0);
    hmq = WinCreateMsgQueue (hab, 0);

    /** Initialisierung fÅr die WPSac-Funktion: **/
    //  1. Pfade holen
    //     => szPathLAN, szPathLoc
    szPathLAN[0] = szPathLoc[0] = '\0';
    ulc = PrfQueryProfileString (HINI_SYSTEMPROFILE,
                                 OS2SYS_APP_NAME,
                                 OS2SYS_KEY_CLNT_LAN,
                                 NULL,
                                 szPathLAN,
                                 CCHMAXPATH);
    szPathLAN[ulc] = '\0';
    ulc = PrfQueryProfileString (HINI_SYSTEMPROFILE,
                                 OS2SYS_APP_NAME,
                                 OS2SYS_KEY_CLNT_LOCAL,
                                 NULL,
                                 szPathLoc,
                                 CCHMAXPATH);
    szPathLoc[ulc] = '\0';

    //  2. Im Falle einer LAN-UnterstÅtzung LAN-Typ
    //     bestimmen und Laufwerk verbinden
    //     => ./.
    MountLANDirectory ();

    //  3. Warten auf LAN-Laufwerk
    //     => ./.
    Wait4LANDirectory ();

    //  4. lokale INI-Datei îffnen
    //     => ./.
    bIsWPSac = OpenLocINI ();

    //  5. Sicherstellen, da· mindestens 1 User existiert
    //     => ./.
    bIsWPSac &= CreateCheck1User ();

    //  6. Dateien umkopieren
    //     => ./.
    ulErrId = CopyLANDir (szPathLoc, szPathLAN);

    //  7. Prelogoff
    UnmountLANDirectory ();

    //  8. Einsprungadresse in AUTHORIZ.DLL bestimmen
    //     => ./.
    //     Falls 6. nicht ok war, wird WPSac nicht verwendet
    if (bIsWPSac)
        {
        if (!LoadAuthorize ())
            DebugE (D_SAM, "main", "Problem mit Authorize-DLL");
        }
    else
        DebugE (D_SAM, "main", "Problem mit UPROFILE.INI");

    //  9. ALIASDATA-Struktur initialisieren */
    //     => pLogin
    memset (&strucUserData, 0, sizeof (ALIASDATA));

    /** MagnetkartenunterstÅtzung starten **/
    MCInit ();

    /** Dialogbox als Object-Window laden **/
    //     => hwndLogin
    hwndLogin = WinLoadDlg (HWND_DESKTOP,                   // Parent
                            HWND_DESKTOP,                   // Owner
                            (PFNWP) LoginDlgProc,           // DlgProc
                            NULLHANDLE,                     // Module Handle
                            IDD_LOGIN,                      // Resource ID
                            NULL);                          // CreateParms

    /** Hilfe initialisieren **/
    //     => hwndHelp
    hiHelp.cb                       = sizeof (HELPINIT);    // LÑnge Init-Struktur
    hiHelp.ulReturnCode             = 0;
    hiHelp.pszTutorialName          = NULL;                 // kein Tutorial
    hiHelp.phtHelpTable             = (PHELPTABLE)MAKEULONG (IDH_MAIN, 0xFFFF);
    hiHelp.hmodHelpTableModule      = NULLHANDLE;
    hiHelp.hmodAccelActionBarModule = NULLHANDLE;           // Module Handle
    hiHelp.idAccelTable             = NULLHANDLE;
    hiHelp.idActionBar              = NULLHANDLE;
    hiHelp.fShowPanelId             = CMIC_HIDE_PANEL_ID;

    WinLoadString (hab, NULLHANDLE, IDS_HELPWINDOWTITLE, TITLELEN, szWindowTitle);
    hiHelp.pszHelpWindowTitle = (PSZ)szWindowTitle;

    WinLoadString (hab, NULLHANDLE, IDS_HELPLIBRARYNAME, LIBNAMELEN, szLibName);
    hiHelp.pszHelpLibraryName = (PSZ)szLibName;

    hwndHelp = WinCreateHelpInstance (hab, &hiHelp);
    if (hwndHelp)
        WinAssociateHelpInstance (hwndHelp, hwndLogin);

    /** Fehlermeldung von oben ausgeben, falls notwendig **/
    if (ulErrId)
        WinPostMsg (hwndLogin, WM_MESSAGEBOX, MPFROMLONG (ulErrId), MPVOID);

    /** Shared Memory initialisieren **/
    //     => pShareMem
    if (InitSharedMem (hwndLogin) != 0)
        {
        lGlobalRetCode = SAMERR_SHMEM;
        goto Exit;
        }

    /** Semaphoren anlegen **/
    //     => hmtxPrfReset, hevPrfResetLock
    if (InitSem () != 0)
        {
        lGlobalRetCode = SAMERR_SEM;
        goto Exit;
        }

    if (!bIsWPSac)
        {
        /* FÅr normale WPSam-Umgebung: Shell starten */
        DebugE (D_SAM, "main", "WPSam-Umgebung starten");
        WinSetParent (hwndLogin, HWND_OBJECT, TRUE);
        ChangeWPS ();
        happShell = StartShell ();
        }
    else
        {
        /* WPSac: Login-Fenster anzeigen */
        DebugE (D_SAM, "main", "WPSac-Umgebung starten");
        WinSetParent (hwndLogin, HWND_DESKTOP, TRUE);
        WinSendMsg (hwndLogin, WM_LOGON, MPVOID, MPVOID);
        }

    /* Message-Schleife */
    while (WinGetMsg (hab, &qmsg, 0L, 0L, 0L))
         WinDispatchMsg (hab, &qmsg);

    WinTerminateApp (happShell);        // WPSam-Code???

Exit:
    if (bIsWPSac)
        {
        CloseLocINI ();                 // lokale Profildatei schlie·en
        UnloadAuthorize ();             // DLL entladen

        DebugE (D_SAM, "FreeMem", "main Ende");
        }

    WinDestroyMsgQueue (hmq);
    WinTerminate (hab);

    DebugULd (D_SAM, "main", "RÅckgabewert", lGlobalRetCode);
    return lGlobalRetCode;
    }
