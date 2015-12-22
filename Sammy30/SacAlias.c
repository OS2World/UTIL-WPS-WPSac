/***************************************************************************
 *
 * MODULNAME: Sac_Alias
 * -------------
 *
 * VERSION: 1.0
 * --------
 *
 * BESCHREIBUNG:
 * -------------
 * Funktionen zum Erstellen, Lesen und éndern der Alias-Liste von Sac.
 * Zur öbergabe der Daten wird die ALIASDATA-Struktur verwendet.
 * Die VerschlÅsselung ist hier enthalten, jedoch transparent.
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *   IBM C/C++ Set/2 Version 1.2
 *   OS/2 Linker LINK386.EXE
 *
 * BENôTIGTE FILES:
 * ----------------
 *   SAMGLOB.H    - C Include-Datei
 *
 * BENôTIGTE BIBLIOTHEKEN:
 * -----------------------
 *   IBM C Multithreaded Runtime Library
 *   OS2386.LIB OS/2 Import-Library fÅr 32-Bit API
 *
 *  Copyright (C) 1993 Noller & Breining Software 1994
 *
 ******************************************************************************/
#define INCL_DOS
#define INCL_WINSHELLDATA
#define INCL_WINCOUNTRY
#include <os2.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "samglob.h"
#include "SamMem.h"
#include "sammy.h"
#include "samdata.h"
#include "SacAlias.h"
#include "prepare.h"
#include "profile.h"
#include "magpro2.h"
#include "debug.h"

#define CCHMAXKEY   0x1000      /* 1 Page Platz fÅr einen Key-Wert aus INI-Datei */

/* Modulinterne Funktionen */
ULONG CheckAlias (PALIASDATA);

/*******************************************************************
   Bestimmt den Benutzer mit der Nummer lUser im System. Ist kein
   Benutzer mehr vorhanden (lUser > aktuelle Anzahl), wird ein
   Leerstring Åbergeben.
   Eingang: lUser:   Nummer des gewÅnschten Benutzers (0-based)
            pszUser: Zeiger auf Ergebnispuffer (CCHMAXUSERNAME)
   Ausgang: pszUser: Ergebnisstring (uppercase)CCHMAXUSERNAME
 *******************************************************************/
VOID QueryAlias (LONG lUser, PSZ pszUser)
    {
    ULONG ulSize, ulLen;
    LONG  lc;
    PCHAR pszUsers, pszString;

    pszUser[0] = '\0';
    if (!QueryLocINISize (NULL, NULL, &ulSize))
        return;

    /* Platz fÅr alle Application-Names allokieren */
    pszUsers = AllocMem (ulSize);
    if (pszUsers == NULL)
        return;

    ulSize = QueryLocINIString (NULL, NULL, pszUsers, ulSize);

    /* Benutzer mit der Nummer lUser bestimmen (Benutzernamen haben kein vorangestelltes '_') */
    pszString = pszUsers;
    lc = -1;
    while ((ulLen = strlen (pszString)) > 0)
        {
        if (*pszString != '_')
            {
            lc++;
            if (lc == lUser)
                {
                strcpy (pszUser, pszString);
                WinUpper (hab, 0, 0, pszUser);
                return;
                }
            }
        pszString += ulLen + 1;
        }

    DebugE (D_SAM, "FreeMem", "AliasFind1");
    FreeMem ((PPVOID)&pszUsers);
    DebugE (D_SAM, "FreeMem", "AliasFind2");
    return;
    }

/*******************************************************************
   Findet den ersten zu pAliasData passenden Eintrag in der
   Alias-Datei.
   Folgende Flags kînnen am Eingang gesetzt sein:
   ALIAS_USER     ≥ALIAS_PASSWORD ≥ALIAS_ID   ≥ Aktion
   ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ≈ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ≈ƒƒƒƒƒƒƒƒƒƒƒ≈ƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒƒ
    nein          ≥ nein          ≥ nein      ≥ rc=STAT_USERNOTFOUND
    nein          ≥ nein          ≥ ja        ≥ ID prÅfen
    nein          ≥ ja            ≥ nein      ≥ rc=STAT_USERNOTFOUND
    nein          ≥ ja            ≥ ja        ≥ PWD+ID prÅfen
    ja            ≥ nein          ≥ nein      ≥ USER prÅfen
    ja            ≥ nein          ≥ ja        ≥ USER+ID prÅfen
    ja            ≥ ja            ≥ nein      ≥ USER+PWD prÅfen
    ja            ≥ ja            ≥ ja        ≥ USER+PWD+ID prÅfen

   Eingang: pAliasData: Zeiger auf ALIASDATA-Struktur
   Ausgang: pAliasData: restliche Felder der ALIASDATA-Struktur gefÅllt
   return:  STAT_PASSWORDOK     Benutzer ist ok
            STAT_NOPASSWORD     Benutzer ist ok
            STAT_NOUSERFOUND    INI-Datei nicht gefunden
            STAT_USERNOTFOUND   Benutzer nicht gefunden
            STAT_WRONGPASSWD    falsches Pa·wort
 *******************************************************************/
ULONG AliasFind (PALIASDATA pAliasData)
    {
    ULONG      ulSize, ulc, ulLen;
    ULONG      ulRet;
    PCHAR      pszUsers, pszString;

    if (!QueryLocINISize (NULL, NULL, &ulSize))
        return STAT_NOUSERFOUND;

    /* Platz fÅr alle Application-Names allokieren */
    pszUsers = AllocMem (ulSize);
    if (pszUsers == NULL)
        return STAT_NOUSERFOUND;
    memset (pszUsers, '\0', ulSize);

    ulSize = QueryLocINIString (NULL, NULL, pszUsers, ulSize);

    /* Zahl der Benutzer bestimmen (Benutzernamen haben kein vorangestelltes '_') */
    pszString = pszUsers;
    ulc = 0;
    while ((ulLen = strlen (pszString)) > 0)
        {
        if (*pszString != '_')
            ulc++;
        pszString += ulLen + 1;
        }
    if (ulc == 0)
        {
        /* Kein Benutzer gefunden => beenden */
        ulRet = STAT_NOUSERFOUND;
        }
    else
        {
        /* Mindestens 1 Benutzer in Alias-Datenbank gefunden */
        if (pAliasData->ulFlag & ALIAS_PASSWORD)
            WinUpper (hab, 0, 0, pAliasData->szPassword);

        if (pAliasData->ulFlag & ALIAS_USER)
            {
            if (pAliasData->szUser[0] == '_')
                {
                /* Vorsichtsma·nahme, falls UPROFILE diesen */
                /* ungÅltigen Benutzernamen zulÑ·t!         */
                ulRet = STAT_USERNOTFOUND;
                }
            else
                {
                /* Benutzer kann direkt gesucht werden */
                WinUpper (hab, 0, 0, pAliasData->szUser);

                /* Benutzer suchen; wenn gefunden, ist ulSize>0 */
                pszString = pszUsers;
                WinUpper (hab, 0, 0, pszString);
                while (ulSize > 0)
                    {
                    if (strcmp (pAliasData->szUser, pszString) == 0)
                        break;
                    ulc        = strlen (pszString) + 1;
                    ulSize    -= ulc;
                    pszString += ulc;
                    }

                /* Evtl. weitere Merkmale untersuchen */
                if (ulSize > 0)
                    ulRet = CheckAlias (pAliasData);
                else
                    ulRet = STAT_USERNOTFOUND;
                }
            }
        else
            {
            /* pszUser wurde nicht angegeben */
            if (!(pAliasData->ulFlag & ALIAS_ID))
                ulRet = STAT_USERNOTFOUND;
            else
                {
                ulRet = STAT_USERNOTFOUND;
                pszString = pszUsers;
                while (ulSize > 0)
                    {
                    strcpy (pAliasData->szUser, pszString);
                    ulRet = CheckAlias (pAliasData);
                    if (ulRet == STAT_WRONGPASSWD)
                        {
                        ulc        = strlen (pszString) + 1;
                        ulSize    -= ulc;
                        pszString += ulc;
                        }
                    else
                        break;
                    }
                }
            }
        }

    DebugE (D_SAM, "FreeMem", "AliasFind1");
    FreeMem ((PPVOID)&pszUsers);
    DebugE (D_SAM, "FreeMem", "AliasFind2");
    return ulRet;
    }

/*******************************************************************
   PrÅft ob ein Eintrag in der Alias-Datei mit dem Benutzer in
   der AliasData-Struktur Åbereinstimmt.
   Es wird davon ausgegangen, da· ALIAS_USER bereits geprÅft wurde,
   falls das entsprechende Flag gesetzt ist.
   Eingang: pAliasData: Zeiger auf ALIASDATA-Struktur
   Ausgang: pAliasData: restliche Felder der ALIASDATA-Struktur gefÅllt
   return:  STAT_PASSWORDOK     Benutzer ist ok
            STAT_NOPASSWORD     Benutzer ist ok
            STAT_NOUSERFOUND    INI-Datei nicht gefunden
            STAT_USERNOTFOUND   Benutzer nicht gefunden
            STAT_WRONGPASSWD    falsches Pa·wort
 *******************************************************************/
ULONG CheckAlias (PALIASDATA pAliasData)
    {
    ULONG   ulRet;
    CHAR    szPassword[CCHMAXPASSWORD];
    CHAR    szID[CCHMAXTRACKDATA];

    memset (szID,       '\0', CCHMAXTRACKDATA);
    memset (szPassword, '\0', CCHMAXPASSWORD);

    QueryLocINIString (pAliasData->szUser, MAGPRO_CARDCODE_KEY, szID, CCHMAXTRACKDATA);
    QueryLocINICryptedData (pAliasData->szUser, MAGPRO_PASSWORD_KEY, szPassword, CCHMAXPASSWORD);
    szPassword[CCHMAXPASSWORD] = '\0';

    if (pAliasData->ulFlag & ALIAS_PASSWORD)
        {
        WinUpper (hab, 0, 0, szPassword);
        if ((strlen (pAliasData->szPassword) == 0) &&
            (flMode & FLG_SYS_PWD_MANDATORY))
            ulRet = STAT_WRONGPASSWD;
        else
            {
            if (strcmp (szPassword, pAliasData->szPassword))
                ulRet = STAT_WRONGPASSWD;
            else
                ulRet = STAT_PASSWORDOK;
            }
        }
    else
        {
        strcpy (pAliasData->szPassword, szPassword);
        ulRet = STAT_NOPASSWORD;
        }

    if (ulRet != STAT_WRONGPASSWD)
        {
        if (pAliasData->ulFlag & ALIAS_ID)
            {
            WinUpper (hab, 0, 0, szID);
            if (strcmp (szID, pAliasData->szID))
                ulRet = STAT_WRONGPASSWD;
            else
                ulRet = STAT_PASSWORDOK;
            }
        else
            strcpy (pAliasData->szID, szID);
        }

    if (ulRet != STAT_WRONGPASSWD)
        {
        QueryLocINIStringFlag (pAliasData->szUser, MAGPRO_USERDESK_KEY,
            pAliasData->szDesktop, sizeof (pAliasData->szDesktop),
            &(pAliasData->ulFlag), ALIAS_DESKTOP);
        QueryLocINIStringFlag (pAliasData->szUser, MAGPRO_SAC_USER_KEY,
            pAliasData->szSacUser, sizeof (pAliasData->szSacUser),
            &(pAliasData->ulFlag), ALIAS_SAC_USER);
        QueryLocINIStringFlag (pAliasData->szUser, MAGPRO_SAC_GROUP_KEY,
            pAliasData->szSacGroup, sizeof (pAliasData->szSacGroup),
            &(pAliasData->ulFlag), ALIAS_SAC_GROUP);
        pAliasData->bIsAdmin = ReadAdminFlag (pAliasData->szUser);
        }

    return ulRet;
    }

VOID AliasSaveNewPassword (PSZ pszUser, PSZ pszPassword)
    {
    WriteLocINICryptedData (pszUser, MAGPRO_PASSWORD_KEY, pszPassword, CCHMAXPASSWORD);
    SetPwdAge (strucUserData.szUser);
    AddToTodoList (pszUser, MAGPRO_PASSWORD_KEY);
    AddToTodoList (pszUser, MAGPRO_PASSWORDAGE_KEY);

    return;
    }
