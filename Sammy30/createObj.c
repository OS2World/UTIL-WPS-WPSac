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

#define INCL_DOSPROCESS
#define INCL_DOSFILEMGR
#define INCL_DOSERRORS
#define INCL_WINWORKPLACE
#define INCL_WINSHELLDATA
#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <malloc.h>

#include <cutil.h>

#include "samglob.h"
#include "debug.h"
#include "magpro2.h"
#include "profile.h"

#define CLASSNAME           "Sammy.CreateObj"
#define CLASS_IDENTIFIER    "DeskManPRIVATECLASS="
#define CLASS_LAUNCHPAD     "WPLaunchPad"
#define CLASS_LP_CONFIG     "DMExtLaunchPad="
#define CLASS_LP_DRAWER     "DRAWEROBJECTS="
#define CLASS_EXENAME       "EXENAME="

#define LPCONFIG_SEP        ",~;"
#define EXENAME_SEP         ";"""

#define SETUP_ICONFILE      "ICONFILE"
#define SETUP_ICONNFILE     "ICONNFILE"
#define SETUP_BGBITMAP      "BACKGROUND"

typedef struct _OBJDEF *POBJDEF;
typedef struct _OBJDEF
    {
    PSZ     pszClassname;
    PSZ     pszTitle;
    PSZ     pszLocation;
    PSZ     pszSetup;
    POBJDEF pNext;
    } OBJDEF;

POBJDEF pFirstObjDef;

VOID AddObjDefEntry (PSZ pszC, PSZ pszT, PSZ pszL, PSZ pszS)
    {
    POBJDEF pObj, pObjLast;

    for (pObj = pFirstObjDef; pObj != NULL; pObj = pObj->pNext)
        {
        pObjLast = pObj;
        }

    pObjLast->pNext = malloc (sizeof (OBJDEF));
    pObj = pObjLast->pNext;
    pObj->pszClassname = strdup (pszC);
    pObj->pszTitle     = strdup (pszT);
    pObj->pszLocation  = strdup (pszL);
    pObj->pszSetup     = strdup (pszS);
    pObj->pNext        = NULL;

    if (pFirstObjDef == NULL)
        pFirstObjDef = pObj;

    return;
    }

POBJDEF SearchObjDef (PSZ pszT, PSZ pszC, PSZ pszL)
    {
    POBJDEF pObj;

    for (pObj = pFirstObjDef; pObj != NULL; pObj = pObj->pNext)
        {
        if (strcmp (pszT, pObj->pszTitle)     == 0 &&
            strcmp (pszC, pObj->pszClassname) == 0 &&
            strcmp (pszL, pObj->pszLocation)  == 0)
            break;
        }

    return pObj;
    }

VOID DeleteObjDefs (VOID)
    {
    POBJDEF pObj, pObjNext;

    pObj = pFirstObjDef;
    while (pObj != NULL)
        {
        pObjNext = pObj->pNext;

        DebugE (D_SAM, "FreeMem", "ObjDefs1");
        free (pObj);
        DebugE (D_SAM, "FreeMem", "ObjDefs2");
        pObj = pObjNext;
        }

    return;
    }


/* szFilename: Dateiname des *.DRC-Files                                */
/* szDesktopPath: Pfad auf das lokale Verzeichnis der ArbeitsoberflÑche */
/* KOPIERT das neue Resource-File in das lokale Verzeichnis             */
BOOL IsNewResourceFile (PSZ szDesktopPath, PSZ szFilename, PSZ szResourceFile)
    {
    BOOL           bRC = FALSE;
    CHAR           szUNC[CCHMAXPATH];
    ULONG          ulSize;
    ULONG          ulDateTimeLocal, ulDateTimeRemote;
    APIRET         rc;
    FSINFO         fsInfo;
    FILESTATUS3    fs3;
    STRUC_EAT_DATA strucValue;

    ulSize = PrfQueryProfileString (HINI_SYSTEMPROFILE, OS2SYS_APP_NAME, OS2SYS_KEY_CLNT_LOCAL, NULL, szUNC, CCHMAXPATH);
    DebugE (D_SAM, "IsNewResourceFile, Path", szUNC);

    if (ulSize && szUNC[0])
        {
        /* Es wurde ein UNC-Pfad eingegeben */
        szUNC[ulSize]   = '\\';
        szUNC[ulSize+1] = '\0';
        strcat (szUNC, szFilename);
        DebugE (D_SAM, "IsNewResourceFile, File", szUNC);

        if (szUNC[1] == ':')
            {
            /* Es handelt sich um eine Darstellung "Laufwerk:Pfad" */
            /* Laufwerk prÅfen */
            rc = DosQueryFSInfo ((ULONG)(toupper (szUNC[0])-'A'+1), FSIL_VOLSER, (PVOID)&fsInfo, sizeof (fsInfo));
            if (rc == 0 || rc == ERROR_NO_VOLUME_LABEL)
                {
                /* Pfad prÅfen */
                rc = DosQueryPathInfo (szUNC, FIL_STANDARD, (PVOID)&fs3, sizeof (fs3));
                if (rc == 0)
                    {
                    /* Pfadangabe ist korrekt => Datei kann kopiert werden */
                    /* EA_MODIFYDATE lesen */
                    strucValue.usEAType = EAT_BINARY;
                    strucValue.uscValue = sizeof (ulDateTimeLocal);
                    strucValue.pValue   = (PBYTE)&ulDateTimeLocal;
                    if (EARead (szDesktopPath, EA_MODIFYDATE, &strucValue))
                        {
                        ulDateTimeRemote = (*(USHORT *)(&fs3.fdateLastWrite)<<16) + *(USHORT *)(&fs3.ftimeLastWrite);
                        DebugULx (D_SAM, "IsNewResourceFile", "ulDateTimeRemote", ulDateTimeRemote);
                        DebugULx (D_SAM, "IsNewResourceFile", "ulDateTimeLocal", ulDateTimeLocal);
                        if (ulDateTimeRemote > ulDateTimeLocal || strucValue.uscValue == 0)
                            {
                            DebugE (D_SAM, "IsNewResourceFile", "neuen Timestamp schreiben");
                            /* Remote-Datei ist aktueller => Lokale Datei ersetzen */
                            DosCopy (szUNC, szResourceFile, DCPY_EXISTING);
                            strucValue.usEAType = EAT_BINARY;
                            strucValue.uscValue = sizeof (ulDateTimeLocal);
                            strucValue.pValue   = (PBYTE)&ulDateTimeRemote;
                            EAWrite (szDesktopPath, EA_MODIFYDATE, &strucValue);
                            bRC = TRUE;
                            }
                        }
                    }
                else
                    {
                    WinAlarm (HWND_DESKTOP, WA_ERROR);
                    DebugE (D_SAM, "IsNewResourceFile", "fehlerhafte UNC-Pfadangabe");
                    }
                }
            else
                {
                WinAlarm (HWND_DESKTOP, WA_ERROR);
                DebugE (D_SAM, "IsNewResourceFile", "fehlerhafte UNC-Pfadangabe");
                }
            }
        else
            {
            WinAlarm (HWND_DESKTOP, WA_NOTE);
            DebugE (D_SAM, "IsNewResourceFile", "UNC-Pfade nicht unterstÅtzt");
            }
        }

    return bRC;
    }

/* String (umschlossen von '"') suchen; Position einschlie·lich '"' zurÅckgeben.
 * RÅckgabewert NULL bei fehlerhaftem String.
 */
PSZ findString (PSZ psz)
    {
    while (*psz != '\0')
        {
        switch (*psz)
            {
            case '\t':
            case ' ':
                psz++;
                break;

            case '"':
                return psz;

            default:
                return NULL;
            }
        }

    return NULL;
    }

/* Separiert einen mit '"' umklammerten String. Die AnfÅhrungsstriche werden entfernt, der
 * Ergebnisstring in ppsz plaziert. Der RÅckgabewert zeigt auf das nÑchste verfÅgbare Zeichen
 * (nach dem ehemaligen schlie·enden AnfÅhrungsstrich).
 * RÅckgabewert NULL bei fehlerhaftem String
 */
PSZ separateString (PSZ psz, PSZ *ppsz)
    {
    if (*psz++ == '"')
        {
        *ppsz = psz;
        while (*psz != '"')
            if (*psz == '\0')
                return NULL;
            else
                psz++;
        *psz = '\0';                        // String terminieren
        }
    else
        return NULL;

    return ++psz;
    }

/* Separiert aus einem mit '"' umklammerten String drei Substrings, die durch ';' getrennt
 * sind. Der RÅckgabewert zeigt auf das nÑchste verfÅgbare Zeichen (nach dem ehemaligen
 * schlie·enden AnfÅhrungsstrich). RÅckgabewert NULL bei fehlerhaftem String.
 */
PSZ separateSubStrings (PSZ psz, PSZ *ppsz1, PSZ *ppsz2, PSZ *ppsz3)
    {
    PSZ pszString;
    PSZ pszS1     = NULL;
    PSZ pszS2     = NULL;

    if (*psz != '"')
        return NULL;
    else
        pszString = ++psz;

    /* Ende des Strings suchen und terminieren, letzte 2 Semikolon merken */
    while (*psz != '"')
        {
        switch (*psz)
            {
            case '\0':
                return NULL;

            case ';':
                pszS1 = pszS2;
                pszS2 = psz++;
                break;

            default:
                psz++;
            }
        }
    *psz = '\0';                            // String terminieren

    /* Teilstrings sichern; Terminatorzeichen einfÅgen */
    if (pszS2 == NULL || pszS1 == NULL)
        return NULL;                        // es gibt zuwenig Teilstrings

    *ppsz1   = pszString;
    *pszS1++ = '\0';
    *ppsz2   = pszS1;
    *pszS2++ = '\0';
    *ppsz3   = pszS2;

    return ++psz;
    }

/* Separiert aus einem String einen LaunchPad-Eintrag, bestehend aus fÅnf Substrings, die durch ',' getrennt
 * sind. Das letzte Zeichen ist eine Tilde '~'. Zwei Tilden zeigen den letzten Eintrag an.
 * Der RÅckgabewert zeigt auf das nÑchste verfÅgbare Zeichen (nach der Tilde).
 * RÅckgabewert NULL bei fehlerhaftem String oder letztem Eintrag.
 * Aufbau eine Substrings:
 * Title,Class,Location_ObjectID,Drawer_No,Drawer_level~
 * FÅr die RÅckgabeparameter dÅrfen NULL-Strings Åbergeben werden
 */
PSZ separateLPConfigStrings (PSZ psz, PSZ *ppszT, PSZ *ppszC, PSZ *ppszL, PUSHORT pusDrawerNo, PUSHORT pusDrawerLevel)
    {
    PCHAR pcNext;

    /* Titel-String bestimmen */
    pcNext = strpbrk (psz, LPCONFIG_SEP);
    if (pcNext == NULL || *pcNext != ',')
        return NULL;
    if (ppszT)
        {
        *pcNext = '\0';
        *ppszT  = psz;
        DebugE (D_SAM, "separateLPConfigStrings, Title", psz);
        }
    psz = pcNext + 1;

    /* Class-String bestimmen */
    pcNext = strpbrk (psz, LPCONFIG_SEP);
    if (pcNext == NULL || *pcNext != ',')
        return NULL;
    if (ppszC)
        {
        *pcNext = '\0';
        *ppszC  = psz;
        DebugE (D_SAM, "separateLPConfigStrings, Class", psz);
        }
    psz = pcNext + 1;

    /* Location_ObjectID bestimmen */
    pcNext = strpbrk (psz, LPCONFIG_SEP);
    if (pcNext == NULL || *pcNext != ',')
        return NULL;
    if (ppszL)
        {
        *pcNext = '\0';
        *ppszL  = psz;
        DebugE (D_SAM, "separateLPConfigStrings, Location", psz);
        }
    psz = pcNext + 1;

    /* Drawer-Nummer bestimmen */
    pcNext = strpbrk (psz, LPCONFIG_SEP);
    if (pcNext == NULL || *pcNext != ',')
        return NULL;
    if (pusDrawerNo)
        {
        *pusDrawerNo = atoi (psz);
        DebugE (D_SAM, "separateLPConfigStrings, DrawerNo", psz);
        }
    psz = pcNext + 1;

    /* Drawer-Level bestimmen */
    pcNext = strpbrk (psz, LPCONFIG_SEP);
    if (pcNext == NULL || *pcNext != '~')
        return NULL;
    if (pusDrawerLevel)
        {
        *pusDrawerLevel = atoi (psz);
        DebugE (D_SAM, "separateLPConfigStrings, DrawerLevel", psz);
        }
    psz = pcNext + 1;

    return psz;
    }

PCHAR RestOfString (PCHAR pszSrc)
    {
    PCHAR psz1, psz2;

    psz1 = strchr (pszSrc, ';');
    psz2 = strchr (pszSrc, ',');
    if (psz1 == NULL && psz2 == NULL)
        return pszSrc + strlen (pszSrc);
    else
        {
        if (psz1 == NULL)
            return psz2;
        if (psz2 == NULL)
            return psz1;
        if (psz1 < psz2)
            return psz1;
        else
            return psz2;
        }
    }

PCHAR ExtractFilename (PCHAR pSrc, PCHAR pszBuffer)
    {
    PCHAR pszEnd, psz;

    /* Stringende oder ";" suchen */
    pszEnd = RestOfString (pSrc) - 1;

    /* von hinten nach '\' suchen */
    psz = pszEnd;
    while (*psz != '\\')
        {
        if (psz == pSrc)
            break;
        psz--;
        }

    /* String umkopieren */
    if (*psz == '\\')
        psz++;

    memcpy (pszBuffer, psz, pszEnd-psz+1);
    pszBuffer[(ULONG)(pszEnd-psz)+1] = '\0';
    return pszBuffer;
    }

VOID CreateLaunchpadSetupString (HOBJECT hObject, PSZ pszSetupString)
    {
    USHORT  usDrawerNo;
    ULONG   ulLen;
    POBJDEF pObj;
    PSZ     pszConfigString, psz;
    PSZ     pszSetup, pszExeName;
    PSZ     pszT, pszC, pszL;

    /* Konfigurationsstring fÅr Klickstartleiste suchen */
    pszConfigString = strstr (pszSetupString, CLASS_LP_CONFIG);
    if (pszConfigString == NULL)
       return;

    /* String duplizieren, damit er modifiziert werden kann */
    pszConfigString = strdup (pszConfigString + sizeof (CLASS_LP_CONFIG) - 1);

    /* Alle KonfigurationseintrÑge bearbeiten */
    psz      = pszConfigString;
    ulLen    = strlen (psz);
    pszSetup = malloc (ulLen);

    for (;;)
        {
        psz = separateLPConfigStrings (psz, &pszT, &pszC, &pszL, &usDrawerNo, NULL);
        if (psz == NULL)
            break;

        /* Objekt in der Liste angelegter Objekte suchen */
        pObj = SearchObjDef (pszT, pszC, pszL);

        if (pObj && (strcmp (pszC, "WPProgram") == 0 || strcmp (pszC, "WPProgramFile") == 0))
            {
            pszExeName = strstr (pObj->pszSetup, CLASS_EXENAME);
            if (pszExeName)
                {
                strcpy (pszSetup, CLASS_LP_DRAWER);
                _itoa (usDrawerNo, pszSetup + strlen (pszSetup), 10);
                strcat (pszSetup, ",");
                ulLen = strpbrk (pszExeName, EXENAME_SEP) - pszExeName - (sizeof (CLASS_EXENAME) - 1);
                if (ulLen > 0)
                    {
                    DebugE (D_SAM, "CreateLaunchpadSetupString, SetupString", pszSetup);
                    strncat (pszSetup, pszExeName + sizeof (CLASS_EXENAME) - 1, ulLen);
                    WinSetObjectData (hObject, pszSetup);
                    }
                }
            }
        }

    DebugE (D_SAM, "FreeMem", "Launchpad1");
    free (pszSetup);
    DebugE (D_SAM, "FreeMem", "Launchpad2");
    free (pszConfigString);
    DebugE (D_SAM, "FreeMem", "Launchpad3");
    return;
    }

/* Eine Zeile einer (D)RC-Datei besteht aus drei Strings:
 * - 1. Kommando; aktuell wird nur "PM_InstallObject" unterstÅtzt
 * - 2. 3 Teilstrings:
 *      1. Titel
 *      2. Klassenname
 *      3. Positionierung (Object ID oder absoluter Pfad)
 * - 3. Setupstring fÅr das neue Objekt
 */
BOOL CreateObjects (PSZ pszProfile)
    {
    BOOL        bRC = FALSE;
    ULONG       ulFlags, ulc;
    CHAR        szLine[CCHMAXRCLINE];
    CHAR        szUNC[CCHMAXPATH];
    PCHAR       pszModPath, pszBitmapPath, pszIcon, pszIconEnd, pszBuffer;
    PCHAR       pszCommand, pszTitle, pszClassName, pszLocation, pszSetupString;
    PCHAR       pszTemp, pszSemicolon;
    HOBJECT     hObject;
    FILE       *source = fopen (pszProfile, "r");

    pFirstObjDef = NULL;

    if (source == NULL)
        return FALSE;

    /* Deskman DRC-Files einlesen und bearbeiten */
    while (fgets (szLine, CCHMAXRCLINE, source) != NULL)
        {
        /* String zerlegen in Teilstrings */
        pszTemp = szLine;

        /* 1. Commandstring */
        pszTemp = findString (pszTemp);     // Stringanfang suchen
        if (pszTemp == NULL)
            break;                          // fehlerhafter String
        pszTemp = separateString (pszTemp, &pszCommand);
        if (pszTemp == NULL)
            break;                          // fehlerhafter String

        /* 2. Objektdefinition */
        pszTemp = findString (pszTemp);     // Stringanfang suchen
        if (pszTemp == NULL)
            break;                          // fehlerhafter String
        pszTemp = separateSubStrings (pszTemp, &pszTitle, &pszClassName, &pszLocation);
        if (pszTemp == NULL)
            break;

        /* 3. Setupstring */
        pszTemp = findString (pszTemp);     // Stringanfang suchen
        if (pszTemp == NULL)
            break;                          // fehlerhafter String
        pszTemp = separateString (pszTemp, &pszSetupString);
        if (pszTemp == NULL)
            break;                          // fehlerhafter String

        /* Klasse registrieren? */
        if ((pszTemp = strstr (pszSetupString, CLASS_IDENTIFIER)) != NULL)
            {
            pszSemicolon = strchr (pszTemp, ';');
            if (pszSemicolon != NULL)
                *pszSemicolon = '\0';
            WinRegisterObjectClass (pszClassName, pszTemp+sizeof (CLASS_IDENTIFIER)-1);
            if (pszSemicolon != NULL)
                strcpy (pszTemp, pszSemicolon+1);
            else
                *pszTemp = '\0';
            }

        /* Icon-/Bitmap-Dateien werden aus dem DRC-Verzeichnis geholt, wenn sie aus dem Deskman-Pfad stammen */
        pszModPath    = (PCHAR)malloc (CCHMAXRCLINE);
        pszBitmapPath = (PCHAR)malloc (CCHMAXPATH);
        pszBuffer     = (PCHAR)malloc (CCHMAXPATHCOMP);

        /* Pfad fÅr Icondateien entspricht Pfad fÅr DRC-Datei */
        ulc = PrfQueryProfileString (HINI_SYSTEMPROFILE, OS2SYS_APP_NAME, OS2SYS_KEY_CLNT_LOCAL, NULL, szUNC, CCHMAXPATH);
        if (ulc == 0)
            szUNC[0] = '\0';

        if (pszModPath)
            {
            /* Der Deskman-Pfad stammt aus dem Ini-Eintrag MAGPRO_ICONSRC_KEY */
            ulc = QueryLocINIString (MAGPRO_FILE_APP, MAGPRO_ICONSRC_KEY, pszBitmapPath, CCHMAXPATH);
            if (ulc)
                {
                /* ICONFILE=<path> */
                pszIcon = strstr (pszSetupString, SETUP_ICONFILE);
                if (pszIcon)
                    {
                    pszIconEnd = strchr (pszIcon, '=');
                    if (pszIconEnd)
                        {
                        if (strnicmp (pszIconEnd+1, pszBitmapPath, strlen (pszBitmapPath)) == 0)
                            {
                            memcpy (pszModPath, pszSetupString, (ULONG)(pszIconEnd-pszSetupString)+1);
                            strcpy (pszModPath+(ULONG)(pszIconEnd-pszSetupString)+1, szUNC);
                            strcat (pszModPath, "\\");
                            strcat (pszModPath, ExtractFilename (pszIconEnd+1, pszBuffer));
                            strcat (pszModPath, RestOfString (pszIconEnd+1));
                            strcpy (pszSetupString, pszModPath);
                            }
                        }
                    }

                DebugE (D_SAM, "SetupString nach ICONFILE", pszSetupString);

                /* ICONNFILE=<index>,<path> */
                pszIcon = strstr (pszSetupString, SETUP_ICONNFILE);
                if (pszIcon)
                    {
                    pszIconEnd = strchr (pszIcon, '=');
                    if (pszIconEnd)
                        {
                        pszIconEnd = strchr (pszIconEnd, ',');
                        if (pszIconEnd)
                            {
                            if (strnicmp (pszIconEnd+1, pszBitmapPath, strlen (pszBitmapPath)) == 0)
                                {
                                memcpy (pszModPath, pszSetupString, (ULONG)(pszIconEnd-pszSetupString)+1);
                                strcpy (pszModPath+(ULONG)(pszIconEnd-pszSetupString)+1, szUNC);
                                strcat (pszModPath, "\\");
                                strcat (pszModPath, ExtractFilename (pszIconEnd+1, pszBuffer));
                                strcat (pszModPath, RestOfString (pszIconEnd+1));
                                strcpy (pszSetupString, pszModPath);
                                }
                            }
                        }
                    }
                DebugE (D_SAM, "SetupString nach ICONNFILE", pszSetupString);

                /* BACKGROUND=<path>,<mode>,<scale>,<type>,<color> */
                pszIcon = strstr (pszSetupString, SETUP_BGBITMAP);
                if (pszIcon)
                    {
                    pszIconEnd = strchr (pszIcon, '=');
                    if (pszIconEnd)
                        {
                        if (strnicmp (pszIconEnd+1, pszBitmapPath, strlen (pszBitmapPath)) == 0)
                            {
                            memcpy (pszModPath, pszSetupString, (ULONG)(pszIconEnd-pszSetupString)+1);
                            strcpy (pszModPath+(ULONG)(pszIconEnd-pszSetupString)+1, szUNC);
                            strcat (pszModPath, "\\");
                            strcat (pszModPath, ExtractFilename (pszIconEnd+1, pszBuffer));
                            strcat (pszModPath, RestOfString (pszIconEnd+1));
                            strcpy (pszSetupString, pszModPath);
                            }
                        }
                    }
                DebugE (D_SAM, "SetupString nach BACKGROUND", pszSetupString);
                }

            DebugE (D_SAM, "FreeMem", "Bitmaps1");
            free (pszBitmapPath);
            DebugE (D_SAM, "FreeMem", "Bitmaps2");
            free (pszModPath);
            DebugE (D_SAM, "FreeMem", "Bitmaps3");
            }

        /* Objekt generieren */
        DebugE (D_SAM, "WinCreateObject", pszTitle);
        ulc = sizeof (ulFlags);
        if (!QueryLocINIData (MAGPRO_SYSTEM_APP, MAGPRO_CO_FLAG_KEY, &ulFlags, &ulc))
            ulFlags = CO_UPDATEIFEXISTS;
        hObject = WinCreateObject (pszClassName, pszTitle, pszSetupString, pszLocation, ulFlags);
        if (hObject != NULLHANDLE)
            {
            if (strcmp (pszClassName, CLASS_LAUNCHPAD) == 0)
                {
                /* Launchpad-EintrÑge ergÑnzen */
                DebugE (D_SAM, "CreateObjects", "CreateLaunchpadSetupString");
                CreateLaunchpadSetupString (hObject, pszSetupString);
                }
            else
                AddObjDefEntry (pszClassName, pszTitle, pszLocation, pszSetupString);
            }
        }

    DeleteObjDefs ();

    if (pszTemp)
        bRC = TRUE;

    fclose (source);

    return bRC;
    }
