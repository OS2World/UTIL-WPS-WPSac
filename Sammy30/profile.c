#define INCL_DOSPROCESS
#define INCL_WINSHELLDATA
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "SamGlob.h"

#include "magpro2.h"
#include "profile.h"
#include "SamMem.h"
#include "SamData.h"
#include "SammyDlg.h"
#include "debug.h"
#include "sammy.h"

typedef enum
    {
    ActionNothing,
    ActionWarning,
    ActionTimeStamp
    } INIACTION;

typedef struct _INIENTRY
    {
    PSZ                pszAppName;
    PSZ                pszKeyName;
    INIACTION          enAction;
    BYTE              *pbValue;
    ULONG              ulcValue;
    ULONG              ulTimestamp;
    struct _INIENTRY  *pNext;
    } INIENTRY, *PINIENTRY;

static HINI hiniAlias = NULLHANDLE;         // Handle der lokalen INI-Datei

static PINIENTRY pFirstIniEntry = NULL;

PSZ QueryProfile (PSZ szPath, ULONG ulLen, BOOL bLocalIni)
    {
    ULONG ulLenPath, ulLenFile;

    /* 1. INI-Pfad holen */
    ulLenPath = PrfQueryProfileString (HINI_SYSTEMPROFILE,
                                       OS2SYS_APP_NAME,
                                       bLocalIni ? OS2SYS_KEY_CLNT_LOCAL : OS2SYS_KEY_CLNT_LAN,
                                       NULL,
                                       szPath,
                                       ulLen);
    if (szPath[ulLenPath-1] != '\0')
        szPath[ulLenPath] = '\0';
    else
        ulLenPath--;

    /* 2. INI-Datei îffnen */
    if (szPath[0] != '\0')
        {
        szPath[ulLenPath] = '\\';
        ulLenFile = PrfQueryProfileString (HINI_SYSTEMPROFILE,
                                           OS2SYS_APP_NAME,
                                           OS2SYS_KEY_INI_FILE,
                                           NULL,
                                           szPath+ulLenPath+1,
                                           ulLen-ulLenPath-1);
        if (ulLenFile > 0)
            {
            szPath[ulLenPath + ulLenFile + 1] = '\0';
            return szPath;
            }
        }

    return NULL;
    }

BOOL IsLANProfile (VOID)
    {
    CHAR szLANPath[CCHMAXPATH];
    CHAR szLocPath[CCHMAXPATH];

    PCHAR pszLANPath = QueryProfile (szLANPath, CCHMAXPATH, FALSE);
    PCHAR pszLocPath = QueryProfile (szLocPath, CCHMAXPATH, TRUE);

    if (pszLANPath == NULL)
        {
        DebugE (D_SAM, "IsNoLANProfile, Fehler: LANPath == NULL", szLANPath);
        return FALSE;
        }

    if (pszLocPath == NULL)
        {
        DebugE (D_SAM, "IsNoLANProfile, Fehler: LocPath == NULL", szLocPath);
        return FALSE;
        }

    return strcmp (pszLocPath, pszLANPath) != 0 ? TRUE : FALSE;
    }

HINI OpenLANProfile (VOID)
    {
    HINI hini = NULLHANDLE;
    CHAR szLANPath[CCHMAXPATH];

    PCHAR pszLANPath = QueryProfile (szLANPath, CCHMAXPATH, FALSE);

    if (IsLANProfile ())
        hini = PrfOpenProfile (hab, pszLANPath);

    return hini;
    }

/*******************************************************************\
 *  Funktionen fÅr die lokale INI-Datei                            *
\*******************************************************************/

BOOL OpenLocINI (VOID)
    {
    CHAR  szIniPath[CCHMAXPATH];

    if (hiniAlias)
        {
        PrfCloseProfile (hiniAlias);
        hiniAlias = NULLHANDLE;
        }

    if (QueryProfile (szIniPath, CCHMAXPATH, TRUE))
        hiniAlias = PrfOpenProfile (hab, szIniPath);

    return hiniAlias ? TRUE : FALSE;
    }

HINI QueryLocINIHandle (VOID)
    {
    return hiniAlias;
    }

BOOL QueryLocINISize (PSZ pszApp, PSZ pszKey, PULONG pulc)
    {
    if (!hiniAlias)
        {
        *pulc = 0;
        return FALSE;
        }

    return PrfQueryProfileSize (hiniAlias, pszApp, pszKey, pulc);
    }

ULONG QueryLocINIString (PSZ pszApp, PSZ pszKey, PSZ pszDest, ULONG ulc)
    {
    ULONG ulSize;

    if (!hiniAlias)
        {
        if (ulc > 0)
            *pszDest = '\0';
        return 0;
        }

    ulSize = PrfQueryProfileString (hiniAlias, pszApp, pszKey, "", pszDest, ulc);
    if (ulSize > 0 && pszDest[ulSize-1])
        {
        if (ulSize >= ulc)          // keine abgeschnittenen String zulassen
            ulSize = 0;
        pszDest[ulSize] = '\0';     // String terminieren
        }

    return ulSize;
    }

BOOL QueryLocINIData (PSZ pszApp, PSZ pszKey, PVOID pDest, PULONG pulc)
    {
    BOOL bRC = FALSE;

    if (hiniAlias)
        bRC = PrfQueryProfileData (hiniAlias, pszApp, pszKey, pDest, pulc);
    else
        *pulc = 0;

    return bRC;
    }

BOOL QueryLocINICryptedData (PSZ pszApp, PSZ pszKey, PCHAR pDest, ULONG ulc)
    {
    ULONG ul  = ulc;
    BOOL  bRC = QueryLocINIData (pszApp, pszKey, pDest, &ul);

    /* kein Fehler beim Lesen aufgetreten => entschlÅsseln */
    if (bRC)
        return CRYPT_STRING (pDest, ulc, CA_Decrypt);

    return bRC;
    }

LONG QueryLocINIInt (PSZ pszApp, PSZ pszKey, LONG lDefault)
    {
    if (hiniAlias)
        return PrfQueryProfileInt (hiniAlias, pszApp, pszKey, lDefault);

    return lDefault;
    }

BOOL WriteLocINIString (PSZ pszApp, PSZ pszKey, PSZ pszSource)
    {
    if (hiniAlias)
        return PrfWriteProfileString (hiniAlias, pszApp, pszKey, pszSource);

    return FALSE;
    }

BOOL WriteLocINICryptedData (PSZ pszApp, PSZ pszKey, PSZ pSource, ULONG ulc)
    {
    CRYPT_STRING (pSource, ulc, CA_Crypt);

    if (hiniAlias)
        return PrfWriteProfileData (hiniAlias, pszApp, pszKey, pSource, ulc);

    return FALSE;
    }

VOID CloseLocINI (VOID)
    {
    if (hiniAlias)
        {
        PrfCloseProfile (hiniAlias);
        hiniAlias = NULLHANDLE;
        }

    return;
    }

/*******************************************************************
   QueryLocINIStringFlag: Lesen eines Strings aus einer Profildatei.
   ZusÑtzlich zur Funktion des API-Calls PrfQueryProfileString
   wird im Falle eines gefundenen Strings ein Flag im Åbergebenen
   Flagwort gesetzt, im umgekehrten Fall gelîscht.
   Eingang: hini:       INI-Datei Handle
            pszApp:     Application name
            pszKey:     Key name
            pszDest:    Zielpuffer
            ulc:        LÑnge des Zielpuffers
            pulFlag:    Zeiger auf Flagwort (ULONG)
            ulFlag:     zu setzende/lîschende Flagmaske
   Ausgang: pszDest:    gelesener String
            pulFlag:    Zeiger auf bearbeitete Flags
 *******************************************************************/
LONG QueryLocINIStringFlag (PSZ pszApp, PSZ pszKey,
                         PSZ pszDest, ULONG ulc,
                         PULONG pulFlag, ULONG ulFlag)
    {
    ULONG ulRead = 0;

    if (hiniAlias)
        ulRead = PrfQueryProfileString (hiniAlias, pszApp, pszKey,
                                        NULL,
                                        pszDest, ulc);

    if (ulRead && hiniAlias)
        {
        *pulFlag |= ulFlag;
        }
    else
        {
        if (ulc > 0)
            *pszDest = '\0';
        *pulFlag &= ~ulFlag;
        }

    return ulRead;
    }

BOOL WriteAdminFlag (PSZ pszUser, BOOL bIsAdmin)
    {
    CHAR sz[CCHMAXUSERNAME+1];

    strcpy (sz, pszUser);

    if (bIsAdmin)
        strcat (sz, "1");
    else
        strcat (sz, "0");

    return WriteLocINICryptedData (pszUser, MAGPRO_ADMINFLAG_KEY, sz, CCHMAXUSERNAME+1);
    }

BOOL ReadAdminFlag (PSZ pszUser)
    {
    CHAR sz[CCHMAXUSERNAME+1];

    if (QueryLocINICryptedData (pszUser, MAGPRO_ADMINFLAG_KEY, sz, CCHMAXUSERNAME+1))
        if (memcmp (pszUser, sz, min (strlen (pszUser), CCHMAXUSERNAME)) == 0)
            return sz[strlen (pszUser)] == '1';

    return FALSE;
    }

ULONG GetTimestamp (PSZ pszApp)
    {
    ULONG ulc;
    BOOL  bRC;
    CHAR  bBuffer[CCHMAXINT];

    bRC = PrfQueryProfileSize (hiniAlias, pszApp, MAGPRO_LASTMOD_KEY, &ulc);
    if (bRC && ulc)
        {
        bRC = PrfQueryProfileData (hiniAlias, pszApp, MAGPRO_LASTMOD_KEY, bBuffer, &ulc);
        if (bRC && ulc)
            return atoi (bBuffer);
        }

    return 0;
    }

VOID SetTimestamp (PSZ pszApp)
    {
    time_t timestamp;
    CHAR   sz[CCHMAXINT];

    if (hiniAlias)
        {
        timestamp = time (NULL);
        _itoa (timestamp, sz, 10);
        PrfWriteProfileString (hiniAlias, pszApp, MAGPRO_LASTMOD_KEY, sz);
        }

    return;
    }

/********************************************************************
 *  Bestimmt, wie lange WPSac-Pa·wîrter gÅltig bleiben.
 *  return:  Intervall in Tagen; 0 = kein Eintrag
 ********************************************************************/
ULONG GetPwdValidIntvl (VOID)
    {
    ULONG ulSize;
    BOOL  bRC;
    ULONG value = 0;

    if (hiniAlias)
        {
        ulSize = sizeof (ULONG);
        bRC = PrfQueryProfileData (hiniAlias, MAGPRO_SYSTEM_APP, MAGPRO_PWD_INTVL_KEY, &value, &ulSize);

        if (bRC == FALSE || ulSize != sizeof (ULONG))
            value = 0;
        }

    DebugULd (D_SAM, "GetPwdValidIntvl", "Acceptance interval", value);
    return value;
    }

/********************************************************************
 *  Bestimmt, ob fÅr ein WPSac-Pa·wort ein Timestamp geschrieben
 *  wurde.
 *  Eingang: pszUser:    WPSac-Username
 *  return:  TRUE:  Timestamp vorhanden
 *           FALSE: kein Timestamp vorhanden
 ********************************************************************/
BOOL IsPwdAgeSet (PSZ pszUser)
    {
    ULONG ul;
    BOOL  bRC = FALSE;

    if (hiniAlias)
        {
        PrfQueryProfileSize (hiniAlias, pszUser, MAGPRO_PASSWORDAGE_KEY, &ul);
        DebugE (D_SAM, "IsPwdAgeSet", ul > 1 ? "PWD-timestamp exists" : "NO PWD TIMESTAMP!");
        bRC = ul > 1 ? TRUE : FALSE;
        }

    return bRC;
    }

/********************************************************************
 *  Setzt einen Timestamp fÅr ein WPSac-Pa·wort zur Altersbestimmung
 *  Eingang: pszUser:    WPSac-Username
 ********************************************************************/
VOID SetPwdAge (PSZ pszUser)
    {
    time_t timePwd;
    CHAR   sz[CCHMAXINT];

    if (hiniAlias)
        {
        timePwd = time (NULL);
        _itoa (timePwd, sz, 10);
        PrfWriteProfileString (hiniAlias, pszUser, MAGPRO_PASSWORDAGE_KEY, sz);
        }

    return;
    }

/********************************************************************
 *  Bestimmt das Alter eines WPSac-Pa·wortes in Tagen
 *  Eingang: pszUser:    WPSac-Username
 *  return:  Alter in Tagen; 0 = kein Eintrag
 ********************************************************************/
ULONG GetPwdAge (PSZ pszUser)
    {
    time_t timePwd;
    CHAR   sz[CCHMAXINT];
    ULONG  ul;
    ULONG  ulAge = 0;

    if (hiniAlias)
        {
        /* Pa·wortalter bestimmen */
        ul = PrfQueryProfileString (hiniAlias, pszUser, MAGPRO_PASSWORDAGE_KEY, NULL, sz, CCHMAXINT);
        if (ul)
            {
            timePwd = atoi (sz);
            ulAge = timePwd ? (LONG)difftime (time (NULL), timePwd) / (60*60*24) : 0;
            }
        }

    DebugULd (D_SAM, "GetPwdAge", "Age of Password", ulAge);
    return ulAge;
    }

/********************************************************************
 *  PrÅft, ob lokal mindestens 1 Benutzer bekannt ist
 *  Falls nicht, wird ein Default-Root-Benutzer angelegt
 *  (User = "ROOT", Password = "")
 *  return: TRUE:  es existiert mind. 1 User
 *          FALSE: Das Anlegen ging schief
 ********************************************************************/
BOOL CreateCheck1User (VOID)
    {
    PSZ   pszAppName, pszAppNames;
    CHAR  szDefUser[CCHMAXUSERNAME];
    CHAR  szDefPassword[CCHMAXPASSWORD];
    ULONG ulc;
    BOOL  bRC = FALSE;

    if (!hiniAlias)
        return FALSE;

    /* Suchen von BenutzereintrÑgen (beginnen nicht mit "_") */
    if (PrfQueryProfileSize (hiniAlias, NULL, NULL, &ulc))
        {
        pszAppName = pszAppNames = AllocMem (ulc);
        if (pszAppNames)
            {
            PrfQueryProfileData (hiniAlias, NULL, NULL, pszAppNames, &ulc);
            if (ulc)
                {
                /* PrÅfen, ob 1 User existiert */
                while (pszAppName[0] != '\0')
                    {
                    if (pszAppName[0] != '_')
                        {
                        bRC = TRUE;
                        break;
                        }
                    pszAppName += strlen (pszAppName) + 1;
                    }
                }
            FreeMem ((PPVOID)&pszAppNames);
            }
        }

    if (!bRC)
    {
    /* Es existiert noch kein User => "ROOT" anlegen */
    WinLoadString (hab,
                   NULLHANDLE,
                   IDS_DEFAULTUSER,
                   CCHMAXUSERNAME,
                   szDefUser);
    WinLoadString (hab,
                   NULLHANDLE,
                   IDS_DEFAULTPASSWORD,
                   CCHMAXPASSWORD,
                   szDefPassword);

    bRC = WriteLocINICryptedData (szDefUser, MAGPRO_PASSWORD_KEY, szDefPassword, CCHMAXPASSWORD);
    }

    return bRC;
    }

/*******************************************************************\
 *  Funktionen fÅr die Verwaltung von Service-Daten in der         *
 *  lokalen Profildatei. Jeder Service-Daten-Eintrag ist fÅr ein   *
 *  Logon/Logoff-Skript zustÑndig                                  *
\*******************************************************************/

/********************************************************************
 *  Liest einen kompletten Service-Daten-String (bestehend aus
 *  MAXSTRING Teilstrings)
 *  Eingang: pszUser:         WPSac-Username
 *           pszService:      Service-Name ("L_*")
 *           Argv[MAXSTRING]: Leerstrings
 *  Ausgang: Argv[MAXSTRING]: Array der Einzelstrings
 ********************************************************************/
VOID ReadServiceData (PSZ pszUser, PSZ pszService, RXSTRING Argv[])
    {
    PCHAR      pcLogonData;
    ULONG      ulSize, ulc, ulIndex;
    BOOL       bRC;

    /* REXX-Strings initialisieren */
    for (ulc = 0; ulc < MAXSTRING; ulc++)
        {
        Argv[ulc].strlength = 0;
        Argv[ulc].strptr    = NULL;
        }

    /* GesamtlÑnge des INI-Eintrages bestimmen */
    bRC = QueryLocINISize (pszUser, pszService, &ulSize);
    if (!bRC || !ulSize || (ulSize & 7))
        return;

    /* Daten lesen */
    pcLogonData = AllocMem (ulSize);

    if (QueryLocINICryptedData (pszUser, pszService, pcLogonData, ulSize))
        {
        /* Strings aus Profile (Reihenfolge s. _string) einlesen */
        for (ulc = ulIndex = 0; ulc < MAXSTRING && ulIndex < ulSize; ulc++)
            {
            if (Argv[ulc].strptr)
                DosFreeMem (Argv[ulc].strptr);
            Argv[ulc].strlength = strlen (&pcLogonData[ulIndex]);
            DosAllocMem ((PPVOID)&Argv[ulc].strptr, Argv[ulc].strlength + 1, PAG_COMMIT | PAG_READ | PAG_WRITE);
            memcpy (Argv[ulc].strptr, &pcLogonData[ulIndex], Argv[ulc].strlength + 1);
            ulIndex += Argv[ulc].strlength + 1;
            }
        }

    DebugE (D_SAM, "FreeMem", "Free LogonData1");
    FreeMem ((PPVOID)&pcLogonData);
    DebugE (D_SAM, "FreeMem", "Free LogonData2");
    return;
    }

/********************************************************************
 *  ServiceData bezeichnet den Inhalt eines Eintrages in der
 *  INI-Datei fÅr einen "Login-Service", also einer einzelnen
 *  Rexx-Prozedur. Der entsprechende Keyname beginnt mit "L_".
 *  Der Eintrag besteht aus den einzelnen Teilstrings:
 *  USERNAME, PASSWORD, DOMAIN, LOGIN, LOGOUT, CHANGEPWD,
 *  PWDAGE, NEWPASSWORD, NEWPWDAGE
 *  Diese Funktion liest einen String mit der Nummer lString
 *  Eingang: pszUser:    WPSac-Username
 *           pszService: Service-Name ("L_*")
 *           lString:    String-Nummer
 *           prxString:  Leerstring
 *  Ausgang: prxString:  gelesener String
 ********************************************************************/
BOOL ReadServiceDataString (PSZ pszUser, PSZ pszService, LONG lString, PRXSTRING prxString)
    {
    ULONG    ulc;
    RXSTRING arxString[MAXSTRING];

    if (hiniAlias == 0)
        return FALSE;

    if (lString < MAXSTRING)
        {
        ReadServiceData (pszUser, pszService, arxString);
        if (prxString->strptr)
            DosFreeMem (prxString->strptr);
        prxString->strlength      = arxString[lString].strlength;
        prxString->strptr         = arxString[lString].strptr;
        arxString[lString].strptr = NULL;
        }

    for (ulc = 0; ulc < MAXSTRING; ulc++)
        {
        if (arxString[ulc].strptr != NULL)
            {
            DebugE (D_SAM, "FreeMem", "WSD1");
            DosFreeMem (arxString[ulc].strptr);
            DebugE (D_SAM, "FreeMem", "WSD2");
            }
        }

    return TRUE;
    }

/********************************************************************
 *  Schreibt einen kompletten Service-Daten-String (bestehend aus
 *  MAXSTRING Teilstrings)
 *  Eingang: pszUser:         WPSac-Username
 *           pszService:      Service-Name ("L_*")
 *           Argv[MAXSTRING]: Array der Einzelstrings
 ********************************************************************/
VOID WriteServiceData (PSZ pszUser, PSZ pszService, RXSTRING Argv[])
    {
    PCHAR      pcLogonData;
    ULONG      ulSize, ulc, ulIndex;

    /* GesamtlÑnge des INI-Eintrages bestimmen */
    for (ulSize = ulc = 0; ulc < MAXSTRING; ulc++)
        ulSize += Argv[ulc].strlength + 1;

    if (!ulSize)
        return;

    /* String zusammenstellen (LÑnge mu· wegen VerschlÅsselung durch 8 teilbar sein */
    ulSize = (ulSize >> 3) << 3;
    pcLogonData = (PSZ)AllocMem (ulSize);

    for (ulIndex = ulc = 0; ulc < MAXSTRING; ulc++)
        {
        if (Argv[ulc].strlength > 0)
            {
            memcpy (pcLogonData + ulIndex, Argv[ulc].strptr, Argv[ulc].strlength+1);
            ulIndex += Argv[ulc].strlength + 1;
            }
        else
            pcLogonData[ulIndex++] = '\0';
        }

    /* Daten schreiben */
    if (WriteLocINICryptedData (pszUser, pszService, pcLogonData, ulSize))
        {
        /* "Todo-Daten" fÅr das globale INI-File schreiben */
        AddToTodoList (pszUser, pszService);
        }

    DebugE (D_SAM, "FreeMem", "Free ServData1");
    FreeMem ((PPVOID)&pcLogonData);
    DebugE (D_SAM, "FreeMem", "Free ServData2");
    return;
    }

/********************************************************************
 *  ServiceData bezeichnet den Inhalt eines Eintrages in der
 *  INI-Datei fÅr einen "Login-Service", also einer einzelnen
 *  Rexx-Prozedur. Der entsprechende Keyname beginnt mit "L_".
 *  Der Eintrag besteht aus den einzelnen Teilstrings:
 *  USERNAME, PASSWORD, DOMAIN, LOGIN, LOGOUT, CHANGEPWD,
 *  PWDAGE, NEWPASSWORD, NEWPWDAGE
 *  Diese Funktion schreibt den String mit der Nummer lString
 *  Eingang: pszUser:    WPSac-Username
 *           pszService: Service-Name ("L_*")
 *           lString:    String-Nummer
 *           prxString:  zu schreibender String
 ********************************************************************/
BOOL WriteServiceDataString (PSZ pszUser, PSZ pszService, LONG lString, PRXSTRING prxString)
    {
    ULONG    ulc;
    RXSTRING arxString[MAXSTRING];

    if (hiniAlias == 0)
        return FALSE;

    if (lString < MAXSTRING)
        {
        ReadServiceData (pszUser, pszService, arxString);
        DebugE (D_SAM, "FreeMem", "arxString1");
        DosFreeMem (arxString[lString].strptr);
        DebugE (D_SAM, "FreeMem", "arxString2");
        arxString[lString] = *prxString;
        WriteServiceData (pszUser, pszService, arxString);
        arxString[lString].strptr = NULL;
        }

    for (ulc = 0; ulc < MAXSTRING; ulc++)
        {
        if (arxString[ulc].strptr != NULL)
            {
            DebugE (D_SAM, "FreeMem", "WSD1");
            DosFreeMem (arxString[ulc].strptr);
            DebugE (D_SAM, "FreeMem", "WSD2");
            }
        }

    return TRUE;
    }

/*******************************************************************\
 *  Funktionen fÅr die Verwaltung des ToDo-Caches zur              *
 *  Synchronisation von lokalem und LAN-Profil                     *
\*******************************************************************/

/********************************************************************
 *  In der lokalen INI-Datei kann bei einem Benutzer ein Key
 *  MAGPRO_TOCOPY_KEY existieren, der eine Folge von ASCIIZ-Strings
 *  enthÑlt. Jeder dieser Strings bezeichnet den Key, der nach
 *  einem erfolgreichen Einlog-Vorgang in das globale INI-File
 *  kopiert werden mu· ("ToDo-Liste").
 *  Diese Funktion ergÑnzt einen Eintrag in dieser Liste.
 *  Eingang: pszUser:   WPSac-Username
 *           pszString: Zu ergÑnzender Eintrag
 ********************************************************************/
VOID AddToTodoList (PSZ pszUser, PSZ pszString)
    {
    PCHAR pcToDoString;
    ULONG ulSize, ulIndex, ulc;

    if (!hiniAlias)
        return;

    PrfQueryProfileSize (hiniAlias, pszUser, MAGPRO_TOCOPY_KEY, &ulSize);
    ulSize += strlen (pszString) + 1;
    pcToDoString = AllocMem (ulSize);
    ulIndex = ulSize;
    if (PrfQueryProfileData (hiniAlias, pszUser, MAGPRO_TOCOPY_KEY, pcToDoString, &ulIndex) == FALSE)
        ulIndex = 0;

    /* Ist der Eintrag bereits vorhanden? */
    for (ulc = 0; ulc < ulIndex; ulc += strlen (pcToDoString) + 1)
        {
        /* Jeden Teilstring in pcToDoString untersuchen */
        if (strcmp (&pcToDoString[ulc], pszString) == 0)
            break;
        }
    if (ulc >= ulIndex)
        {
        /* Eintrag ist noch nicht vorhanden */
        strcpy (&pcToDoString[ulIndex], pszString);
        PrfWriteProfileData (hiniAlias, pszUser, MAGPRO_TOCOPY_KEY, pcToDoString, ulSize);
        }

    /* Timestamp korrigieren */
    SetTimestamp (pszUser);

    DebugE (D_SAM, "FreeMem", "AddToDoList1");
    FreeMem ((PPVOID)&pcToDoString);
    DebugE (D_SAM, "FreeMem", "AddToDoList2");
    return;
    }

BOOL HandleTodoLists (VOID)
    {
    ULONG ulSize;
    BOOL  bRC;
    BOOL  bReturn = TRUE;
    PSZ   pszz, pszUser;

    if (!hiniAlias)
        return FALSE;

    bRC = PrfQueryProfileSize (hiniAlias, NULL, NULL, &ulSize);
    if (bRC && ulSize)
        {
        pszUser = pszz = AllocMem (ulSize);
        if (pszz)
            {
            bRC = PrfQueryProfileData (hiniAlias, NULL, NULL, pszz, &ulSize);
            if (bRC && ulSize)
                {
                while (pszUser[0] != '\0' && bReturn)
                    {
                    bReturn &= HandleTodoList (pszUser);
                    pszUser += strlen (pszUser) + 1;
                    }
                }
            FreeMem ((PPVOID)&pszz);
            }
        else
            bReturn = FALSE;

        }

    return bReturn;
    }

/********************************************************************
 *  Abarbeiten der ToDo-Liste
 *  Eingang: hini:      INI-File handle
 *  Return:  TRUE: Liste abgearbeitet; FALSE: Fehler aufgetreten
 ********************************************************************/
BOOL HandleTodoList (PSZ pszUser)
    {
    HINI  hiniLAN;
    ULONG ulSize, ulc;
    PCHAR pcToDoString, pcToDoBuffer;
    PBYTE pBuffer;
    BOOL  bRC = TRUE;

    if (!hiniAlias)
        return FALSE;

    PrfQueryProfileSize (hiniAlias, pszUser, MAGPRO_TOCOPY_KEY, &ulSize);
    if (ulSize == 0)
        {
        DebugE (D_SAM, "HandleTodoList", "ToDo-list is empty");
        return TRUE;
        }

    /* globale INI-Datei îffnen */
    hiniLAN = NULLHANDLE;
    if (IsLANProfile ())
        {
        hiniLAN = OpenLANProfile ();
        if (hiniLAN == NULLHANDLE)
            {
            DebugE (D_SAM, "HandleTodoList", "ini file on LAN path cannot be opened");
            return FALSE;
            }
        }

    /* Wenn hiniLAN == NULLHANDLE, existiert kein LAN-Profile */
    if (hiniLAN != NULLHANDLE)
        {
        /* ToDo-Liste bestimmen */
        pcToDoBuffer = pcToDoString = AllocMem (ulSize);
        PrfQueryProfileData (hiniAlias, pszUser, MAGPRO_TOCOPY_KEY, pcToDoString, &ulSize);

        /* Liste abarbeiten */
        while (ulSize > 0)
            {
            /* Jeder Teilstring in pcToDoString ist ein Keyname */
            DebugE (D_SAM, "HandleTodoList, key", pcToDoString);
            PrfQueryProfileSize (hiniAlias, pszUser, pcToDoString, &ulc);
            pBuffer = AllocMem (ulc);
            PrfQueryProfileData (hiniAlias, pszUser, pcToDoString, pBuffer, &ulc);
            bRC &= PrfWriteProfileData (hiniLAN, pszUser, pcToDoString, pBuffer, ulc);
            DebugE (D_SAM, "FreeMem", "Handle ToDo List1");
            FreeMem ((PPVOID)&pBuffer);
            DebugE (D_SAM, "FreeMem", "Handle ToDo List2");
            ulc = strlen (pcToDoString) + 1;
            ulSize       -= ulc;
            pcToDoString += ulc;
            }

        DebugE (D_SAM, "FreeMem", "FreeBuffer1");
        FreeMem ((PPVOID)&pcToDoBuffer);
        DebugE (D_SAM, "FreeMem", "FreeBuffer2");
        PrfCloseProfile (hiniLAN);
        }

    /* ToDo-Liste leeren */
    if (bRC == TRUE)
        PrfWriteProfileData (hiniAlias, pszUser, MAGPRO_TOCOPY_KEY, NULL, 0);

    DebugE (D_SAM, "HandleTodoList, return value is", bRC ? "TRUE" : "FALSE");
    return bRC;
    }

/*******************************************************************\
 *  Funktionen zur Sicherung/Restaurierung von INI-EintrÑgen, die  *
 *  beim Umkopieren des LAN-Profiles in das lokale Profil nicht    *
 *  zerstîrt werden dÅrfen                                         *
\*******************************************************************/

PINIENTRY GetLastIniEntry (VOID)
    {
    PINIENTRY pEntry = NULL;

    if (pFirstIniEntry)
        for (pEntry = pFirstIniEntry; pEntry->pNext;)
            pEntry = pEntry->pNext;

    return pEntry;
    }

PINIENTRY CreateNewIniEntry (VOID)
    {
    PINIENTRY pEntry;
    PINIENTRY pNewEntry = AllocMem (sizeof (INIENTRY));

    memset (pNewEntry, 0, sizeof (INIENTRY));

    if (pFirstIniEntry)
        {
        pEntry = GetLastIniEntry ();
        pEntry->pNext = pNewEntry;
        }
    else
        pFirstIniEntry = pNewEntry;

    return pNewEntry;
    }

PINIENTRY RemoveIniEntry (PSZ pszApp, PSZ pszKey)
    {
    PINIENTRY pPrevious = NULL;
    PINIENTRY pEntry;

    for (pEntry = pFirstIniEntry; pEntry != NULL; pEntry = pEntry->pNext)
        {
        if (strcmp (pEntry->pszAppName, pszApp) == 0 &&
            strcmp (pEntry->pszKeyName, pszKey) == 0)
            {
            if (pPrevious)
                pPrevious->pNext = pEntry->pNext;
            else
                pFirstIniEntry = pEntry->pNext;
            return pEntry;
            }
        pPrevious = pEntry;
        }

    return NULL;
    }

PINIENTRY RemoveFirstIniEntry (VOID)
    {
    PINIENTRY pEntry = NULL;

    if (pFirstIniEntry)
        {
        pEntry = pFirstIniEntry;
        pFirstIniEntry = pEntry->pNext;
        }

    return pEntry;
    }

VOID FreeIniEntry (PINIENTRY pEntry)
    {
    FreeMem ((PPVOID)&pEntry->pszAppName);
    FreeMem ((PPVOID)&pEntry->pszKeyName);
    FreeMem ((PPVOID)&pEntry->pbValue);
    FreeMem ((PPVOID)&pEntry);

    return;
    }

/********************************************************************
 *  FÅllen einer INIENTRY-Struktur
 *  Eingang: pszApp:     Application name
 *           pszKey:     Key name
 *           iniAction:  Aktion bei Restore
 *  Return:  TRUE:  Eintrag gefunden und in Liste eingehÑngt
 *           FALSE: Eintrag in INI-Datei nicht gefunden
 ********************************************************************/
BOOL AddIniEntry (PSZ pszApp, PSZ pszKey, INIACTION iniAction)
    {
    BOOL      bRC;
    ULONG     ulc;
    PINIENTRY pIniEntry;

    if (PrfQueryProfileSize (hiniAlias, pszApp, pszKey, &ulc))
        {
        pIniEntry = CreateNewIniEntry ();
        pIniEntry->pszAppName  = AllocMem (strlen (pszApp) + 1);
        pIniEntry->pszKeyName  = AllocMem (strlen (pszKey) + 1);
        pIniEntry->enAction    = iniAction;
        pIniEntry->ulTimestamp = 0;
        pIniEntry->pNext       = NULL;
        strcpy (pIniEntry->pszAppName, pszApp);
        strcpy (pIniEntry->pszKeyName, pszKey);

        if (ulc)
            {
            pIniEntry->pbValue  = AllocMem (ulc);
            pIniEntry->ulcValue = ulc;
            bRC = PrfQueryProfileData (hiniAlias, pszApp, pszKey,
                                       pIniEntry->pbValue, &pIniEntry->ulcValue);
            if (!bRC)
                {
                FreeMem ((PPVOID)&pIniEntry->pbValue);
                pIniEntry->pbValue  = NULL;
                pIniEntry->ulcValue = 0;
                }
            else
                {
                if (iniAction == ActionTimeStamp)
                    pIniEntry->ulTimestamp = GetTimestamp (pszApp);
                }
            }
        else
            {
            pIniEntry->pbValue  = NULL;
            pIniEntry->ulcValue = 0;
            }
        bRC = TRUE;
        }
    else
        bRC = FALSE;

    return bRC;
    }

VOID SaveUserEntries (PSZ pszUserName)
    {
    ULONG ulc;
    PSZ   pszService, pszz;

    if (PrfQueryProfileSize (hiniAlias, pszUserName, MAGPRO_TOCOPY_KEY, &ulc))
        {
        pszService = pszz = AllocMem (ulc);
        if (pszz)
            {
            PrfQueryProfileData (hiniAlias, pszUserName, MAGPRO_TOCOPY_KEY, pszz, &ulc);
            if (ulc)
                {
                while (pszService[0] != '\0')
                    {
                    AddIniEntry (pszUserName, pszService, ActionTimeStamp);
                    pszService += strlen (pszService) + 1;
                    }
                }
            FreeMem ((PPVOID)&pszz);
            }
        }

    return;
    }

VOID SaveLocalINIEntries (VOID)
    {
    BOOL  bRC;
    PSZ   pszAppNames, pszAppName;
    ULONG ulc;

    if (!hiniAlias)
        return;

    /* Sichern der EintrÑge, die lokal modifiziert werden kînnen */
    AddIniEntry (MAGPRO_SYSTEM_APP, MAGPRO_KEY_DT_PATH,     ActionWarning);
    AddIniEntry (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGBMP, ActionNothing);
    AddIniEntry (MAGPRO_SYSTEM_APP, MAGPRO_KEY_LASTUSER,    ActionNothing);
    AddIniEntry (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGCLR, ActionNothing);

    /* Sichern von Benutzereinstellungen (Pa·wortÑnderungen, Timestamps) */
    if (PrfQueryProfileSize (hiniAlias, NULL, NULL, &ulc))
        {
        pszAppName = pszAppNames = AllocMem (ulc);
        if (pszAppNames)
            {
            PrfQueryProfileData (hiniAlias, NULL, NULL, pszAppNames, &ulc);
            if (ulc)
                {
                while (pszAppName[0] != '\0')
                    {
                    if (pszAppName[0] != '_')
                        {
                        bRC = AddIniEntry (pszAppName, MAGPRO_TOCOPY_KEY, ActionTimeStamp);
                        if (bRC)
                            SaveUserEntries (pszAppName);
                        }
                    pszAppName += strlen (pszAppName) + 1;
                    }
                }
            FreeMem ((PPVOID)&pszAppNames);
            }
        }

    return;
    }

BOOL RestoreLocalINIEntries (VOID)
    {
    BOOL      bReturn = FALSE;
    BOOL      bRC;
    ULONG     ulc;
    PBYTE     pBuffer;
    PINIENTRY pEntry;

    if (!hiniAlias)
        return FALSE;

    for (;;)
        {
        pEntry = RemoveFirstIniEntry ();
        if (pEntry == NULL)
            break;

        switch (pEntry->enAction)
            {
            // Aktion: In die aktuelle lokale Profildatei den Originalwert zurÅckschreiben
            case ActionNothing:
                PrfWriteProfileData (hiniAlias,
                                     pEntry->pszAppName,
                                     pEntry->pszKeyName,
                                     pEntry->pbValue,
                                     pEntry->ulcValue);
                break;

            // Aktion: RÅckgabewert auf "TRUE", falls sich Wert geÑndert hat
            // Die aufrufende Routine sollte in diesem Fall eine Warnung ausgeben
            case ActionWarning:
                bRC = PrfQueryProfileSize (hiniAlias, pEntry->pszAppName, pEntry->pszKeyName, &ulc);
                if (!bRC)
                    break;

                if (ulc)
                    {
                    pBuffer = AllocMem (ulc);
                    bRC = PrfQueryProfileData (hiniAlias,
                                               pEntry->pszAppName,
                                               pEntry->pszKeyName,
                                               pBuffer,
                                               &ulc);
                    if (!bRC)
                        {
                        FreeMem ((PPVOID)&pBuffer);
                        break;
                        }
                    }
                else
                    pBuffer = NULL;

                if (pEntry->pbValue != NULL &&
                    pEntry->ulcValue > 0    &&
                    memcmp (pBuffer, pEntry->pbValue, pEntry->ulcValue) != 0)
                    bReturn = TRUE;

                FreeMem ((PPVOID)&pBuffer);
                break;

            case ActionTimeStamp:
                if (GetTimestamp (pEntry->pszAppName) < pEntry->ulTimestamp)
                    PrfWriteProfileData (hiniAlias,
                                         pEntry->pszAppName,
                                         pEntry->pszKeyName,
                                         pEntry->pbValue,
                                         pEntry->ulcValue);
                break;
            }

        FreeIniEntry (pEntry);
        }

    return bReturn;
    }

