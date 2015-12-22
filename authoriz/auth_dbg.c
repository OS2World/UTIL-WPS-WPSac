/***************************************************************************
 *
 * PROGRAM NAME: AUTH_DBG.C
 * -------------
 *
 * REVISION LEVEL: 1.0
 * ---------------
 *
 * WHAT THIS PROGRAM DOES:
 * -----------------------
 *  Anzeigen der Werte im Authorize Sh-Mem
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
 *
 *  Copyright (C) 1995 Noller & Breining Software
 *
 ******************************************************************************/
#define INCL_WIN
#define INCL_PM
#define INCL_DOS
#define INCL_DOSSESMGR      /* Session Manager values */
#define INCL_DOSQUEUES      /* Queue values */
#define INCL_DOSERRORS

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#define SHAREDMEMNAME "\\SHAREMEM\\AUTHORIZ"

/* Globale Variablen */
CHAR *pShareMem = 0;
CHAR *poi;
int  i;

int main (void)
    {

    if (!(DosGetNamedSharedMem ((PPVOID) &pShareMem,
                              SHAREDMEMNAME,
                              PAG_READ | PAG_WRITE)))
        {
        printf ("Contents of shared memory:\n");
        poi = pShareMem;

        while (*poi == 1)
            {
            for (i=0; i<poi[2];i++)
                {
                printf("%02X, ", poi[i]);
                if (i % 16 == 15)
                    printf("\n");
                }
            printf("\n\n");
            poi += poi[2];
            }

        }

    else
        printf ("Shared memory not found!\n");

    return 0;
    }

