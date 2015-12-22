#define INCL_DOS
#define INCL_DOSPROCESS
#define INCL_DOSERRORS
#define INCL_DOSFILEMGR
#define INCL_WINSHELLDATA

#include <string.h>
#include <stdlib.h>
#include <os2.h>

#include <cutil.h>

#include "samglob.h"
#include "magpro2.h"
#include "MakeIni.h"
#include "prepare.h"
#include "profile.h"
#include "sacalias.h"
#include "SamData.h"
#include "SamDlg.h"
#include "sammy.h"
#include "sammydlg.h"

PCHAR pszRootUserIni, pszRootSystemIni;

/* Benutzer-Initialisierung */
CHAR szUserPath[CCHMAXPATH];
static CHAR szUserResource[CCHMAXPATH];
static CHAR szUserProfile[CCHMAXPATH];
static CHAR szSystemResource[CCHMAXPATH];
static CHAR szSystemProfile[CCHMAXPATH];
static USER User;

/*****************************************************************************
 * Untersucht, ob eine Datei vom Typ WPSAM oder WPSAC ist. Dazu wird der
 * Puffer von DosFind* Åbergeben.
 * Eingang: pszDesktop: Name der ArbeitsoberflÑche
 * return:  DTTYPE_*-Werte
 *****************************************************************************/
ULONG GetDesktopType (PCHAR pszDesktop)
    {
    BOOL bRC;
    CHAR szDtPath[CCHMAXPATH];
    CHAR szPath[CCHMAXPATH];
    STRUC_EAT_DATA arValue[2];

    QueryLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_DT_PATH, szDtPath, sizeof (szDtPath));

    /* Existiert die ArbeitsoberflÑche? */
    DosSearchPath (SEARCH_IGNORENETERRS, szDtPath, pszDesktop, szPath, sizeof (szPath));

#pragma info(none)
    arValue[0].uscValue = max (sizeof (EA_TYPEWPSAM), sizeof (EA_TYPEWPSAC));
#pragma info(restore)
    arValue[0].pValue   = EA_TYPENAME;
    arValue[1].pValue   = NULL;
    bRC =  EAReadMV (szPath, EA_TYPENAME, EAT_MVST, arValue);
    bRC &= ((arValue[0].usEAType == EAT_ASCII) & (arValue[0].uscValue > 0));
    if (!bRC)
        return DTTYPE_NONE;

    /* Welchen Typ hat die ArbeitsoberflÑche? */
    if (strcmp (arValue[0].pValue, EA_TYPEWPSAM) == 0)
        return DTTYPE_WPSAM;
    if (strcmp (arValue[0].pValue, EA_TYPEWPSAC) == 0)
        return DTTYPE_WPSAC;

    return DTTYPE_NONE;
    }

/*****************************************************************************
 * Untersucht, ob eine Datei vom Typ WPSAM oder WPSAC ist. Dazu wird der
 * Puffer von DosFind* Åbergeben.
 * Eingang: pFileFind: Ausgangspuffer von DosFind* (Level 3 Info)
 * return:  TRUE: Typ ist korrekt
 *****************************************************************************/
BOOL IsType (PFILEFINDBUF3 pffb3)
    {
    return GetDesktopType (pffb3->achName) == DTTYPE_NONE ? FALSE : TRUE;
    }

/*****************************************************************************
 * PrÅft, ob ein Benutzer initialisiert ist und ob er privat oder global ist.
 * Eingang: pszUser: kompletter Pfad incl. Benutzername
 * return:  USTAT_*-Werte
 *****************************************************************************/
ULONG IsInitialized (PCHAR pszUser)
    {
    ULONG  ulRC = 0;                    /* User nicht initialisiert */
    BOOL   bRC;
    USHORT usBytes;
    PCHAR  pszProfile;
    CHAR   szPath[CCHMAXPATH];
    CHAR   szPathProfile[CCHMAXPATH];
    FILESTATUS3    fs3;

    /* Ist das Benutzerprofil bereits initialisiert? */
    strcpy (szPath, pszUser);
    strcpy (szPathProfile, pszUser);
    strcat (szPathProfile, "\\");
    pszProfile = szPathProfile + strlen (szPathProfile);
    usBytes = CCHMAXPATH - (pszProfile - szPathProfile);
    EAReadASCII (szPath, EA_USERPROFILE, pszProfile, &usBytes);
    bRC = DosQueryPathInfo (szPathProfile, FIL_STANDARD, &fs3,
        sizeof (FILESTATUS3)) == 0;
    if (bRC && (fs3.cbFile!=0))
        {
        ulRC = USTAT_INITIALIZED;

        /* Ist es ein privater Benutzer und wurde das Systemprofil initialisiert? */
        usBytes = CCHMAXPATH - (pszProfile - szPathProfile);
        EAReadASCII (szPath, EA_SYSTEMPROFILE, pszProfile, &usBytes);
        if (usBytes != 0)
            {
            ulRC |= USTAT_PRIVATE;                  /* privater Benutzer */
            bRC = DosQueryPathInfo (szPathProfile, FIL_STANDARD, &fs3,
                sizeof (fs3)) == 0;
            if (!bRC || (fs3.cbFile==0))
                ulRC &= !USTAT_INITIALIZED;
            }
        }

    return ulRC;
    }

/*****************************************************************************
 * Bestimmt zu einem Benutzernamen den zugehîrigen vollstÑndigen Pfad
 * des Benutzerverzeichnisses. Ist der Benutzername ungÅltig, wird
 * ein DOS-Fehlercode zurÅckgegeben.
 * Eingang: pszUser    : Benutzername
 *          ulcUserPath: Grî·e des Ergebnispuffers in Byte
 * Ausgang: pszUserPath: Ergebnis
 * return:  0          : Ergebnis o.k.
 *          sonst      : DOS-Fehlercode
 *****************************************************************************/
APIRET GetUserPath (PCHAR pszUser, PCHAR pszUserPath, ULONG ulcUserPath)
    {
    PCHAR        pszDTPath;
    CHAR         szString[CCHMAXEALEN];
    CHAR         szPath[CCHMAXPATH];
    CHAR         szDir[CCHMAXPATH];
    CHAR         szUser[CCHMAXPATH];
    USHORT       usBytes;
    ULONG        ulCount, i;
    APIRET       rc;
    HDIR         hDir;
    FILEFINDBUF3 ffb3;

    /* Pfad auf ArbeitsoberflÑchen holen */
    QueryLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_DT_PATH, szPath, sizeof (szPath));
    pszDTPath = szPath;

    while (pszDTPath != NULL)
        {
        /* Pfad+Wildcard erstellen */
        for (i=0;; i++)
            {
            if (*pszDTPath == ';')
                {
                pszDTPath++;
                break;
                }
            else if (*pszDTPath == '\0')
                {
                pszDTPath = NULL;
                break;
                }
            else
                szDir[i] = *pszDTPath++;
            }
        if (szDir[i-1] != '\\')
            i++;

        /* Der Pfad wird nur ausgewertet, wenn er nicht zu lang ist */
        if (i + sizeof (DIR_ALL)-2 < CCHMAXPATH)
            {
            strcpy (szDir+i-1, DIR_ALL);    // diese 2 Zeilen mîglicherweise vertauschen, da
            strcpy (szUser, szDir);         // in szUser sowieso DIR_ALL wieder Åberschrieben wird ???

            /* Dateisuche */
            ulCount = 1;
            hDir = HDIR_CREATE;
            rc = DosFindFirst (szDir, &hDir, MUST_HAVE_DIRECTORY,
                &ffb3, sizeof (FILEFINDBUF3), &ulCount, FIL_STANDARD);

            while (rc==0)
                {
                if (IsType (&ffb3) && (i+ffb3.cchName < CCHMAXPATH))
                    {
                    strcpy (szUser+i, ffb3.achName);
                    usBytes = CCHMAXEALEN;
                    if (EAReadASCII (szUser, EA_LONGNAME, szString, &usBytes))
                        if (stricmp (szString, pszUser) == 0)
                            {
                            if (strlen (szUser) < ulcUserPath)
                                {
                                rc = 0;
                                strcpy (pszUserPath, szUser);
                                }
                            else
                                rc = ERROR_BUFFER_OVERFLOW;
                            DosFindClose (hDir);
                            return rc;
                            }
                    }

                ulCount = 1;
                rc = DosFindNext (hDir, &ffb3, sizeof (FILEFINDBUF3), &ulCount);
                }

            DosFindClose (hDir);
            }
        }

    return rc;
    }

/*****************************************************************************
 * Bestimmt zu einem Benutzernamen alle Informationen, die zum Start
 * einer ArbeitsoberflÑche notwendig sind. Falls die OberflÑche noch
 * nicht initialisiert ist, wird dies mit MAKEINI nachgeholt
 * Eingang: szUser   : Benutzername
 * Ausgang: pulErrId : Fehler-ID
 * return:  TRUE     : Ergebnis o.k.
 *          FALSE    : *pulErrId enthÑlt eine Fehlerstring-ID
 *****************************************************************************/
BOOL PrepareDT (PSZ szUser, PULONG pulErrId)
    {
    SHORT   sPwd;
    USHORT  usBytes, usLen;
    ULONG   ulUserStat;
    ULONG   ulMsgId;
    HWND    hwndMsg;

    *pulErrId = 0;

    /* String mit Benutzernamen prÅfen */
    if (szUser[0] == '\0')
        sPwd = STAT_ROOTUSER;
    else
        {
        if (GetUserPath (szUser, szUserPath, CCHMAXPATH))
            sPwd = STAT_USERNOTFOUND;
        else
            /* kein Kennwort abfragen */
            sPwd = STAT_NOPASSWORD;
        }

    /* Im Fehlerfall den korrekten Fehlertext laden */
    if (sPwd == STAT_USERNOTFOUND)
        {
        *pulErrId = IDS_DESKTPNOTFOUND;
        return FALSE;
        }

    /* Zuerst wird geprÅft, ob die INI-Dateien existieren */
    if (sPwd == STAT_ROOTUSER)
        ulUserStat = USTAT_INITIALIZED;
    else
        ulUserStat = IsInitialized (szUserPath);

    /* Benutzerinitialisierung, falls nîtig */
    if (!(ulUserStat & USTAT_INITIALIZED))
        {
        /* Benutzer noch nicht initialisiert => nachholen */
        User.pszUserName       = szUser;
        User.pszUserPath       = szUserPath;
        User.pszUserProfile    = szUserProfile;
        User.pszUserResource   = szUserResource;
        User.pszSystemProfile  = szSystemProfile;
        User.pszSystemResource = szSystemResource;
        User.pszDesktopName    = TXT_DESKTOP;

        usBytes = CCHMAXPATH;
        EAReadASCII (szUserPath, EA_USERPROFILE, szUserProfile, &usBytes);
        usBytes = CCHMAXPATH;
        EAReadASCII (szUserPath, EA_USERRESOURCE, szUserResource, &usBytes);
        usBytes = CCHMAXPATH;
        EAReadASCII (szUserPath, EA_SYSTEMPROFILE, szSystemProfile, &usBytes);
        usBytes = CCHMAXPATH;
        EAReadASCII (szUserPath, EA_SYSTEMRESOURCE, szSystemResource, &usBytes);

        /* Hier evtl. ein Fenster einbauen ("Benutzer wird initialisiert") ??? */
        ulMsgId = IDS_USERPREPARE;
        hwndMsg = WinLoadDlg (HWND_DESKTOP, HWND_DESKTOP, TimedMsgProc, NULLHANDLE, IDD_TIMEDMSG, &ulMsgId);
        createIni (&User);
        WinDestroyWindow (hwndMsg);
        switch (User.ulErrorMsg)                    // Fehlerbehandlung ???
            {
            case ERR_NOUSERRESOURCE:
                *pulErrId = IDS_USERINCOMPL;
                return FALSE;

            case ERR_NOSYSTEMRESOURCE:
                *pulErrId = IDS_USERINCOMPL;
                return FALSE;

            case ERR_NOUSERINICREATED:
                *pulErrId = IDS_ERRCREATE;
                return FALSE;

            case ERR_NOSYSTEMINICREATED:
                *pulErrId = IDS_ERRCREATE;
                return FALSE;

            case ERR_NODESKINRC:
                *pulErrId = IDS_NODESKINRC;
                return FALSE;

            case ERR_PATHTOOLONG:
                *pulErrId = IDS_PATHTOOLONG;
                return FALSE;
            }

        History (szUserPath, APPNAME, TXT_SETUP);
        }
    else
        {
        History (szUserPath, APPNAME, TXT_PRFRESET);
        }

    /** SAMMY startet nur globale Benutzer bzw. Root **/
    /* USERPROFILE */
    if (sPwd == STAT_ROOTUSER)
        usBytes = 0;
    else
        {
        strcpy (pShareMem->szUserIni, szUserPath);
        strcat (pShareMem->szUserIni, "\\");
        usLen   = strlen (pShareMem->szUserIni);
        usBytes = CCHMAXPATHCOMP - usLen;
        EAReadASCII (szUserPath, EA_USERPROFILE,
            pShareMem->szUserIni + usLen,
            &usBytes);
        }
    if (usBytes == 0)
        pShareMem->szUserIni[0] = '\0';

    /* SYSTEMPROFILE */
    if (sPwd == STAT_ROOTUSER)
        usBytes = 0;
    else
        {
        strcpy (pShareMem->szSystemIni, szUserPath);
        strcat (pShareMem->szSystemIni, "\\");
        usLen   = strlen (pShareMem->szSystemIni);
        usBytes = CCHMAXPATHCOMP - usLen;
        EAReadASCII (szUserPath, EA_SYSTEMPROFILE,
            pShareMem->szSystemIni + usLen,
            &usBytes);
        }
    if (usBytes == 0)
        pShareMem->szSystemIni[0] = '\0';

    /* ENVIRONMENT */
    /* nur WPSam: Environment ist in EAs von szUserPath */
    /*
    if (sPwd == STAT_ROOTUSER)
        usBytes = 0;
    else
        {
        usBytes = CCHSHARE_INITOS2 - sizeof (SHARE1);
        EAReadASCII (szUserPath, EA_ENVNAME, pShareMem->szEnvironment, &usBytes);
        }
    if (usBytes == 0)
        pShareMem->szEnvironment[0] = '\0';
    */

    return TRUE;
    }

/*****************************************************************************
 * Erzeugt ein Benutzerverzeichnis, in dem alle benutzerrelevanten Dateien
 * (ArbeitsoberflÑche, INI-Dateien, SmartCenter-Dateien) abgelegt werden.
 * In die EAs des Verzeichnisses wird der Benutzertyp (WPSac / WPSam),
 * sowie die Dateinamen von RC- und INI-Dateien geschrieben.
 * Eingang: pAlias   : Benutzerbeschreibung
 * Ausgang: pulErrId : Fehler-ID
 * return:  TRUE     : Ergebnis o.k.
 *          FALSE    : *pulErrId enthÑlt eine Fehlerstring-ID
 *****************************************************************************/
BOOL CreateDTDir (PALIASDATA pAlias, PULONG pulErrId)
    {
    ULONG   ulSize, i, ulBootDrive;
    BOOL    bRet;
    STRUC_EAT_DATA arValue[2];

    /* Boot-Laufwerk bestimmen */
    DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof (ULONG));

    *pulErrId = 0;

    if (!(pAlias->ulFlag & ALIAS_DESKTOP))
        {
        *pulErrId = IDS_USERINCOMPL;
        return FALSE;
        }

    /** Benutzerverzeichnis erzeugen **/
    /* Pfad auf ArbeitsoberflÑchen holen */
    QueryLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_DT_PATH, szUserPath, sizeof (szUserPath));
    ulSize = strlen (szUserPath);

    /* Den ersten Pfad verwenden, falls mehrere angegeben */
    for (i = 0; i < ulSize; i++)
        if (szUserPath[i] == ';')
            {
            szUserPath[i] = '\0';
            ulSize = i;
            break;
            }
    if (CCHMAXPATH - ulSize > 1 + strlen (pAlias->szDesktop))
        {
        strcat (szUserPath, "\\");
        strcat (szUserPath, pAlias->szDesktop);
        }
    else
        {
        *pulErrId = IDS_PATHTOOLONG;
        return FALSE;
        }

    /* Benutzerverzeichnis anlegen */
    if (DosCreateDir (szUserPath, NULL))
        {
        *pulErrId = IDS_ERRCREATE;
        return FALSE;
        }

    /* Erweiterte Attribute des neuen Benutzers schreiben */
    bRet  = EAWriteASCII (szUserPath, EA_LONGNAME, pAlias->szDesktop);

    arValue[0].usEAType = arValue[1].usEAType = EAT_ASCII;
    arValue[0].pValue   = EA_TYPEWPSAC; //???
    arValue[0].uscValue = sizeof (EA_TYPEWPSAC) - 1;
    arValue[1].pValue   = NULL;
    bRet &= EAWriteMV (szUserPath, EA_TYPENAME, EAT_MVST, arValue);

    QueryLocINIString (MAGPRO_FILE_APP,
                       MAGPRO_BASERESOURCE_KEY,
                       szUserResource,              // was ist mit Bootlaufwerk bei Pfaden aus INI-File???
                       CCHMAXPATH);

    /* Defaultstring verwenden, falls MAGPRO_BASERESOURCE_KEY nicht vorhanden oder Leerstring war */
    if (szUserResource[0] == '\0')
        {
        strcpy (szUserResource, "?:\\os2\\ini.rc");
        szUserResource[0] = (CHAR) (ulBootDrive - 1) + 'A';     // Boot-Laufwerk eintragen
        }

    bRet &= EAWriteASCII (szUserPath, EA_USERRESOURCE, szUserResource);

    szSystemResource[0] = szSystemProfile[0] = '\0';
    bRet &= EAWriteASCII (szUserPath, EA_SYSTEMRESOURCE, szSystemResource);
    bRet &= EAWriteASCII (szUserPath, EA_SYSTEMPROFILE,  szSystemProfile);

    strcpy (szUserProfile, "os2.ini");
    bRet &= EAWriteASCII (szUserPath, EA_USERPROFILE, szUserProfile);

    if (!bRet)
        {
        DosDeleteDir (szUserPath);
        *pulErrId = IDS_ERRCREATE;
        bRet = FALSE;
        }

    return bRet;
    }
