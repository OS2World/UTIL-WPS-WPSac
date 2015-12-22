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
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_WINSHELLDATA
#include <os2.h>
#include <string.h>

#include "Authoriz.h"
#include "Authorize.h"
#include "MagPro2.h"
#include "Sammy.h"
#include "debug.h"

static BOOL (* APIENTRY pfnInit)(PSZ pszGrp, PSZ pszUsr) = NULL;    // Funktion aus AUTHORIZ.DLL
static HMODULE hmAuthorize = NULLHANDLE;                            // Handle der Authorisierungs-DLL

static CHAR szErrorText[CCHMAXPATHCOMP];                            // Fehlertext von DosLoadModule

BOOL LoadAuthorize (VOID)
    {
    /*  Einsprungadresse in AUTHORIZ.DLL bestimmen */
    if (!DosLoadModule (szErrorText, CCHMAXPATHCOMP, AUTH_DLLNAME, &hmAuthorize))
        {
        if (DosQueryProcAddr (hmAuthorize, 0, "Init2", (PFN *)&pfnInit))
            {
            DebugS (D_SAM, "fehlerhafte Authorize-DLL");
            UnloadAuthorize ();
            }
        }
    return pfnInit ? TRUE : FALSE;
    }

VOID UnloadAuthorize (VOID)
    {
    DosFreeModule (hmAuthorize);
    hmAuthorize = NULLHANDLE;
    pfnInit     = NULL;

    return;
    }

// ??? ok, falls nicht verfÅgbar ???
BOOL CallAuthorize (PSZ pszGrp, PSZ pszUsr)
    {
    if (pfnInit)
        return (*pfnInit) (pszGrp, pszUsr);
    else
        return FALSE;
    }

VOID InitAuthorize (ALIASDATA *pLoginData)
    {
    CHAR szUserProfile[CCHMAXPATH];
    CHAR szGroupProfile[CCHMAXPATH];
    ULONG ulCount;

    szUserProfile[0]  = '\0';
    szGroupProfile[0] = '\0';

    if (pLoginData->ulFlag & (ALIAS_SAC_USER | ALIAS_SAC_GROUP))
        {
        ulCount = PrfQueryProfileString (HINI_SYSTEMPROFILE, OS2SYS_APP_NAME, OS2SYS_KEY_CLNT_LOCAL, NULL, szUserProfile, CCHMAXPATH);
        szUserProfile[ulCount] = '\0';
        strcat (szUserProfile, "\\");
        strcpy (szGroupProfile, szUserProfile);

        if (pLoginData->ulFlag & ALIAS_SAC_USER)
            {
            strcat (szUserProfile, pLoginData->szSacUser);
            DebugE (D_SAM, "InitAuthorize, pfnInit-String, UserProfile", szUserProfile);
            }

        if (pLoginData->ulFlag & ALIAS_SAC_GROUP)
            {
            strcat (szGroupProfile, pLoginData->szSacGroup);
            DebugE (D_SAM, "InitAuthorize, pfnInit-String, GroupProfile", szGroupProfile);
            }
        }
    CallAuthorize (szGroupProfile, szUserProfile);

    return;
    }

