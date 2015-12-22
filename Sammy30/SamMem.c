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
#define INCL_DOSFILEMGR
#define INCL_DOSMEMMGR
#define INCL_DOSMISC
#define INCL_DOSPROCESS
#include <os2.h>
#include <string.h>

#include "SamData.h"

PVOID AllocMem (ULONG ulSize)
    {
    PVOID pMem;

    if (DosAllocMem (&pMem, ulSize, PAG_COMMIT | PAG_READ | PAG_WRITE))
        pMem = NULL;

    return pMem;
    }

VOID FreeMem (PPVOID ppMem)
    {
    if (*ppMem)
        {
        DosFreeMem (*ppMem);
        *ppMem = NULL;
        }

    return;
    }

/***********************************************************************
    Initialisiert den Shared-Memory-Bereich. Wenn er noch nicht
    existiert, wird er allokiert.
    return: Fehlercode von AllocSharedMem-Aufrufen
 ***********************************************************************/
APIRET InitSharedMem (HWND hwnd)
    {
    PCHAR  pszEnv;
    APIRET rc = 0;

    if (DosGetNamedSharedMem ((PPVOID)&pShareMem,
                              SHARE_INITOS2,
                              PAG_READ | PAG_WRITE))
        {
        if ((rc = DosAllocSharedMem ((PPVOID)&pShareMem,
                               SHARE_INITOS2,
                               CCHSHARE_INITOS2,
                               PAG_COMMIT | PAG_READ | PAG_WRITE)) != 0)
            return rc;                                      // Speicher lÑ·t sich nicht allokieren
        else
            {
            /* Shared Memory initialisieren */
            memset (pShareMem, '\0', CCHSHARE_INITOS2);
            strcpy (pShareMem->szRegFile,
                    DosScanEnv (ENV_SYSTEM_INI, &pszEnv) ? "" : pszEnv);
            }
        }

    pShareMem->ulActive     |= ACTIVE_SAMMY;
    pShareMem->hwndSammy     = hwnd;
    return rc;
    }

