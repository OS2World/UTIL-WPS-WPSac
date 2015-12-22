/***************************************************************************
 *
 * MODULNAME: MAGSUPP
 * -------------
 *
 * VERSION: 1.0
 * --------
 *
 * BESCHREIBUNG:
 * -------------
 *   Das Modul kapselt die MagnetkartenunterstÅtzung von der
 *   Åbrigen Applikation. Es arbeitet mit einem zusÑtzlichen
 *   Thread, der die Lesefunktion Åbernimmt. Dieser Thread
 *   ist eine Endlosschleife, die in einem anderen Modul
 *   implementiert sein mu·.
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *   IBM C/C++ Set/2 Version 1.2
 *   OS/2 Linker LINK386.EXE
 *
 * BENôTIGTE FILES:
 * ----------------
 *   MAGSUPP.C    - C Sourcecode
 *   SAMGLOB.H    - C Include-Datei
 *   MAGCALLS.H   - C Include-Datei fÅr MAGCALLS.DLL
 *
 * BENôTIGTE BIBLIOTHEKEN:
 * -----------------------
 *   IBM C Multithreaded Runtime Library
 *   OS2386.LIB OS/2 Import-Library fÅr 32-Bit API
 *
 *  Copyright (C) 1993 Noller & Breining Software 1994
 *
 ******************************************************************************/
#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#include <os2.h>
#include <stdlib.h>

#include <magcalls.h>

#include "samglob.h"
#include "MagSupp.h"

#define LIBNAME     "MAGCALLS"          /* Name der DLL-Datei */
#define LDERRSIZE   128                 /* LÑnge des Object-Buffers von DosLoadModule */
#define STACKSIZE   16384               /* Stacksize des Threads */

CHAR    LoadErr[LDERRSIZE];
HMODULE hmodMag = NULLHANDLE;
HMAG    hMag    = NULLHANDLE;

APIRET16 (* APIENTRY16 pfnMagOpen) (PHMAG);
APIRET16 (* APIENTRY16 pfnMagClose) (HMAG);
APIRET16 (* APIENTRY16 pfnMagSetOperationMode) (HMAG, OPMODE);
APIRET16 (* APIENTRY16 pfnMagResetDevice) (HMAG);
APIRET16 (* APIENTRY16 pfnMagAbort) (HMAG);
APIRET16 (* APIENTRY16 pfnMagReadData) (HMAG, USHORT, PBYTE, PUSHORT);
APIRET16 (* APIENTRY16 pfnMagReadConfigStatus) (HMAG, PUSHORT);

/*******************************************************************
   Initialisiert die Kartenleser-UnterstÅtzung. Es wird
   MAGCALLS.DLL dynamisch geladen und die ModuleinsprÅnge ermittelt.
   return:  TRUE:  Init ok.
            FALSE: Fehler aufgetreten.
 *******************************************************************/
BOOL MCInit (void)
    {
    BOOL     rc;

    if (hmodMag != NULLHANDLE)
        return TRUE;

    rc = DosLoadModule (LoadErr, LDERRSIZE, LIBNAME, &hmodMag) ? FALSE : TRUE;

    if (rc)
        {
        rc  = DosQueryProcAddr (hmodMag, 0, "MAGOPEN",
            (PFN *)&pfnMagOpen) ? FALSE : TRUE;
        rc &= DosQueryProcAddr (hmodMag, 0, "MAGCLOSE",
            (PFN *)&pfnMagClose) ? FALSE : TRUE;
        rc &= DosQueryProcAddr (hmodMag, 0, "MAGSETOPERATIONMODE",
            (PFN *)&pfnMagSetOperationMode) ? FALSE : TRUE;
        rc &= DosQueryProcAddr (hmodMag, 0, "MAGRESETDEVICE",
            (PFN *)&pfnMagResetDevice) ? FALSE : TRUE;
        rc &= DosQueryProcAddr (hmodMag, 0, "MAGABORT",
            (PFN *)&pfnMagAbort) ? FALSE : TRUE;
        rc &= DosQueryProcAddr (hmodMag, 0, "MAGREADDATA",
            (PFN *)&pfnMagReadData) ? FALSE : TRUE;
        rc &= DosQueryProcAddr (hmodMag, 0, "MAGREADCONFIGSTATUS",
            (PFN *)&pfnMagReadConfigStatus) ? FALSE : TRUE;
        }

    if (!rc)
        hmodMag = NULLHANDLE;
    return rc;
    }

/*******************************************************************
   Beendet die Kartenleser-UnterstÅtzung. Das Modul
   wird MAGCALLS.DLL wieder freigegeben.
   return:  TRUE:  Free ok.
            FALSE: Fehler aufgetreten.
 *******************************************************************/
BOOL MCFree (void)
    {
    BOOL rc;

    rc = DosFreeModule(hmodMag) ? FALSE : TRUE;

    hmodMag = NULLHANDLE;
    return rc;
    }

/*******************************************************************
   ôffnet den Kartenleser.
   return:  TRUE:  Open ok.
            FALSE: Fehler aufgetreten.
 *******************************************************************/
BOOL MCOpen (void)
    {
    BOOL   rc;
    USHORT us;

    if (hmodMag != NULLHANDLE)
        rc = pfnMagOpen (&hMag) ? FALSE : TRUE;
    else
        rc = FALSE;

    if (rc)
        {
        pfnMagReadConfigStatus (hMag, &us);

        if (!(us & CFG_READT2))
            {
            pfnMagClose (hMag);
            rc = FALSE;
            }
        }

    if (!rc)
        hMag = NULLHANDLE;

    return rc;
    }

/*******************************************************************
   Schlie·t den Kartenleser.
   return:  TRUE:  Close ok.
            FALSE: Fehler aufgetreten.
 *******************************************************************/
BOOL MCClose (void)
    {
    BOOL rc;

    if (hmodMag && hMag)
        rc = pfnMagClose (hMag) ? FALSE : TRUE;
    else
        rc = FALSE;

    hMag = 0;
    return rc;
    }

/*******************************************************************
   Bricht den aktuellen Lesevorgang ab
   return:  TRUE:  Abort ok.
            FALSE: Fehler aufgetreten.
 *******************************************************************/
BOOL MCAbort (void)
    {
    BOOL rc;

    if (hmodMag && hMag)
        rc = pfnMagAbort (hMag) ? FALSE : TRUE;
    else
        rc = FALSE;

    return rc;
    }

/*******************************************************************
   Lese-Thread. Der Thread ruft MCRead, solange diese Funktion
   mit TRUE zurÅckkehrt. In diesem Fall wird MAGNET->ulNotification
   mit mp1 = mp2 = NULL gepostet.
   Im Fehlerfall (return (MCRead) == FALSE) beendet der Thread
   Eingang: pMagnet : Zeiger auf MAGNET-Struktur
 *******************************************************************/
void _Optlink thMCRead (void *pmagnet)
    {
    while (MCRead ((PMAGNET) pmagnet))
        WinPostMsg (((PMAGNET)pmagnet)->hwnd,
                    ((PMAGNET)pmagnet)->ulNotification,
                    0, 0);

    return;
    }

/*******************************************************************
   Startet den Lese-Thread.
   Eingang: pMagnet : Zeiger auf MAGNET-Struktur
   return:  TRUE:  Thread gestartet.
            FALSE: Fehler aufgetreten.
 *******************************************************************/
BOOL MCStart (PMAGNET pMagnet)
    {
    if (!(hmodMag && hMag))
        return FALSE;

    return (_beginthread (thMCRead, NULL, STACKSIZE, (void *) pMagnet) == -1) ?
        FALSE : TRUE;
    }

/*******************************************************************
   Modulinterne Funktion: Konvertiert das untere Nibble von b
   in ein ASCII-Zeichen (Ñhnlich _itoa).
   Eingang: b : Byte; oberes Nibble wird maskiert
   return:  Ergebnis
 *******************************************************************/
CHAR Hex2Char (BYTE b)
    {
    b &= 0x0F;
    return (b>9) ? b + 'A' - 0x0A : b + '0';
    }

/*******************************************************************
   Lese-Funktion innerhalb des Lese-Threads. Der Lese-Thread mu·
   innerhalb seiner Schleife diese Funktion rufen.
   Eingang: pMagnet : Zeiger auf MAGNET-Struktur
   return:  TRUE:  Lesen erfolgreich.
            FALSE: Fehler aufgetreten.
 *******************************************************************/
BOOL MCRead (PMAGNET pMagnet)
    {
    APIRET16 rcMag;
    CHAR     szData[CCHMAXTRACKDATA];
    USHORT   usTransferCount;
    ULONG    j;

    rcMag = (hmodMag == NULLHANDLE) ? MAGERR_NOTAVL : MAGERR_OK;

    if (rcMag == MAGERR_OK)
        {
        pfnMagSetOperationMode (hMag, 0);
        usTransferCount = sizeof (szData);
        rcMag = pfnMagReadData (hMag, RDTRK_TRACK2,
            (CHAR * _Seg16) szData, &usTransferCount);

        /* Wenn zuviele Daten auf der Karte sind, hinten abschneiden */
        if (rcMag == MAGERR_BUFFSIZ)
            rcMag = MAGERR_OK;

        if (rcMag == MAGERR_OK)
            if ((((PREADDATA) szData)->SVal & (SVAL_BLANK + SVAL_READERR)) == 0)
                {
                for (j=2; j < usTransferCount; j++)
                    pMagnet->pszReadData[j-2] = Hex2Char (szData[j]);
                pMagnet->pszReadData[j-2] = '\0';
                }
        }

    return rcMag ? FALSE : TRUE;
    }
