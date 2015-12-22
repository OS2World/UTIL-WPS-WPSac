/*
 *
 *
 *   Module Name: MakeIni
 *
 *   OS/2 Workplace Shell Access Manager
 *
 *   Initialize User desktop
 *
 */
#define CLASSNAME   "createIni"         // FÅr Debug-Funktionen

#define INCL_DOS
#define INCL_WINWORKPLACE
#define INCL_WINWINDOWMGR

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cutil.h>

#include "samglob.h"
#include "debug.h"
#include "MakeIni.h"
#include "SamMem.h"

#define WPC_SAM         "WPSamF"
#define WPC_IBM         "WPDesktop"

#define MAKEINI  "?:\\OS2\\MAKEINI.EXE"

#define FINDSTR_DESKTOP "*\"PM_InstallObject\"*\"*;WPDesktop;*\"*\"*OBJECTID=<WP_DESKTOP>\""
#define FINDSTR_COMMAND "\"PM_InstallObject\""
#define FINDSTR_CLASS   "WPDesktop"

#define CHANGESTR       "*\"PM_InstallObject\"*\"%%;%;%%\"*"

/* Definitionen fÅr strMdfyTest */
#define MDFYTEST_END    0
#define MDFYTEST_AST    1
#define MDFYTEST_PCNT   2
#define MDFYTEST_CHAR   3
#define MDFYTEST_ERR    4

/* Entfernt '*'-Zeichen am Stringanfang */
/* TRUE, wenn Zeichen entfernt wurden */
BOOL strStrip (PSZ *psz)
    {
    BOOL bRC = FALSE;
    PSZ  pszTemp = *psz;

    while (*pszTemp != '\0')
        {
        if (*pszTemp != '*')
            break;
        pszTemp++;
        bRC = TRUE;
        }

    *psz = pszTemp;
    return bRC;
    }

/* Untersucht, ob ein String mit '*' oder '%' beginnt und entfernt diese */
/* PlausibilitÑtsprÅfung: Die Zeichen dÅrfen nicht in direkter Folge stehen */
ULONG strMdfyTest (PSZ *psz)
    {
    ULONG ulRC = MDFYTEST_END;
    CHAR  c1, c2;

    c2 = **psz;

    while ((c1 = **psz) != '\0')
        {
        switch (c1)
            {
            case '*':
                (*psz)++;
                if (c1 == c2)
                    ulRC = MDFYTEST_AST;
                else
                    return MDFYTEST_ERR;
                break;
            case '%':
                (*psz)++;
                if (c1 == c2)
                    return MDFYTEST_PCNT;
                else
                    return MDFYTEST_ERR;
            default:
                if (c1 == c2)
                    return MDFYTEST_CHAR;
                else
                    return ulRC;
            }
        c2 = c1;
        }

    return ulRC;
    }

/* sucht die ersten len Zeichen von pszString2 in pszString2 */
PSZ strnstr (const PSZ pszString1, const PSZ pszString2, ULONG len)
    {
    PSZ psz = pszString1;

    while (*psz != '\0')
        {
        if (strncmp (psz, pszString2, len) == 0)
            return psz;
        psz++;
        }

    return NULL;
    }

/* Bestimmt die LÑnge des Strings bis zum nÑchsten '*' oder '%' */
ULONG getTokenLen (PSZ pszString)
    {
    PSZ pszTempA, pszTempP;

    /* LÑnge des Teilstrings bis zum nÑchsten '*' oder '%' bestimmen */
    pszTempA = strchr (pszString, '*');
    pszTempP = strchr (pszString, '%');
    if (pszTempP == NULL)
        if (pszTempA == NULL)
            return strlen (pszString);
        else
            return pszTempA - pszString;
    else if (pszTempA == NULL)
        return pszTempP - pszString;
    else if (pszTempA > pszTempP)
        return pszTempP - pszString;
    else
        return pszTempA - pszString;
    }

BOOL strExprCompare (const PSZ pszString, const PSZ pszExpression)
    {
    PSZ   pszSrc  = pszString;
    PSZ   pszExpr = pszExpression;
    PSZ   pszTemp;
    BOOL  bWasAst;
    ULONG ulLen = strlen (pszExpression);

    while (ulLen)
        {
        /* NÑchsten Teilstring nach '*' suchen */
        bWasAst = strStrip (&pszExpr);
        if (*pszSrc == '\0' || *pszSrc == '\n')
            return (*pszExpr == '\0' ? TRUE : FALSE);
        if (*pszExpr == '\0')
            return bWasAst;

        /* LÑnge des Teilstrings bis zum nÑchsten '*' bestimmen */
        ulLen = getTokenLen (pszExpr);

        /* pszExpr in pszSrc suchen */
        pszTemp = strnstr (pszSrc, pszExpr, ulLen);
        if (pszTemp == NULL)
            return FALSE;
        if (!bWasAst && pszSrc != pszTemp)
            return FALSE;
        pszSrc = pszTemp;

        /* Vergleichsstring neu positionieren */
        pszSrc  += ulLen;
        pszExpr += ulLen;
        }

    return TRUE;
    }

/* Modifiziert einen String pszString anhand des Testmusters pszExpression; an Stelle der '%'-Zeichen */
/* werden Teilstrings aus psz[] eingefÅgt. Das Resultat erscheint in pszDestination                   */
BOOL strExprChange (PSZ pszDestination, const PSZ pszString, const PSZ pszExpression, const PSZ psz[])
    {
    PSZ   pszSrc  = pszString;
    PSZ   pszExpr = pszExpression;
    PSZ   pszTemp;
    ULONG flType;
    ULONG ulArgNum = 0;
    ULONG ulLen = strlen (pszExpr);

    pszDestination[0] = '\0';

    while ((*pszExpr != '\0') && (pszSrc != NULL))
        {
        /* Stringanfang bewerten */
        flType = strMdfyTest (&pszExpr);
        switch (flType)
            {
            case MDFYTEST_ERR:
                strcpy (pszDestination, pszString);
                return FALSE;

            case MDFYTEST_END:
                return TRUE;

            case MDFYTEST_PCNT:
                strcat (pszDestination, psz[ulArgNum++]);
                ulLen = getTokenLen (pszExpr);
                pszSrc = strnstr (pszSrc, pszExpr, ulLen);
                break;

            case MDFYTEST_AST:
                ulLen = getTokenLen (pszExpr);
                if (ulLen)
                    {
                    pszTemp = strnstr (pszSrc, pszExpr, ulLen);
                    strncat (pszDestination, pszSrc, pszTemp - pszSrc);
                    pszSrc = pszTemp;
                    }
                else
                    {
                    /* pszExpr ist zu Ende */
                    strcat (pszDestination, pszSrc);
                    pszSrc += strlen (pszSrc);
                    }
                break;

            case MDFYTEST_CHAR:
                ulLen = getTokenLen (pszExpr);
                strncat (pszDestination, pszSrc, ulLen);
                pszSrc  += ulLen;
                pszExpr += ulLen;
                break;
            }
        }

    if (!pszSrc)
        {
        strcpy (pszDestination, pszString);
        return FALSE;
        }

    return TRUE;
    }

VOID createIni (PUSER pUser)
    {
    ULONG          ulBootDrive, ulFlags;
    LONG           lLen;
    PCHAR          szPos;
    STRUC_EAT_DATA arValue[2];
    FILE           *source, *dest;
    BOOL           bDeskFound = FALSE;
    BOOL           bWPSamF;
    PSZ            arsz[5];
    static CHAR szResName[CCHMAXPATH], szPrfName[CCHMAXPATH];
    static CHAR szLine[CCHMAXRCLINE], szLine2[CCHMAXRCLINE];

    /* Boot-Laufwerk bestimmen */
    DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof (ULONG));
    pUser->ulErrorMsg = 0;

    /* User Resource und User Profile erstellen, falls nicht vorhanden */
    lLen = strlen (pUser->pszUserPath) + strlen (pUser->pszUserProfile) + 1;
    if (lLen+1 >= CCHMAXPATH)
        {
        pUser->ulErrorMsg = ERR_PATHTOOLONG;
        DosExit (EXIT_THREAD, 0);
        }
    strcpy (szPrfName, pUser->pszUserPath);
    strcat (szPrfName, "\\");
    strcat (szPrfName, pUser->pszUserProfile);

    if (!FileExist (szPrfName))
        {
        /* User resource erzeugen */
        strcpy (szResName, pUser->pszUserPath);
        strcat (szResName, "\\INI.RC");

        DebugE (D_SAM, "User rc-file, source", pUser->pszUserResource);
        DebugE (D_SAM, "User rc-file, target", szResName);

        /* Default User Resource îffnen */
        if ((source = fopen (pUser->pszUserResource, "r")) == NULL)
            {
            pUser->ulErrorMsg = ERR_NOUSERRESOURCE;
            DosExit (EXIT_THREAD, 0);
            }

        /* User Resource îffnen */
        if ((dest = fopen (szResName, "w")) == NULL)
            {
            fclose (source);
            pUser->ulErrorMsg = ERR_NOUSERINICREATED;
            DosExit (EXIT_THREAD, 0);
            }

        /* Objekt-Typ lesen */  // ??? nicht benîtigt
        arValue[0].uscValue = sizeof (szLine2);
        arValue[0].pValue   = szLine2;
        arValue[1].pValue   = NULL;
        EAReadMV (pUser->pszUserPath, EA_TYPENAME, EAT_MVST, arValue);

        arValue[0].uscValue = sizeof (ULONG);
        arValue[0].pValue   = (PBYTE)&ulFlags;
        EARead   (pUser->pszUserPath, EA_FLAGS, arValue);

        DebugE (D_SAM, "Object Type", szLine2);
        DebugULx (D_SAM, "Read EA", "_ulFlag", ulFlags);
// ???  bWPSamF = strcmp (szLine2, EA_TYPEWPSAM) == 0 && !(ulFlags & PRIVATESYS);
        bWPSamF = !(ulFlags & PRIVATESYS);
        bWPSamF = FALSE;        // provisorisch fÅr WPSac ???
        DebugE (D_SAM, "bWPSamF", bWPSamF ? "TRUE" : "FALSE");

        /* PM_InstallObject fÅr <WPDesktop> Ñndern                                                       */
        /* Die Originalzeile hat den folgenden Aufbau:                                                   */
        /*  "PM_InstallObject" "ArbeitsoberflÑche;WPDesktop;path;option" "[setup;]OBJECTID=<WP_DESKTOP>" */
        /* Ist dieser String gefunden, mu· folgendes geschehen:                                          */
        /*  "ArbeitsoberflÑche" => "ArbeitsoberflÑche_User"                                              */
        /*  "WPDesktop"         => WPC_SAM oder WPC_IBM                                                  */
        /*  path                => pUser->pszUserPath                                                      */
        while (fgets (szLine, CCHMAXRCLINE, source) != NULL)
            {
            if (strExprCompare (szLine, FINDSTR_DESKTOP) == FALSE)
                fputs (szLine, dest);
            else
                {
                arsz[0] = pUser->pszDesktopName;        /* Teilstring: Neuer Desktop-Name */
                arsz[1] = pUser->pszUserName;
                arsz[2] = bWPSamF ? WPC_SAM : WPC_IBM;  /* Teilstring: Klassenname */
/* Achtung: Hier gibts ein Problem, wenn in UPROFILE keine Laufwerksbezeichnungen eingetragen sind.
 * Daher sollte dies an einer frÅheren Stelle abgefangen werden. TemporÑr geschieht dies hier:
 */
                arsz[3] = (pUser->pszUserPath[1] == ':' ? "" : "C:");
                arsz[4] = pUser->pszUserPath;             /* Teilstring: Desktop-Pfad */

                strExprChange (szLine2, szLine, CHANGESTR, arsz);

                fputs (szLine2, dest);
                DebugE (D_SAM, "User Resource File, <WP_DESKTOP>-Registration, OrgLine\n\t", szLine);
                DebugE (D_SAM, "User Resource File, <WP_DESKTOP>-Registration, NewLine\n\t", szLine2);
                bDeskFound = TRUE;
                }
            }

        fclose (source);
        fclose (dest);

        if (!bDeskFound)
            {
            pUser->ulErrorMsg = ERR_NODESKINRC;
            DosExit (EXIT_THREAD, 0);
            }

        /* User Profile compilieren */
        lLen = CCHMAXRCLINE - strlen (MAKEINI)
            - 1 - 3 - 1 - strlen (szPrfName) - strlen (szResName);
        if (lLen < 3)
            {
            pUser->ulErrorMsg = ERR_PATHTOOLONG;
            DosExit (EXIT_THREAD, 0);
            }

        memset (szLine, '\0', sizeof (szLine));
        strcpy (szLine, MAKEINI);
        szLine[0] = (CHAR) (ulBootDrive - 1) + 'A';
        szPos = szLine + strlen (szLine) + 1;
        strcpy (szPos, "\"");                           /* LÑnge: 1 Zeichen */
        strcat (szPos, szPrfName);
        strcat (szPos, "\" \"");                        /* LÑnge: 3 Zeichen */
        strcat (szPos, szResName);
        strcat (szPos, "\"");                           /* LÑnge: 1 Zeichen */

        DebugE (-1, "User resource file, ExecPgm", szLine);
        DebugE (-1, "User resource file, Argument", szPos);

        DosExecPgm (szLine2,                            /* Object name buffer */
                    sizeof(szLine2),                    /* Length of object name buffer */
                    EXEC_SYNC,                          /* Execution flags */
                    szLine,                             /* Argument string */
                    NULL,                               /* Environment */
                    &pUser->rcSuccess,                  /* Termination codes */
                    szLine);                            /* Program file name */

        DebugULd (-1, "User resource file", "codeTerminate",
                 pUser->rcSuccess.codeTerminate);
        DebugULd (-1, "User resource file", "codeResult",
                 pUser->rcSuccess.codeResult);

// ???  #ifndef DEBUG
//      DosForceDelete (szResName);
//      #endif

        if (pUser->rcSuccess.codeTerminate || pUser->rcSuccess.codeResult)
            {
            pUser->ulErrorMsg = ERR_NOUSERINICREATED;
            DosExit (EXIT_THREAD, 0);
            }

        WinSetObjectData (WinQueryObject (szPrfName),                   // ???
                          "NOMOVE=YES;NODELETE=YES;NORENAME=YES");
        }

    /* System Profile erstellen, falls nicht vorhanden */
    if (*(pUser->pszSystemResource) != '\0')
        {
        if (strlen (pUser->pszSystemResource) > CCHMAXPATH-1)
            {
            pUser->ulErrorMsg = ERR_PATHTOOLONG;
            DosExit (EXIT_THREAD, 0);
            }
        strcpy (szResName, pUser->pszSystemResource);

        lLen = strlen (pUser->pszUserPath) + strlen (pUser->pszSystemProfile) + 1;
        if (lLen+1 > CCHMAXPATH)
            {
            pUser->ulErrorMsg = ERR_PATHTOOLONG;
            DosExit (EXIT_THREAD, 0);
            }
        strcpy (szPrfName, pUser->pszUserPath);
        strcat (szPrfName, "\\");
        strcat (szPrfName, pUser->pszSystemProfile);

        if (!FileExist (szPrfName))
            {
            if (!FileExist (szResName))
                {
                pUser->ulErrorMsg = ERR_NOSYSTEMRESOURCE;
                DosExit (EXIT_THREAD, 0);
                }

            /* System Profile compilieren */
            lLen = CCHMAXRCLINE - strlen (MAKEINI)
                - 1 - 3 - 1 - strlen (szPrfName) - strlen (szResName);
            if (lLen < 3)
                {
                pUser->ulErrorMsg = ERR_PATHTOOLONG;
                DosExit (EXIT_THREAD, 0);
                }

            memset (szLine, '\0', sizeof (szLine));
            strcpy (szLine, MAKEINI);
            szLine[0] = (CHAR) (ulBootDrive - 1) + 'A';
            szPos = szLine + strlen (szLine) + 1;
            strcpy (szPos, "\"");                        /* LÑnge: 1 Zeichen */
            strcat (szPos, szPrfName);
            strcat (szPos, "\" \"");                     /* LÑnge: 3 Zeichen */
            strcat (szPos, szResName);
            strcat (szPos, "\"");                        /* LÑnge: 1 Zeichen */

            DebugE (-1, "System resource file, ExecPgm", szLine);
            DebugE (-1, "System resource file, Argument", szPos);

            DosExecPgm (szLine2,                /* Object name buffer */
                        sizeof(szLine2),        /* Length of object name buffer */
                        EXEC_SYNC,              /* Execution flags */
                        szLine,                 /* Argument string */
                        NULL,                   /* Environment */
                        &pUser->rcSuccess,      /* Termination codes */
                        szLine);                /* Program file name */

            DebugULd (-1, "System resource file", "codeTerminate",
                    pUser->rcSuccess.codeTerminate);
            DebugULd (-1, "System resource file", "codeResult",
                    pUser->rcSuccess.codeResult);

            if (pUser->rcSuccess.codeTerminate || pUser->rcSuccess.codeResult)
                {
                pUser->ulErrorMsg = ERR_NOSYSTEMINICREATED;
                DosExit (EXIT_THREAD, 0);
                }

            WinSetObjectData (WinQueryObject (szPrfName),
                              "NOMOVE=YES;NODELETE=YES;NORENAME=YES");
            }
        }
    pUser->ulErrorMsg = 0;
    return;
    }
