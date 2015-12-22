/***************************************************************************\
 *
 * PROGRAMMNAME: WPDCF77
 * -------------
 *
 * VERSION: 4.0
 * --------
 *
 * MODULNAME: CLOCK
 * ----------
 *
 * BESCHREIBUNG:
 * -------------
 *   Erzeugen des Frame- und Client-Windows fÅr die Uhr, Frame-Window-Prozedur
 *
 * HINWEISE:
 * ---------
 *
 * FUNKTIONEN:
 * -----------
 *   ClkInit ()
 *   ClkFrameWndProc ()
 *   MessageBox ()
 *
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *  1.00    10-23-94  First release
 *  2.00    06-16-95  Extensions
 *  3.00    10-30-95  Client/Server Support; SOM2
 *  4.00    03-15-96  UnterstÅtzung fÅr hopf
 *
 *  Copyright (C) Noller & Breining Software 1995
 *
\******************************************************************************/
#define INCL_DOSFILEMGR
#define INCL_GPI
#define INCL_DOS

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

#include "bitmap.h"
#include "SamMem.h"
#include "debug.h"

typedef union _UBMPARRAYFILEHEADER
    {
    BITMAPARRAYFILEHEADER   bafh;
    BITMAPARRAYFILEHEADER2  bafh2;
    } UBMPARRAYFILEHEADER;
typedef UBMPARRAYFILEHEADER *PUBMPARRAYFILEHEADER;

typedef union _UBMPFILEHEADER
    {
    BITMAPFILEHEADER        bfh;
    BITMAPFILEHEADER2       bfh2;
    } UBMPFILEHEADER;
typedef UBMPFILEHEADER *PUBMPFILEHEADER;

typedef union _UBMPINFO
    {
    BITMAPINFO              bmi;
    BITMAPINFO2             bmi2;
    } UBMPINFO;
typedef UBMPINFO *PUBMPINFO;

typedef union _UBMPINFOHEADER
    {
    BITMAPINFOHEADER        bmih;
    BITMAPINFOHEADER2       bmih2;
    } UBMPINFOHEADER;
typedef UBMPINFOHEADER *PUBMPINFOHEADER;

typedef struct _BMPFILEDATA
    {
    CAPS            strucCaps;                  // cx, cy, Zahl der Farben
    UBMPFILEHEADER  unBmpFileHdr;               // BITMAPFILEHEADER(2)
    PUBMPINFOHEADER pOffsetInfoHdr;             // Pos. BITMAPINFOHEADER(2) innerhalb BITMAPFILEHEADER(2)
    ULONG           ulSizePalette;              // Grî·e der Palette in Byte
    ULONG           ulcInfoHeader;              // Grî·e des BITMAPINFOHEADER(2)
    ULONG           ulFileOffsetPalette;
    ULONG           ulcData;                    // Grî·e des Datenbereiches in Byte
    } BMPFILEDATA;
typedef BMPFILEDATA *PBMPFILEDATA;

BOOL readBuffer (HFILE hFile, PVOID pBuffer, ULONG ulcBuffer)
    {
    ULONG ulLen;

    if (!DosRead (hFile, pBuffer, ulcBuffer, &ulLen) &&
        ulLen == ulcBuffer)
        return TRUE;
    else
        return FALSE;
    }

PCAPS getNearestBitmap (PCAPS pTarget, PCAPS pSource1, PCAPS pSource2)
    {
    LONG lDiff1, lDiff2;

    if (pTarget && pSource1 && pSource2)
        {

        /* Zuerst Auflîsung prÅfen */
        lDiff1 = abs (pTarget->cx - pSource1->cx) + abs (pTarget->cy - pSource1->cy) -
                 abs (pTarget->cx - pSource2->cx) - abs (pTarget->cy - pSource2->cy);

        if (lDiff1 > 0)
            return pSource2;

        if (lDiff1 < 0)
            return pSource1;

        /* Auflîsungs-Unterschiede sind gleich -> Farbzahl vergleichen */
        lDiff1 = pSource1->ulMaxColors - pTarget->ulMaxColors;
        lDiff2 = pSource2->ulMaxColors - pTarget->ulMaxColors;

        if ((lDiff1 <  0 && lDiff2 <  0) ||
            (lDiff2 >= 0 && lDiff2 >= 0))
            {
            if (abs (lDiff1) < abs (lDiff2))
                return pSource1;
            else
                return pSource2;
            }
        if (lDiff1 >= 0)
            return pSource1;
        else
            return pSource2;
        }

    return NULL;
    }

ULONG calcDataSize (ULONG cx, ULONG cy, ULONG ulBitsPerPel)
    {
    ULONG ulcData;

    ulcData  = (cx * ulBitsPerPel +7) / 8;
    ulcData += (4 - (ulcData%4)) & 0x3;
    ulcData *= cy;

    return ulcData;
    }

PVOID getBmpInfo (PUBMPFILEHEADER punBmpFileHdr, PULONG pulcInfoHeader, PCAPS pCaps, PULONG pulcData, PULONG pulSize)
    {
    PVOID pOffsetInfoHdr = NULL;

    switch (*pulcInfoHeader)
        {
        case sizeof (BITMAPINFOHEADER):
            pCaps->ulMaxColors = punBmpFileHdr->bfh.bmp.cPlanes * punBmpFileHdr->bfh.bmp.cBitCount;
            pOffsetInfoHdr     = &punBmpFileHdr->bfh.bmp;
            *pulcData          = calcDataSize (punBmpFileHdr->bfh.bmp.cx, punBmpFileHdr->bfh.bmp.cy, pCaps->ulMaxColors);
            pCaps->cx          = punBmpFileHdr->bfh.bmp.cx;
            pCaps->cy          = punBmpFileHdr->bfh.bmp.cy;
            break;

        case sizeof (WINBITMAPINFOHEADER):
            *pulcInfoHeader = sizeof (BITMAPINFOHEADER2);

        case sizeof (BITMAPINFOHEADER2):
            pCaps->ulMaxColors = punBmpFileHdr->bfh2.bmp2.cPlanes * punBmpFileHdr->bfh2.bmp2.cBitCount;
            pOffsetInfoHdr     = &punBmpFileHdr->bfh2.bmp2;
            if (punBmpFileHdr->bfh2.bmp2.ulCompression == BCA_UNCOMP)
                *pulcData = calcDataSize (punBmpFileHdr->bfh2.bmp2.cx, punBmpFileHdr->bfh2.bmp2.cy, pCaps->ulMaxColors);
            else
                *pulcData = punBmpFileHdr->bfh2.bmp2.cbImage;
            pCaps->cx = punBmpFileHdr->bfh2.bmp2.cx;
            pCaps->cy = punBmpFileHdr->bfh2.bmp2.cy;
            break;
        }

    pCaps->ulMaxColors = 1 << pCaps->ulMaxColors;
    if (pCaps->ulMaxColors >= (1 << 24))
        *pulSize = 0;
    else
        {
        if (*pulcInfoHeader == sizeof (BITMAPINFOHEADER))
            *pulSize = pCaps->ulMaxColors * sizeof (RGB);
        else
            *pulSize = pCaps->ulMaxColors * sizeof (RGB2);
        }

    return pOffsetInfoHdr;
    }

ULONG loadBitmapData (HFILE hFile, PBMPFILEDATA pBmpDescr, PUBMPINFO *ppunBmpInfo, PBYTE *ppData, PPOINTL pHotspot)
    {
    ULONG ulPosition;
    PVOID pOffsetPalette;
    BOOL  bRC;
    ULONG rc;

    /* Speicher allokieren fÅr BITMAPINFO(2)-Struktur */
    *ppunBmpInfo = AllocMem (pBmpDescr->ulcInfoHeader + pBmpDescr->ulSizePalette);
    if (*ppunBmpInfo == NULL)
        return BMPERR_INVBMP;

    /* BITMAPINFO-Struktur mit bereits gelesenen Daten fÅllen */
    memcpy (*ppunBmpInfo, pBmpDescr->pOffsetInfoHdr, pBmpDescr->ulcInfoHeader);
    (*ppunBmpInfo)->bmi.cbFix = pBmpDescr->ulcInfoHeader;  // Korrektur fÅr Win-Bmp's

    /* Rest der BITMAPINFO-Struktur (Palettendaten) aus Datei lesen */
    if (pBmpDescr->ulSizePalette)
        {
        if (pBmpDescr->ulcInfoHeader == sizeof (BITMAPINFOHEADER))
            pOffsetPalette = &((*ppunBmpInfo)->bmi.argbColor);
        else
            pOffsetPalette = &((*ppunBmpInfo)->bmi2.argbColor);

        DosSetFilePtr (hFile, pBmpDescr->ulFileOffsetPalette, FILE_BEGIN, &ulPosition);
        if (!(bRC = readBuffer (hFile, (PVOID)pOffsetPalette, pBmpDescr->ulSizePalette)))
            return BMPERR_INVBMP;
        }

    /* Bitmapdaten lesen */
    *ppData = AllocMem (pBmpDescr->ulcData);
    if (*ppData == NULL)
        goto Error;

    DosSetFilePtr (hFile, pBmpDescr->unBmpFileHdr.bfh.offBits, FILE_BEGIN, &ulPosition);
    if (!(bRC = readBuffer (hFile, (PVOID)*ppData, pBmpDescr->ulcData)))
        {
        FreeMem ((PPVOID)ppData);
        ppData = NULL;
        goto Error;
        }

    /* Bestimmen des Hotspot-Punktes */
    if (pHotspot)
        {
        (*pHotspot).x = (LONG)pBmpDescr->unBmpFileHdr.bfh.xHotspot;
        (*pHotspot).y = (LONG)pBmpDescr->unBmpFileHdr.bfh.yHotspot;
        if ((*pHotspot).x >= pBmpDescr->strucCaps.cx)
            (*pHotspot).x  = pBmpDescr->strucCaps.cx;
        if ((*pHotspot).y >= pBmpDescr->strucCaps.cy)
            (*pHotspot).y  = pBmpDescr->strucCaps.cy;
        }

    return BMPERR_OK;

Error:
    /* Bereits allokierten Speicher wieder freigeben */
    FreeMem ((PPVOID)ppunBmpInfo);
    ppunBmpInfo = NULL;
    rc = BMPERR_INVBMP;

    return rc;
    }

ULONG loadBitmap (HFILE hFile, PUBMPINFO *ppunBmpInfo, PBYTE *ppData, PPOINTL pHotspot)
    {
    BOOL             bRC;
    BMPFILEDATA      strucBmpData;
    PVOID            pOffsetInfoHdr;

    *ppunBmpInfo = NULL;
    *ppData      = NULL;

    memset (&strucBmpData.unBmpFileHdr, 0, sizeof (UBMPFILEHEADER));

    /* BITMAPFILEHEADER bis BITMAPINFOHEADER-LÑnge lesen und öberprÅfung */
    bRC = readBuffer (hFile, (PVOID)&strucBmpData.unBmpFileHdr,
                      sizeof (BITMAPFILEHEADER) - sizeof (BITMAPINFOHEADER) + sizeof (ULONG));

    if (!bRC || (strucBmpData.unBmpFileHdr.bfh.usType != BFT_BMAP))
        return BMPERR_INVBMP;

    /* Bestimmen des Bitmaptyps */
    switch (strucBmpData.unBmpFileHdr.bfh.bmp.cbFix)     // (ohne PaletteneintrÑge)
        {
        case sizeof (BITMAPINFOHEADER):
        case sizeof (WINBITMAPINFOHEADER):
        case sizeof (BITMAPINFOHEADER2):
            break;

        default:
            return BMPERR_INVBMP;
        }
    strucBmpData.ulcInfoHeader = strucBmpData.unBmpFileHdr.bfh.bmp.cbFix;

    /* Lesen des Restes des BITMAPFILEHEADER(2)s */
    if (!readBuffer (hFile, (PVOID)&strucBmpData.unBmpFileHdr.bfh.bmp.cx, strucBmpData.ulcInfoHeader - sizeof (ULONG)))
        return BMPERR_INVBMP;

    /* Bestimmen der LÑnge der BITMAPINFO-Struktur einschl. Paletteninfo */
    pOffsetInfoHdr =  getBmpInfo (&strucBmpData.unBmpFileHdr,
                                  &strucBmpData.ulcInfoHeader,
                                  &strucBmpData.strucCaps,
                                  &strucBmpData.ulcData,
                                  &strucBmpData.ulSizePalette);

    strucBmpData.pOffsetInfoHdr = (PUBMPINFOHEADER)pOffsetInfoHdr;
    DosSetFilePtr (hFile, 0, FILE_CURRENT, &strucBmpData.ulFileOffsetPalette);

    return loadBitmapData (hFile, &strucBmpData, ppunBmpInfo, ppData, pHotspot);
    }

ULONG loadBitmapArray (HFILE hFile, PCAPS pCaps, PUBMPINFO *ppunBmpInfo, PBYTE *ppData, PPOINTL pHotspot)
    {
    UBMPARRAYFILEHEADER unBmpArFileHdr;
    CAPS                strucCapsActual;
    PCAPS               pCapsBestFit;
    BMPFILEDATA         strucBmpBestFit;
    BOOL                bRC;
    BOOL                bIsFirstBmp;
    PVOID               pOffsetInfoHdr;
    ULONG               ulcInfoHeader;
    ULONG               ulcPalette;
    ULONG               ulcData;                   // Bitmap-Daten lesen
    ULONG               ulPosition;

    bIsFirstBmp                           = TRUE;
    unBmpArFileHdr.bafh.offNext           = 0;
    strucBmpBestFit.strucCaps.cx          =
    strucBmpBestFit.strucCaps.cy          =
    strucBmpBestFit.strucCaps.ulMaxColors = 0;

    do
        {
        DosSetFilePtr (hFile, unBmpArFileHdr.bafh.offNext, FILE_BEGIN, &ulPosition);

        /* BITMAPARRAYFILEHEADER lesen */
        bRC = readBuffer (hFile, (PVOID)&unBmpArFileHdr,
                          sizeof (BITMAPARRAYFILEHEADER) - sizeof (BITMAPFILEHEADER));

        if (!bRC)
            break;                              // Ende: letztes Bitmap gelesen

        /* BITMAPFILEHEADER bis BITMAPINFOHEADER-LÑnge lesen und öberprÅfung */
        bRC = readBuffer (hFile, (PVOID)&unBmpArFileHdr.bafh.bfh,
                          sizeof (BITMAPFILEHEADER) - sizeof (BITMAPINFOHEADER) + sizeof (ULONG));

        if (!bRC || (unBmpArFileHdr.bafh.bfh.usType != BFT_BMAP))
            return BMPERR_INVBMP;

        /* Bestimmen des Bitmaptyps */
        switch (unBmpArFileHdr.bafh.bfh.bmp.cbFix)     // (ohne PaletteneintrÑge)
            {
            case sizeof (BITMAPINFOHEADER):
            case sizeof (BITMAPINFOHEADER2):
                break;

            default:
                return BMPERR_INVBMP;
            }
        ulcInfoHeader = unBmpArFileHdr.bafh.bfh.bmp.cbFix;

        /* Lesen des Restes des BITMAPFILEHEADER(2)s */
        if (!readBuffer (hFile, (PVOID)&unBmpArFileHdr.bafh.bfh.bmp.cx, ulcInfoHeader - sizeof (ULONG)))
            return BMPERR_INVBMP;

        pOffsetInfoHdr = getBmpInfo ((PUBMPFILEHEADER)&unBmpArFileHdr.bafh.bfh,
                                     &ulcInfoHeader,
                                     &strucCapsActual,
                                     &ulcData,
                                     &ulcPalette);

        pCapsBestFit = getNearestBitmap (pCaps, &strucCapsActual, &strucBmpBestFit.strucCaps);
        if (pCapsBestFit == &strucCapsActual || bIsFirstBmp)
            {
            bIsFirstBmp = FALSE;

            /* Die 2 Varianten der Union UBMPARRAYFILEHEADER sind bis zu bafh.bfh bzw. bafh2.bfh2 identisch */
            memcpy (&strucBmpBestFit.unBmpFileHdr, &unBmpArFileHdr.bafh.bfh, sizeof (UBMPFILEHEADER));
            strucBmpBestFit.strucCaps           = strucCapsActual;
            strucBmpBestFit.pOffsetInfoHdr      = (PUBMPINFOHEADER)
                                                     ((PBYTE)&strucBmpBestFit.unBmpFileHdr +
                                                      ((PBYTE)pOffsetInfoHdr - (PBYTE)&unBmpArFileHdr.bafh.bfh));
            strucBmpBestFit.ulSizePalette       = ulcPalette;
            strucBmpBestFit.ulcInfoHeader       = ulcInfoHeader;
            strucBmpBestFit.ulFileOffsetPalette = ulPosition + unBmpArFileHdr.bafh.cbSize;
            strucBmpBestFit.ulcData             = ulcData;
            }
        } while (unBmpArFileHdr.bafh.offNext);

    return loadBitmapData (hFile, &strucBmpBestFit, ppunBmpInfo, ppData, pHotspot);
    }

ULONG loadNearestBitmap (PSZ pszFilename, PCAPS pCaps, PUBMPINFO *ppunBmpInfo, PBYTE *ppData, PPOINTL pHotspot)
    {
    HFILE            hFile;                     // Dateihandle
    ULONG            ulAction;
    ULONG            rc;
    USHORT           usType;
    BOOL             bRC;

    /* Datei îffnen */
    bRC = DosOpen (pszFilename,
                   &hFile,
                   &ulAction,
                   0,
                   FILE_NORMAL,
                   OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                   OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYWRITE | OPEN_ACCESS_READONLY,
                   NULL) ? FALSE : TRUE;

    if (!bRC)
        return BMPERR_OPENERR;

    /* PrÅfen, ob korrekter Header vorliegt */
    bRC = readBuffer (hFile, (PVOID)&usType, sizeof (usType));
    if (bRC)
        {
        DosSetFilePtr (hFile, 0, FILE_BEGIN, &ulAction);

        switch (usType)
            {
            case BFT_BMAP:
                rc = loadBitmap (hFile, ppunBmpInfo, ppData, pHotspot);
                break;

            case BFT_BITMAPARRAY:
                rc = loadBitmapArray (hFile, pCaps, ppunBmpInfo, ppData, pHotspot);
                break;
            }
        }

    /* Datei schlie·en */
    DosClose (hFile);
    return rc;
    }

ULONG createBitmap (HPS hps, PSZ pszFilename, PHBITMAP phBM, PCAPS pCaps, PPOINTL pHotspot)
    {
    ULONG     rc = BMPERR_OK;
    PUBMPINFO punBmpInfo;
    PBYTE     pData;

    *phBM = NULLHANDLE;

    if (strlen (pszFilename) && hps)
        {
        if ((rc = loadNearestBitmap (pszFilename, pCaps, &punBmpInfo, &pData, pHotspot)) == BMPERR_OK)
            {
            if (punBmpInfo && pData)
                {
                *phBM = GpiCreateBitmap (hps,
                                         (PBITMAPINFOHEADER2)punBmpInfo,
                                         CBM_INIT,
                                         pData,
                                         (PBITMAPINFO2)punBmpInfo);
                }
            if (*phBM == NULLHANDLE)
                rc = BMPERR_INVBMP;
            if (punBmpInfo)
                FreeMem ((PPVOID)&punBmpInfo);
            if (pData)
                FreeMem ((PPVOID)&pData);
            }
        }

    return rc;
    }

