#define INCL_DOSERRORS
#define INCL_DOSPROCESS
#define INCL_DOSSEMAPHORES
#define INCL_GPICONTROL
#define INCL_GPILOGCOLORTABLE
#define INCL_GPILCIDS
#define INCL_GPIPRIMITIVES
#define INCL_WINFRAMEMGR
#define INCL_WINSCROLLBARS
#define INCL_WINSYS
#define INCL_WINWINDOWMGR
#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "SamMem.h"
#include "rxout.h"
#include "SamData.h"
#include "SammyDlg.h"
#include "Sammy.h"
#include "debug.h"

#define CLASS_RXOUT "RexxOutput"

#define STACKSIZE   0x8000
#define CCHMAXTEXT  4096            // Zahl der Zeichen im Textfenster
#define QWP_WNDDATA 0               // Index f�r WINDOWDATA in Window-Data
#define MAXWAITTIME 5000            // 5s

typedef struct
    {
    PSZ     pszTitle;               // Titeltext;
    PHWND   pHwnd;                  // Puffer f�r Window-handle
    HEV     hev;                    // Eventsemaphore f�r Warten auf Window-handle
    } THREADARG, *PTHREADARG;

typedef struct
    {
    LONG    clrBackgnd;             // Farben
    LONG    clrForegnd;
    LONG    cxChar;                 // Schriftart-Eigenschaften
    LONG    cxCaps;
    LONG    cyChar;
    LONG    cyDesc;
    LONG    lcLines;                // Textinformationen
    PCHAR   pszText;
    PCHAR   pszNextText;            // Darstellinformationen
    LONG    cxClient;               // Client-Window Gr��e
    LONG    cyClient;
    LONG    lMaxLen;                // Scrollen: maximale Zeilenl�nge (Weltkoordinaten)
    LONG    lMaxLenOld;             //           maximale Zeilenl�nge vor dem letzten Einf�gen
    LONG    lTextHeight;            //           Texth�he (Weltkoordinaten)
    LONG    lTextHeightOld;         //           Texth�he vor dem letzten Einf�gen
    LONG    lFirstPosY;             //           y-Startposition (Weltkoordinaten)
    LONG    lFirstPosX;             //           x-Startposition (Weltkoordinaten)
    HWND    hwndVScroll;
    HWND    hwndHScroll;
    } WINDOWDATA, *PWINDOWDATA;

/* Modulinterne Funktionen */
MRESULT EXPENTRY RxOutWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT RxCreate (HWND hwnd);
MRESULT RxPaint (HWND hwnd);
MRESULT RxSize (HWND hwnd);
MRESULT RxVScroll (HWND hwnd, LONG lScTyp, LONG lTrack);
MRESULT RxHScroll (HWND hwnd, LONG lScTyp, LONG lTrack);
MRESULT RxPresParamChanged (HWND hwnd, LONG ppTyp);
MRESULT RxClose (HWND hwnd);

void _Optlink thOutputWindow (void *pArg)
    {
    HMQ         hmq;
    QMSG        qmsg;
    HWND        hwndFrame;
    ULONG       fcf = FCF_TITLEBAR   | FCF_SIZEBORDER | FCF_SYSMENU  | FCF_SHELLPOSITION |
                      FCF_VERTSCROLL | FCF_HORZSCROLL | FCF_TASKLIST | FCF_NOBYTEALIGN   |
                      FCF_MINMAX;

    hmq = WinCreateMsgQueue (hab, 0);

    hwndFrame = WinCreateStdWindow (HWND_DESKTOP,                   // Parent
                                    WS_VISIBLE,                     // Frame-window style
                                    &fcf,                           // Frame-creation flags
                                    CLASS_RXOUT,                    // Client class
                                    ((PTHREADARG)pArg)->pszTitle,   // Titlebar text
                                    WS_VISIBLE,                     // Client-window style
                                    NULLHANDLE,                     // Resource id
                                    IDD_RXOUT,                      // Frame-window id
                                    ((PTHREADARG)pArg)->pHwnd);     // Client-window handle
    /* Owner wird hwndLogin, dessen Owner HWND_OBJECT ist. Damit wird vermieden, da� */
    /* das Fenster auf WM_CLOSE an HWND_DESKTOP reagiert. Sp�ter ausbauen???         */
    WinSetOwner (hwndFrame, hwndLogin);

    DosPostEventSem (((PTHREADARG)pArg)->hev);

    /* Da PostTextToWindow m�glicherweise bereits wieder terminiert ist, */
    /* darf ab hier pArg nicht mehr verwendet werden!                    */

    while (WinGetMsg (hab, &qmsg, 0L, 0L, 0L))
         WinDispatchMsg (hab, &qmsg);

    WinDestroyMsgQueue(hmq);
    return;
    }

BOOL PostTextToWindow (PHWND pHwnd, PSZ szText, PSZ szTitle, LONG clrBackgnd)
    {
    PWINDOWDATA pWnd;
    APIRET      rc;
    ULONG       ulc;
    ULONG       ulcFirstLine;
    CLASSINFO   classInfo;
    THREADARG   thArg;

    /* Klasse bereits registriert? */
    if (WinQueryClassInfo (hab, CLASS_RXOUT, &classInfo) == FALSE)
        WinRegisterClass (hab, CLASS_RXOUT, RxOutWndProc, CS_SAVEBITS, sizeof (PWINDOWDATA));

    if (*pHwnd == NULLHANDLE)
        {
        thArg.pszTitle = szTitle;
        thArg.pHwnd    = pHwnd;
        DosCreateEventSem (NULL, &thArg.hev, 0, FALSE);

        if (_beginthread (thOutputWindow, NULL, STACKSIZE, &thArg) == -1)
            rc = ERROR_INTERRUPT;
        else
            {
            /* Warten, bis Fenster angelegt ist */
            rc = DosWaitEventSem (thArg.hev, SEM_INDEFINITE_WAIT);
            }

        DosCloseEventSem (thArg.hev);
        if (rc != NO_ERROR)
            return FALSE;                               // Fenster wurde nicht erzeugt!
        }

    /* Text anf�gen */
    pWnd = WinQueryWindowPtr (*pHwnd, QWP_WNDDATA);
    if (pWnd)
        {
        pWnd->clrBackgnd = clrBackgnd;
        ulc = strlen (szText);
        while (pWnd->pszNextText - pWnd->pszText + ulc > CCHMAXTEXT && pWnd->lcLines)
            {
            ulcFirstLine = strlen (pWnd->pszText);
            memmove (pWnd->pszText,
                     pWnd->pszText + ulcFirstLine + 1,
                     CCHMAXTEXT - ulcFirstLine - 1);
            pWnd->pszNextText -= ulcFirstLine + 1;
            pWnd->lcLines--;
            }
        strcpy (pWnd->pszNextText, szText);
        pWnd->pszNextText += ulc + 1;
        pWnd->lcLines++;

        /* Text ausgeben */
        WinPostMsg (*pHwnd, WM_REFRESH, MPVOID, MPVOID);
        }

    return TRUE;
    }

VOID SetScrollbars (PWINDOWDATA pWnd)
    {
    /* vertikalen Scrollbar setzen */
    WinSendMsg (pWnd->hwndVScroll, SBM_SETSCROLLBAR,
                MPFROMSHORT (pWnd->lFirstPosY),
                MPFROM2SHORT (0, pWnd->lTextHeight - pWnd->cyClient));
    WinSendMsg (pWnd->hwndVScroll, SBM_SETTHUMBSIZE,
                MPFROM2SHORT (pWnd->cyClient, pWnd->lTextHeight),
                MPVOID);

    /* horizontalen Scrollbar setzen */
    WinSendMsg (pWnd->hwndHScroll, SBM_SETSCROLLBAR,
                MPFROMSHORT (pWnd->lFirstPosX),
                MPFROM2SHORT (0, pWnd->lMaxLen - pWnd->cxClient));
    WinSendMsg (pWnd->hwndHScroll, SBM_SETTHUMBSIZE,
                MPFROM2SHORT (pWnd->cxClient, pWnd->lMaxLen),
                MPVOID);
    return;
    }

MRESULT EXPENTRY RxOutWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    switch (msg)
        {
        case WM_CREATE:
            return RxCreate (hwnd);

        case WM_SIZE:
            return RxSize (hwnd);

        case WM_REFRESH:
            WinInvalidateRect (hwnd, NULL, FALSE);
            return 0;

        case WM_PRESPARAMCHANGED:
            return RxPresParamChanged (hwnd, LONGFROMMP (mp1));

        case WM_VSCROLL:
            return RxVScroll (hwnd, SHORT2FROMMP (mp2), SHORT1FROMMP (mp2));

        case WM_HSCROLL:
            return RxHScroll (hwnd, SHORT2FROMMP (mp2), SHORT1FROMMP (mp2));

        case WM_PAINT:
            return RxPaint (hwnd);

        case WM_CLOSE:
            return RxClose (hwnd);
        }

    return WinDefWindowProc (hwnd, msg, mp1, mp2);
    }

MRESULT RxCreate (HWND hwnd)
    {
    PWINDOWDATA pWnd;
    FONTMETRICS fm;
    HPS         hps;
    SWP         swp;

    hps = WinGetPS (hwnd);
    GpiQueryFontMetrics (hps, sizeof fm, &fm);
    WinReleasePS (hps);

    pWnd = AllocMem (sizeof (WINDOWDATA));
    pWnd->clrBackgnd     = CLR_BACKGROUND;
    pWnd->clrForegnd     = CLR_DEFAULT;
    pWnd->lcLines        = 0;
    pWnd->lTextHeight    = 0;
    pWnd->lTextHeightOld = 0;
    pWnd->lMaxLen        = 0;
    pWnd->lMaxLenOld     = 0;
    pWnd->lFirstPosY     = 0;
    pWnd->pszText        = AllocMem (CCHMAXTEXT);
    pWnd->pszNextText    = pWnd->pszText;
    pWnd->cxChar         = fm.lAveCharWidth;
    pWnd->cxCaps         = fm.lEmInc;
    pWnd->cyChar         = fm.lMaxBaselineExt;
    pWnd->cyDesc         = fm.lMaxDescender;
    pWnd->hwndHScroll    = WinWindowFromID (WinQueryWindow (hwnd, QW_PARENT),
                                            FID_HORZSCROLL);
    pWnd->hwndVScroll    = WinWindowFromID (WinQueryWindow (hwnd, QW_PARENT),
                                            FID_VERTSCROLL);

    WinSetWindowPtr (hwnd, QWP_WNDDATA, pWnd);


    WinQueryWindowPos (hwnd, &swp);
    WinSetWindowPos (hwnd, NULLHANDLE,
                     0, 0,
                     swp.cx, swp.cy,
                     SWP_SIZE);
    return FALSE;
    }

MRESULT RxSize (HWND hwnd)
    {
    PWINDOWDATA pWnd;
    RECTL       rcl;
    BOOL        bChanged = FALSE;

    pWnd = WinQueryWindowPtr (hwnd, QWP_WNDDATA);

    WinQueryWindowRect (hwnd, &rcl);
    pWnd->cxClient = rcl.xRight;
    pWnd->cyClient = rcl.yTop;

    /* Update vertikaler Scrollbar */
    if (pWnd->lTextHeight - pWnd->lFirstPosY < pWnd->cyClient)
        {
        pWnd->lFirstPosY = pWnd->lTextHeight - pWnd->cyClient;
        if (pWnd->lFirstPosY < 0)
            pWnd->lFirstPosY = 0;
        bChanged = TRUE;
        }

    /* Update horizontaler Scrollbar */
    if (pWnd->lMaxLen - pWnd->lFirstPosX < pWnd->cxClient)
        {
        pWnd->lFirstPosX = pWnd->lMaxLen - pWnd->cxClient;
        if (pWnd->lFirstPosX < 0)
            pWnd->lFirstPosX = 0;
        bChanged = TRUE;
        }
    SetScrollbars (pWnd);
    if (bChanged)
        WinInvalidateRect (hwnd, NULL, FALSE);
    return 0;
    }

MRESULT RxPresParamChanged (HWND hwnd, LONG ppTyp)
    {
    PWINDOWDATA pWnd;
    FONTMETRICS fm;
    HPS         hps;

    if (ppTyp == PP_FONTNAMESIZE)
        {
        pWnd = WinQueryWindowPtr (hwnd, QWP_WNDDATA);
        hps  = WinGetPS (hwnd);
        GpiQueryFontMetrics (hps, sizeof fm, &fm);
        pWnd->cxChar = fm.lAveCharWidth;
        pWnd->cxCaps = fm.lEmInc;
        pWnd->cyChar = fm.lMaxBaselineExt;
        pWnd->cyDesc = fm.lMaxDescender;
        WinReleasePS (hps);
        }
    WinInvalidateRect (hwnd, NULL, FALSE);
    return 0;
    }

MRESULT RxVScroll (HWND hwnd, LONG lScTyp, LONG lTrack)
    {
    PWINDOWDATA pWnd = WinQueryWindowPtr (hwnd, QWP_WNDDATA);

    switch (lScTyp)
        {
        case SB_LINEUP:
            pWnd->lFirstPosY -= pWnd->cyChar;
            break;

        case SB_LINEDOWN:
            pWnd->lFirstPosY += pWnd->cyChar;
            break;

        case SB_PAGEUP:
            pWnd->lFirstPosY -= pWnd->cyClient;
            break;

        case SB_PAGEDOWN:
            pWnd->lFirstPosY += pWnd->cyClient;
            break;

        case SB_SLIDERTRACK:
            pWnd->lFirstPosY = lTrack;
            break;
        }

    if (pWnd->lFirstPosY < 0)
        pWnd->lFirstPosY = 0;
    if (pWnd->lFirstPosY > pWnd->lTextHeight - pWnd->cyClient)
        pWnd->lFirstPosY = pWnd->lTextHeight - pWnd->cyClient;

    WinSendMsg (pWnd->hwndVScroll, SBM_SETPOS,
                MPFROMSHORT (pWnd->lFirstPosY), MPVOID);

    WinInvalidateRect (hwnd, NULL, FALSE);
    return 0;
    }

MRESULT RxHScroll (HWND hwnd, LONG lScTyp, LONG lTrack)
    {
    PWINDOWDATA pWnd = WinQueryWindowPtr (hwnd, QWP_WNDDATA);

    switch (lScTyp)
        {
        case SB_LINEUP:
            pWnd->lFirstPosX -= pWnd->cxChar;
            break;

        case SB_LINEDOWN:
            pWnd->lFirstPosX += pWnd->cxChar;
            break;

        case SB_PAGEUP:
            pWnd->lFirstPosX -= pWnd->cxClient;
            break;

        case SB_PAGEDOWN:
            pWnd->lFirstPosX += pWnd->cxClient;
            break;

        case SB_SLIDERTRACK:
            pWnd->lFirstPosX = lTrack;
            break;
        }

    if (pWnd->lFirstPosX < 0)
        pWnd->lFirstPosX = 0;
    if (pWnd->lFirstPosX > pWnd->lMaxLen - pWnd->cxClient)
        pWnd->lFirstPosX = pWnd->lMaxLen - pWnd->cxClient;

    WinSendMsg (pWnd->hwndHScroll, SBM_SETPOS,
                MPFROMSHORT (pWnd->lFirstPosX), MPVOID);

    WinInvalidateRect (hwnd, NULL, FALSE);
    return 0;
    }

MRESULT RxPaint (HWND hwnd)
    {
    PWINDOWDATA pWnd;
    PCHAR       pszNextLine, psz;
    POINTL      ptl, ptlEnd;
    LONG        cLine;
    LONG        i, ulc;
    RECTL       rcl;
    HPS         hps;

    pWnd = WinQueryWindowPtr (hwnd, QWP_WNDDATA);
    hps  = WinBeginPaint (hwnd, NULLHANDLE, &rcl);

    GpiCreateLogColorTable (hps, 0, LCOLF_RGB, 0, 0, NULL);

    WinFillRect (hps, &rcl, pWnd->clrBackgnd);
    GpiSetColor (hps, pWnd->clrForegnd);

    /* Schriftartgr��en bestimmen */
    ptl.x  = pWnd->cxCaps - pWnd->lFirstPosX;

    /* Textausgabe */
    pszNextLine = pWnd->pszText;
    pWnd->lMaxLen = 0;
    for (cLine=i=0; i<pWnd->lcLines; i++)
        {
        ulc = strlen (pszNextLine);
        psz = pszNextLine + ulc + 1;
        ptl.y = pWnd->cyClient + pWnd->lFirstPosY - ++cLine*pWnd->cyChar + pWnd->cyDesc;
        GpiCharStringAt (hps, &ptl, ulc, pszNextLine);
        GpiQueryCurrentPosition (hps, &ptlEnd);
        pWnd->lMaxLen = max (pWnd->lMaxLen, ptlEnd.x - ptl.x);
        pszNextLine   = psz;
        }
    pWnd->lMaxLen    += pWnd->cxCaps;
    pWnd->lTextHeight = cLine*pWnd->cyChar + pWnd->cyDesc;

    if (pWnd->lTextHeightOld != pWnd->lTextHeight ||
        pWnd->lMaxLenOld != pWnd->lMaxLen)
        {
        SetScrollbars (pWnd);
        pWnd->lTextHeightOld = pWnd->lTextHeight;
        pWnd->lMaxLenOld     = pWnd->lMaxLen;
        }


    WinEndPaint (hps);
    return 0;
    }

MRESULT RxClose (HWND hwnd)
    {
    PWINDOWDATA pWnd;

    /* Allokierten Speicher wieder freigeben */
    pWnd = WinQueryWindowPtr (hwnd, QWP_WNDDATA);
    DebugE (D_SAM, "FreeMem", "RxPaint1");
    FreeMem ((PPVOID)&pWnd->pszText);
    DebugE (D_SAM, "FreeMem", "RxPaint2");
    WinSetWindowPtr (hwnd, QWP_WNDDATA, NULL);
    FreeMem ((PPVOID)&pWnd);
    DebugE (D_SAM, "FreeMem", "RxPaint3");

    WinPostMsg (hwnd, WM_QUIT, MPVOID, MPVOID);
    return 0;
    }
