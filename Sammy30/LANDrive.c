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
#define INCL_DOSERRORS
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_WINCOUNTRY
#define INCL_WINSHELLDATA
#include <os2.h>
#include <string.h>

#include "LSPrelog.h"
#include "magpro2.h"
#include "profile.h"
#include "Sammy.h"
#include "SammyDlg.h"
#include "SamDlg.h"
#include "SamData.h"

/* Prologon entry points */
static HMODULE  hModule                  = NULLHANDLE;
static BOOL (* APIENTRY pfnLogon)(void)  = NULL;
static BOOL (* APIENTRY pfnLogoff)(void) = NULL;

static CHAR szErrorText[CCHMAXPATHCOMP];                            // Fehlertext von DosLoadModule

ULONG ConvertToStringId (ULONG ulErrCode)
    {
    ULONG    i;
    ULONG    arErrMsg[] = {PRLERR_INVDOMAIN,
                           PRLERR_INVPATH,
                           PRLERR_NOPRELOGUSER,
                           PRLERR_LOGON,
                           PRLERR_LOGOFF,
                           0};
    ULONG    arErrId[]  = {IDS_INVDOMAIN,
                           IDS_INVPATH,
                           IDS_NOPRELOGUSER,
                           IDS_LOGON,
                           IDS_LOGOFF};
    for (i = 0; arErrMsg[i] != 0; i++)
        if (arErrMsg[i] == ulErrCode)
            return arErrId[i];

    return 0;
    }

VOID MountLANDirectory (VOID)
    {
    HWND     hwndMsg;
    ULONG    idMsgString;
    ULONG    ulLength;
    ULONG    ulRc;
    CHAR     szPrelogonDomain[CCHMAXDOMAINNAME];    // Prelogon-DomÑnenname
    APIRET   rc;

    if (pfnLogon)
        return;                     // bereits eingeloggt

    /* bestimmen, ob LAN-Server-Anmeldung durchgefÅhrt werden soll */
    ulLength = QueryLocINIString (MAGPRO_PRELOGON_APP, MAGPRO_KEY_PRLOG_DOMAIN, szPrelogonDomain, CCHMAXDOMAINNAME);

    if (ulLength > strlen (SIGNATURE_LS) &&
        memcmp (szPrelogonDomain, SIGNATURE_LS, strlen (SIGNATURE_LS)) == 0)
        {
        /* IBM LAN-Server */
        rc = DosLoadModule (szErrorText, CCHMAXPATHCOMP, "LSPRELOG", &hModule);
        if (rc == NO_ERROR)
            {
            rc  = DosQueryProcAddr (hModule, 0, "DoPreLogon",  (PFN *)&pfnLogon);
            rc |= DosQueryProcAddr (hModule, 0, "DoPreLogoff", (PFN *)&pfnLogoff);
            if (rc)
                {
                MessageBox (HWND_DESKTOP, IDS_NODLL, MBTITLE_ERROR, MB_OK | MB_ERROR);
                pfnLogon  = NULL;
                pfnLogoff = NULL;
                DosFreeModule (hModule);
                hModule = NULLHANDLE;
                }

            /* Messagebox anzeigen */
            idMsgString = IDS_PRELOGON;
            hwndMsg = WinLoadDlg (HWND_DESKTOP, HWND_DESKTOP, TimedMsgProc, NULLHANDLE, IDD_TIMEDMSG, &idMsgString);

            /* Logon rufen */
            ulRc = (pfnLogon)();
            if (ulRc)
                {
                MessageBox (HWND_DESKTOP, ConvertToStringId (ulRc), MBTITLE_ERROR, MB_OK | MB_ERROR);
                pfnLogon  = NULL;
                pfnLogoff = NULL;
                DosFreeModule (hModule);
                hModule = NULLHANDLE;
                }

            /* Messagebox schlie·en */
            WinDestroyWindow (hwndMsg);
            }
        else
            MessageBox (HWND_DESKTOP, IDS_NODLL, MBTITLE_ERROR, MB_OK | MB_ERROR);
        }

    return;
    }

VOID Wait4LANDirectory (VOID)
    {
    CHAR       c;
    LONG       lLANTimeout;
    APIRET     rc;
    FSALLOCATE fs;

    lLANTimeout = PrfQueryProfileInt (HINI_SYSTEMPROFILE,
                                      OS2SYS_APP_NAME,
                                      OS2SYS_KEY_LANTIMEOUT,
                                      0);

    if (lLANTimeout != 0 && strlen (szPathLAN) > 2 && szPathLAN[1] == ':')
        {
        while (lLANTimeout-- > 0)
            {
            c  = WinUpperChar (hab, 0, 0, szPathLAN[0]) - 'A' + 1;
            rc = DosQueryFSInfo (c, FSIL_ALLOC, &fs, sizeof (FSALLOCATE));
            if (rc != ERROR_INVALID_DRIVE)
                break;
            DosSleep (1000);
            }
        }

    return;
    }

/* PrÅfen, ob eine Kopieraktion sinnvoll ist */
BOOL IsCopy (PSZ pszDst, PSZ pszSrc)
    {
    /* Pfade mÅssen absolut sein, daher mu· am  */
    /* Stringanfang ein Laufwerksbuchstabe sein */
    if (*pszDst == '\0' || *pszSrc == '\0')
        return FALSE;

    /* Ende, falls Quelle == Ziel */
    if (strcmp (pszDst, pszSrc) == 0)
        return FALSE;

    return TRUE;
    }

BOOL CopyFiles (PSZ pszDst, PSZ pszSrc, PSZ pszFiles)
    {
    PSZ          pszEndSrc, pszEndDst;
    ULONG        ulLenSrc, ulLenDst;
    HDIR         hDir = HDIR_CREATE;
    FILEFINDBUF3 ffb3;
    ULONG        ulNum;
    APIRET       rc;

    if (!IsCopy (pszDst, pszSrc))
        return FALSE;

    ulLenSrc = strlen (pszSrc);
    ulLenDst = strlen (pszDst);

    /* Ende, falls Puffer zu klein. Achtung: ">", da sizeof '\0' mitzÑhlt */
    if ((ulLenSrc + sizeof ("\\") + strlen (pszFiles) > CCHMAXPATH) ||
        (ulLenDst + sizeof ("\\")                     > CCHMAXPATH))
        return FALSE;

    pszEndDst = pszDst  + ulLenDst;
    pszEndSrc = pszSrc  + ulLenSrc;
    strcpy (pszEndDst, "\\");
    strcpy (pszEndSrc, "\\");
    strcat (pszEndSrc, pszFiles);

    ulNum = 1;
    rc = DosFindFirst (pszSrc, &hDir, FILE_NORMAL, &ffb3, sizeof (ffb3), &ulNum, FIL_STANDARD);
    while (rc == NO_ERROR && ulNum == 1)
        {
        if ((ulLenSrc + strlen (ffb3.achName) < CCHMAXPATH) &&
            (ulLenDst + strlen (ffb3.achName) < CCHMAXPATH))
            {
            strcpy (pszEndSrc+1, ffb3.achName);
            strcpy (pszEndDst+1, ffb3.achName);
            }
        DosCopy (pszSrc, pszDst, DCPY_EXISTING);
        rc = DosFindNext (hDir, &ffb3, sizeof (ffb3), &ulNum);
        }
    DosFindClose (hDir);

    *pszEndSrc = *pszEndDst = '\0';
    return TRUE;
    }

/*******************************************************************
   CopyLANDir: kopiert Dateien aus dem (globalen) LAN-Verzeichnis
   in das lokale Verzeichnis. Vorher werden maschinenlokale EintrÑge
   aus der Profildatei gesichert und nach dem Kopiervorgang in die
   neue Profildatei zurÅckgeschrieben.
   Eingang: pszDst: (lokales) Zielverzeichnis
            pszSrc: (globales) Quellverzeichnis
   return: 0:     ok
           sonst: Message-Id
 *******************************************************************/
ULONG CopyLANDir (PSZ pszDst, PSZ pszSrc)
    {
    BOOL bRC;

    bRC = IsCopy (pszDst, pszSrc);
    if (!bRC)
        return 0;

    SaveLocalINIEntries ();
    CloseLocINI ();
    bRC = CopyFiles (pszDst, pszSrc, "*");
    if (!OpenLocINI ())
        return IDS_NOINIDATABASE;
    if (bRC)
        {
        if (RestoreLocalINIEntries ())
            return IDS_DTPATHCHANGED;

        // Test: Falls LAN-Verzeichnis schreibbar ist,
        // kann jetzt der ToDo-Cache abgearbeitet werden
        HandleTodoLists ();
        }

    return 0;
    }

VOID UnmountLANDirectory (VOID)
    {
    if (hModule && pfnLogoff)
        {
        (pfnLogoff)();

        pfnLogon  = NULL;
        pfnLogoff = NULL;

        DosFreeModule (hModule);
        hModule = NULLHANDLE;
        }

    return;
    }

