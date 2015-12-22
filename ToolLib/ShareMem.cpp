/***************************************************************************
 *
 * PROGRAM NAME: UPROFILE
 * -------------
 *
 * REVISION LEVEL: 4.0
 * ---------------
 *
 * WHAT THIS MODULE DOES:
 * ----------------------
 *  Zugriffsklasse fÅr den Shared Memory-Bereich von WPSam/WPSac
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *  IBM Visual Age C/C++ Version 3.0
 *
 * REQUIRED FILES:
 * ---------------
 *  SamMem.hpp    -   Header-Datei zur Klasse ShareMem
 *  SamGlob.h     -   Globale Header-Datei fÅr WPSam/WPSac
 *
 * REQUIRED LIBRARIES:
 * -------------------
 *  OS2386.LIB    -   OS/2 32-Bit import library
 *
 * CHANGE LOG:
 * -----------
 *
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *  4.00    05-05-99  Erste Version
 *
 *  Copyright (C) 1994...1999 noller & breining software
 *
 ******************************************************************************/
#define INCL_DOSFILEMGR
#define INCL_DOSMEMMGR
#define INCL_DOSMISC
#define INCL_DOSPROCESS
#include <os2.h>
#include <string.h>

#include "ShareMem.hpp"

ShareMem::ShareMem (void)
    {
    if (DosGetNamedSharedMem ((PPVOID)&pShareMem,
                              SHARE_INITOS2,
                              PAG_READ | PAG_WRITE))
        {
        if (!DosAllocSharedMem ((PPVOID)&pShareMem,
                                SHARE_INITOS2,
                                CCHSHARE_INITOS2,
                                PAG_COMMIT | PAG_READ | PAG_WRITE))
            pShareMem = NULL;
        else
            {
            /* Shared Memory initialisieren */
            PCSZ pszEnv;

            memset (pShareMem, '\0', CCHSHARE_INITOS2);
            strcpy (pShareMem->szRegFile,
                    DosScanEnv (ENV_SYSTEM_INI, &pszEnv) ? "" : pszEnv);
            }
        }

    if (pShareMem)
        pShareMem->ulActive |= ACTIVE_UPROFILE;
    }

ShareMem::~ShareMem (void)
    {
    if (pShareMem)
        {
        /* UPROFILE benîtigt den Shared Memory-Bereich nicht mehr */
        pShareMem->ulActive &= ~ACTIVE_UPROFILE;

        /* Usage count fÅr den Shared Memory-Bereich dekrementieren */
        DosFreeMem (pShareMem);
        }
    }

IBase::Boolean ShareMem::isAdmin (void)
    {
    if (pShareMem)
        return (pShareMem->ulUserFlag & USER_FLAG_ADMIN) ? True : False;

    return False;
    }

