#define INCL_DOSPROCESS
#define INCL_WINSHELLDATA
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "samglob.h"
#include "magpro2.h"

#include "Profile.hpp"

/*******************************************************************\
 *  Konstruktoren/Destruktoren                                     *
\*******************************************************************/

Profile::Profile (void)
    {
    HAB   hab;
    ULONG ulLen;
    PSZ   pszEndPath;
    CHAR  szIniPath[CCHMAXPATH];

    hab = WinQueryAnchorBlock (HWND_DESKTOP);

    if (queryProfile (szIniPath, CCHMAXPATH))
        hiniAlias = PrfOpenProfile (hab, szIniPath);
    else
        hiniAlias = NULLHANDLE;
    }

Profile::~Profile (void)
    {
    if (hiniAlias != NULLHANDLE)
        PrfCloseProfile (hiniAlias);
    }

/*******************************************************************\
 *  Methoden zum Lesen/Schreiben des Profiles                      *
\*******************************************************************/

PSZ Profile::queryProfile (PSZ szPath, ULONG ulLen, BOOL isLocalIni)
    {
    ULONG ulLenPath, ulLenFile;

    /* 1. INI-Pfad bestimmen */
    ulLenPath = PrfQueryProfileString (HINI_SYSTEMPROFILE,
                                       OS2SYS_APP_NAME,
                                       isLocalIni ? OS2SYS_KEY_CLNT_LOCAL : OS2SYS_KEY_CLNT_LAN,
                                       NULL,
                                       szPath,
                                       ulLen);
    if (szPath[ulLenPath-1] != '\0')
        szPath[ulLenPath] = '\0';
    else
        ulLenPath--;

    /* 2. INI-Datei bestimmen */
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

HINI Profile::queryProfileHandle (void)
    {
    return hiniAlias;
    }

BOOL Profile::queryProfileSize (PSZ pszApp, PSZ pszKey, PULONG pulc)
    {
    if (!hiniAlias)
        {
        *pulc = 0;
        return FALSE;
        }

    return PrfQueryProfileSize (hiniAlias, pszApp, pszKey, pulc);
    }

ULONG Profile::queryProfileString (PSZ pszApp, PSZ pszKey, PSZ pszDest, ULONG ulc)
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
        if (ulSize >= ulc)          // keine abgeschnittenen Strings zulassen
            ulSize = 0;
        pszDest[ulSize] = '\0';     // String terminieren
        }

    return ulSize;
    }

BOOL Profile::queryProfileData (PSZ pszApp, PSZ pszKey, PVOID pDest, PULONG pulc)
    {
    BOOL bRC = FALSE;

    if (hiniAlias)
        bRC = PrfQueryProfileData (hiniAlias, pszApp, pszKey, pDest, pulc);
    else
        *pulc = 0;

    return bRC;
    }

BOOL Profile::queryProfileCryptedData (PSZ pszApp, PSZ pszKey, PCHAR pDest, ULONG ulc)
    {
    ULONG ul  = ulc;
    BOOL  bRC = queryProfileData (pszApp, pszKey, pDest, &ul);

    /* kein Fehler beim Lesen aufgetreten => entschlÅsseln */
    if (bRC)
        return CRYPT_STRING (pDest, ulc, CA_Decrypt);

    return bRC;

    }

LONG Profile::queryProfileInt (PSZ pszApp, PSZ pszKey, LONG lDefault)
    {
    if (hiniAlias)
        return PrfQueryProfileInt (hiniAlias, pszApp, pszKey, lDefault);

    return lDefault;
    }

BOOL Profile::writeProfileString (PSZ pszApp, PSZ pszKey, PSZ pszSource)
    {
    if (hiniAlias)
        return PrfWriteProfileString (hiniAlias, pszApp, pszKey, pszSource);

    return FALSE;
    }

BOOL Profile::writeProfileCryptedData (PSZ pszApp, PSZ pszKey, PSZ pSource, ULONG ulc)
    {
    CRYPT_STRING (pSource, ulc, CA_Crypt);

    if (hiniAlias)
        return PrfWriteProfileData (hiniAlias, pszApp, pszKey, pSource, ulc);

    return FALSE;
    }

/*******************************************************************\
 *  Methoden zum Lesen/Schreiben des "Service-Profiles". Jeder     *
 *  Service-Daten-Eintrag ist fÅr ein Logon/Logoff-Skript zustÑndig*
\*******************************************************************/

/********************************************************************
 *  Liest einen kompletten Service-Daten-String (bestehend aus
 *  MAXSTRING Teilstrings)
 *  Eingang: pszUser:         WPSac-Username
 *           pszService:      Service-Name ("L_*")
 *           Argv[MAXSTRING]: Leerstrings
 *  Ausgang: Argv[MAXSTRING]: Array der Einzelstrings
 ********************************************************************/
VOID Profile::readServiceData (PSZ pszUser, PSZ pszService, RXSTRING Argv[])
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
    bRC = queryProfileSize (pszUser, pszService, &ulSize);
    if (!bRC || !ulSize || (ulSize & 7))
        return;

    /* Daten lesen */
    pcLogonData = (PCHAR)malloc (ulSize);

    if (queryProfileCryptedData (pszUser, pszService, pcLogonData, ulSize))
        {
        /* Strings aus Profile (Reihenfolge s. enum _string) einlesen */
        for (ulc = ulIndex = 0; ulc < MAXSTRING && ulIndex < ulSize; ulc++)
            {
            if (Argv[ulc].strptr)
                DosFreeMem (Argv[ulc].strptr);
            Argv[ulc].strlength = strlen (&pcLogonData[ulIndex]);
            DosAllocMem ((PPVOID)&(Argv[ulc].strptr), Argv[ulc].strlength + 1, PAG_COMMIT | PAG_READ | PAG_WRITE);
            memcpy (Argv[ulc].strptr, &pcLogonData[ulIndex], Argv[ulc].strlength + 1);
            ulIndex += Argv[ulc].strlength + 1;
            }
        }

    free ((PVOID)pcLogonData);
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
BOOL Profile::readServiceDataString (PSZ pszUser, PSZ pszService, LONG lString, PRXSTRING prxString)
    {
    ULONG    ulc;
    RXSTRING arxString[MAXSTRING];

    if (hiniAlias == 0)
        return FALSE;

    if (lString < MAXSTRING)
        {
        readServiceData (pszUser, pszService, arxString);
        if (prxString->strptr)
            DosFreeMem (prxString->strptr);
        prxString->strlength      = arxString[lString].strlength;
        prxString->strptr         = arxString[lString].strptr;
        arxString[lString].strptr = NULL;
        }

    for (ulc = 0; ulc < MAXSTRING; ulc++)
        {
        if (arxString[ulc].strptr != NULL)
            DosFreeMem (arxString[ulc].strptr);
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
VOID Profile::writeServiceData (PSZ pszUser, PSZ pszService, RXSTRING Argv[])
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
    pcLogonData = (PCHAR)malloc (ulSize);

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

//  nur fÅr lokale Profiledatei
//    /* Daten schreiben */
//    if (writeProfileCryptedData (pszUser, pszService, pcLogonData, ulSize))
//        {
//        /* "Todo-Daten" fÅr das globale INI-File schreiben */
//        addToTodoList (pszUser, pszService);
//        }

    free ((PVOID)pcLogonData);
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
BOOL Profile::writeServiceDataString (PSZ pszUser, PSZ pszService, LONG lString, PRXSTRING prxString)
    {
    ULONG    ulc;
    RXSTRING arxString[MAXSTRING];

    if (hiniAlias == 0)
        return FALSE;

    if (lString < MAXSTRING)
        {
        readServiceData (pszUser, pszService, arxString);
        DosFreeMem (arxString[lString].strptr);
        arxString[lString] = *prxString;
        writeServiceData (pszUser, pszService, arxString);
        arxString[lString].strptr = NULL;
        }

    for (ulc = 0; ulc < MAXSTRING; ulc++)
        {
        if (arxString[ulc].strptr != NULL)
            DosFreeMem (arxString[ulc].strptr);
        }

    return TRUE;
    }

/********************************************************************
 *  Setzt einen Timestamp fÅr ein WPSac-Pa·wort zur Altersbestimmung
 *  Eingang: pszUser:    WPSac-Username
 ********************************************************************/
VOID Profile::setPwdAge (PSZ pszUser)
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
ULONG Profile::getPwdAge (PSZ pszUser)
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

    return ulAge;
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
VOID Profile::addToTodoList (PSZ pszUser, PSZ pszString)
    {
    PCHAR pcToDoString;
    ULONG ulSize, ulIndex, ulc;

    if (!hiniAlias)
        return;

    PrfQueryProfileSize (hiniAlias, pszUser, MAGPRO_TOCOPY_KEY, &ulSize);
    ulSize += strlen (pszString) + 1;
    pcToDoString = (PCHAR)malloc (ulSize);
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
    setTimestamp (pszUser);

    free (pcToDoString);

    return;
    }

/*******************************************************************\
 *  private Methoden                                               *
\*******************************************************************/

VOID Profile::setTimestamp (PSZ pszApp)
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

HINI Profile::openLANProfile (VOID)
    {
    HAB  hab;
    HINI hini = NULLHANDLE;
    CHAR szLANPath[CCHMAXPATH];

    PCHAR pszLANPath = queryProfile (szLANPath, CCHMAXPATH, FALSE);

    hab = WinQueryAnchorBlock (HWND_DESKTOP);

    if (isLANProfile ())
        hini = PrfOpenProfile (hab, pszLANPath);

    return hini;
    }

BOOL Profile::isLANProfile (VOID)
    {
    CHAR szLANPath[CCHMAXPATH];
    CHAR szLocPath[CCHMAXPATH];

    PCHAR pszLANPath = queryProfile (szLANPath, CCHMAXPATH, FALSE);
    PCHAR pszLocPath = queryProfile (szLocPath, CCHMAXPATH, TRUE);

    if (pszLANPath == NULL)
        return FALSE;

    if (pszLocPath == NULL)
        return FALSE;

    return strcmp (pszLocPath, pszLANPath) != 0 ? TRUE : FALSE;
    }

