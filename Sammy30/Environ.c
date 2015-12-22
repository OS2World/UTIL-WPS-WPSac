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
#define INCL_WIN
#define INCL_PM
#define INCL_DOS            /* ?? */
#define INCL_DOSSESMGR      /* Session Manager values */
#define INCL_DOSQUEUES      /* Queue values */
#define INCL_DOSERRORS

#include <os2.h>
#include <string.h>

#include "samglob.h"
#include "magpro2.h"

#include "Environ.h"
#include "profile.h"
#include "SamMem.h"
#include "SamData.h"
#include "sammy.h"
#include "debug.h"

/********************************************************************
 *  Erweitern der Environment
 *  Eingang: pszUser: Benutzername
 ********************************************************************/
PCHAR CreateEnvironment (PSZ pszUser)
    {
    PSZ   pszEnvStrings = NULL;
    PSZ   pszKeyValue   = NULL;
    BOOL  bRC;
    ULONG ulSize, ulIndex, ulEnvSize;

    /* LÑnge der INI-EintrÑge aus SharedMem-Bereich bestimmen */
    ulEnvSize = 1;          // 1 Extra-Byte fÅr Terminierungszeichen
    if (pShareMem->szUserIni && strlen (pShareMem->szUserIni))
        ulEnvSize += sizeof (ENV_USER_INI)   + 1 + strlen (pShareMem->szUserIni);
    if (pShareMem->szSystemIni && strlen (pShareMem->szSystemIni))
        ulEnvSize += sizeof (ENV_SYSTEM_INI) + 1 + strlen (pShareMem->szSystemIni);

    /* Environment-Liste auslesen */
    bRC = QueryLocINISize (pszUser, MAGPRO_ENVLIST_KEY, &ulSize);
    bRC = (!bRC || !ulSize) ? FALSE : TRUE;
    if (bRC)
        {
        pszKeyValue = (PSZ)AllocMem (ulSize);

        if (!QueryLocINIData (pszUser, MAGPRO_ENVLIST_KEY, pszKeyValue, &ulSize))
            pszKeyValue[0] = '\0';

        /* 1. Schleife Åber alle Environment-EintrÑge: LÑnge bestimmen */
        for (ulIndex = 0; pszKeyValue[ulIndex] != '\0';)
            {
            QueryLocINISize (MAGPRO_ENVIRONM_APP, pszKeyValue, &ulSize);

            /* sicherheitshalber 1 Byte mehr allokieren, falls '\0' in INI-Datei fehlt */
            ulEnvSize += ulSize + 1;

            ulIndex += strlen (&pszKeyValue[ulIndex]) + 1;
            }
        }

    pszEnvStrings = (PSZ)AllocMem (ulEnvSize);

    if (pszEnvStrings)
        {
        /* INI-EintrÑge aus SharedMem-Bereich bestimmen */
        ulSize = 0;
        if (pShareMem->szUserIni && strlen (pShareMem->szUserIni))
            {
            strcpy (pszEnvStrings, ENV_USER_INI);
            strcat (pszEnvStrings, "=");
            strcat (pszEnvStrings, pShareMem->szUserIni);
            ulSize += strlen (pszEnvStrings);
            pszEnvStrings[ulSize++] = '\r';
            }
        if (pShareMem->szSystemIni && strlen (pShareMem->szSystemIni))
            {
            strcpy (&pszEnvStrings[ulSize], ENV_SYSTEM_INI);
            strcat (&pszEnvStrings[ulSize], "=");
            strcat (&pszEnvStrings[ulSize], pShareMem->szSystemIni);
            ulSize += strlen (&pszEnvStrings[ulSize]);
            pszEnvStrings[ulSize++] = '\r';
            }

        if (bRC)
            {
            /* 2. Schleife Åber alle Environment-EintrÑge: Puffer fÅllen */
            for (ulIndex = 0; pszKeyValue[ulIndex] != '\0';)
                {
                ulSize    += QueryLocINIString (MAGPRO_ENVIRONM_APP, pszKeyValue, pszEnvStrings + ulSize, ulEnvSize);
                ulEnvSize -= ulSize;

                pszEnvStrings[ulSize-1] = '\r';

                ulIndex += strlen (&pszKeyValue[ulIndex]) + 1;
                }
            }

        pszEnvStrings[ulSize] = '\0';
        }

    DebugE (D_SAM, "FreeMem", "ceKeyNames1");
    FreeMem ((PPVOID)&pszKeyValue);
    DebugE (D_SAM, "FreeMem", "ceKeyNames2");

    return pszEnvStrings;
    }
