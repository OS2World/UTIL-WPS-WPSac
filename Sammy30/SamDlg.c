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
#include <stdlib.h>
#include <SamMem.h>
#include <time.h>

#include "samglob.h"

#include "Authorize.h"
#include "bitmap.h"
#include "ChangePwd.h"
#include "CreateObj.h"
#include "debug.h"
#include "Environ.h"
#include "LANDrive.h"
#include "LogInOut.h"
#include "magpro2.h"
#include "MagSupp.h"
#include "optsdlg.h"
#include "prepare.h"
#include "profile.h"
#include "SacAlias.h"
#include "SamData.h"
#include "SamDlg.h"
#include "Sammy.h"
#include "SammyDlg.h"
#include "SammyPan.h"
#include "SCenter.h"

#define THREADSTACK  0x10000

extern CHAR  szUserPath[CCHMAXPATH];

static HPS hPSBitmap   = NULLHANDLE;
static HBITMAP hBitmap = NULLHANDLE;

VOID SetPos (HWND hwnd)
    {
    ULONG cxScreen, cyScreen, cx, cy;
    HWND  hwndNext;
    HENUM henum;
    SWP   swp;

    /* Bildschirmgrî·e bestimmen */
    cxScreen = WinQuerySysValue (HWND_DESKTOP, SV_CXSCREEN);
    cyScreen = WinQuerySysValue (HWND_DESKTOP, SV_CYSCREEN);

    /* Dialogboxgrî·e anpassen */
    WinQueryWindowPos (hwnd, &swp);
    cx = (cxScreen - swp.cx) / 2;
    cy = (cyScreen - swp.cy) / 2;
    WinSetWindowPos (hwnd, NULLHANDLE, 0, 0, cxScreen, cyScreen, SWP_SIZE | SWP_MOVE);

    /* Positionen setzen */
    henum = WinBeginEnumWindows (hwnd);
    while ((hwndNext = WinGetNextWindow (henum)) != NULLHANDLE)
        {
        WinQueryWindowPos (hwndNext, &swp);
        WinSetWindowPos (hwndNext, NULLHANDLE, swp.x + cx, swp.y + cy, 0, 0, SWP_MOVE);
        }
    WinEndEnumWindows (henum);

    return;
    }

VOID SetColors (HWND hwnd)
    {
    ULONG ulColor;
    HWND  hwndNext;
    HENUM henum;
    CHAR  szClassName[16];

    /* Positionen / Hintergrundfarben setzen */

    ulColor = QueryLocINIInt (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGCLR, CLR_DARKBLUE);
    henum = WinBeginEnumWindows (hwnd);
    while ((hwndNext = WinGetNextWindow (henum)) != NULLHANDLE)
        {
        WinQueryClassName (hwndNext, 16, szClassName);
        if (*szClassName == '#')
            if (atoi (szClassName+1) == (LONG)((SHORT)WC_STATIC))
                WinSetPresParam (hwndNext, PP_BACKGROUNDCOLORINDEX, sizeof (ULONG), &ulColor);
        }
    WinEndEnumWindows (henum);

    return;
    }

VOID SetBitmap (HWND hwnd)
    {
    CHAR  szBitmapName[CCHMAXPATH];
    ULONG ulRet;
    HDC   hDC;
    CAPS  caps = {640, 480, 16};;

    if (hPSBitmap)
        {
        GpiDeleteBitmap (hPSBitmap);
        WinReleasePS (hPSBitmap);
        }

    hDC = WinQueryWindowDC (hwnd);
    if (hDC)
        {
        DevQueryCaps (hDC, CAPS_WIDTH,  1, (PLONG)&caps.cx);
        DevQueryCaps (hDC, CAPS_HEIGHT, 1, (PLONG)&caps.cy);
        DevQueryCaps (hDC, CAPS_COLORS, 1, (PLONG)&caps.ulMaxColors);
        }

    QueryLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGBMP, szBitmapName, CCHMAXPATH);
    if (strlen (szBitmapName))
        {
        hPSBitmap = WinGetPS (hwnd);
        ulRet = createBitmap (hPSBitmap, szBitmapName, &hBitmap, &caps, NULL);

        if (ulRet != BMPERR_OK)
            {
            GpiDeleteBitmap (hBitmap);
            WinReleasePS (hPSBitmap);
            hBitmap   = NULLHANDLE;
            hPSBitmap = NULLHANDLE;
            }
        }

    return;
    }

VOID SetComboBox (HWND hwnd)
    {
    CHAR szUser[CCHMAXUSERNAME];
    LONG lc;

    WinSendDlgItemMsg (hwnd, DID_LOGIN_USER, LM_DELETEALL, MPVOID, MPVOID);

    for (lc = 0; lc <= 0x7FFF; lc++)
        {
        QueryAlias (lc, szUser);
        if (szUser[0] == '\0')
            break;
        WinSendDlgItemMsg (hwnd,
                           DID_LOGIN_USER,
                           LM_INSERTITEM,
                           MPFROMSHORT (LIT_SORTASCENDING),
                           MPFROMP (szUser));
        }

    return;
    }

/*******************************************************************\
    SamFocusChange: Selektiert in Eingabefeldern den gesamten Text,
                    wenn diese den Fokus erhalten
    Eingang: hwnd:       Presentation space
             hwndFocus:  Fokus-Fenster
             usSetFocus: TRUE = hwndFocus erhÑlt Fokus
\*******************************************************************/
VOID SamFocusChange (HWND hwndFocus, USHORT usSetFocus)
    {
    USHORT usId;

    if (usSetFocus == TRUE)
        {
        usId = WinQueryWindowUShort (hwndFocus, QWS_ID);
        if (usId == DID_LOGIN_USER || usId == DID_LOGIN_PASSWORD)
            WinSendMsg (hwndFocus, EM_SETSEL, MPFROM2SHORT (0, 0xFFFF), MPVOID);
        }
    return;
    }

/*******************************************************************\
    SamDrawString: Ausgabe eines Strings in das Rechteck prcl.
                   Die Zeichen werden so verzerrt, da· der gesamte
                   String in das angegebene Rechteck pa·t.
    Eingang: hps:       Presentation space
             ulFGColor: Vordergrundfarbe
             pszString: String
             prcl:      Zeiger auf RECTL-Struktur
\*******************************************************************/
VOID SetFacenameSize (HPS hps, FIXED fSize, PCHAR pszFacename)
    {
    SIZEF       sizef;
    FATTRS      fattr;

    GpiSetCharMode (hps, CM_MODE3);

    /* Schriftart einstellen */
    fattr.usRecordLength  = sizeof (FATTRS);
    fattr.fsSelection     = 0;
    fattr.lMatch          = 0;
    strcpy (fattr.szFacename, pszFacename);
    fattr.idRegistry      = 0;
    fattr.usCodePage      = 0;
    fattr.lMaxBaselineExt = 0;
    fattr.lAveCharWidth   = 0;
    fattr.fsType          = 0;
    fattr.fsFontUse       = 0;
    GpiCreateLogFont (hps, NULL, 0, &fattr);

    /* Wahl der Zeichenbox */
    sizef.cy = sizef.cx = fSize;
    GpiSetCharBox (hps, &sizef);

    return;
    }

VOID SamPaint (HWND hwnd)
    {
    ULONG ulColor;
    LONG  cx, cy;
    LONG  x, y;
    HPS   hpsPaint;
    RECTL rcl;
    RECTL rclText;
    HWND  hwndCtrl;
    SWP   sizeCtrl;
    CHAR  szText[32];

    ulColor = QueryLocINIInt (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGCLR, CLR_DARKBLUE);

    hpsPaint = WinBeginPaint (hwnd, hPSBitmap, NULL);

    WinQueryWindowRect (hwnd, &rcl);
    WinFillRect (hpsPaint, &rcl, ulColor);

    if (hBitmap != NULLHANDLE)
        {
        double dx, dy;
        POINTL aptl[4];
        BITMAPINFOHEADER2 bmpInfo;

        bmpInfo.cbFix = sizeof (BITMAPINFOHEADER2);
        GpiQueryBitmapInfoHeader (hBitmap, &bmpInfo);

        cx = min (bmpInfo.cx, rcl.xRight - rcl.xLeft);
        cy = min (bmpInfo.cy, rcl.yTop - rcl.yBottom);
        dx = (double)bmpInfo.cx/cx;
        dy = (double)bmpInfo.cy/cy;

        if (dx > 1.0 || dy > 1.0)
            {
            if (dx > dy)
                cy = bmpInfo.cy * (rcl.xRight - rcl.xLeft) / bmpInfo.cx;
            else
                cx = bmpInfo.cx * (rcl.yTop - rcl.yBottom) / bmpInfo.cy;
            }

        x  = (rcl.xRight - rcl.xLeft - cx) / 2;
        y  = (rcl.yTop - rcl.yBottom - cy) / 2;

        aptl[0].x = x;                  // Tx1 inclusive
        aptl[0].y = y;                  // Ty1
        aptl[1].x = cx + x;             // Tx2
        aptl[1].y = cy + y;             // Ty2
        aptl[2].x = 0;                  // Sx1 non inclusive
        aptl[2].y = 0;                  // Sy1
        aptl[3].x = bmpInfo.cx;         // Sx2
        aptl[3].y = bmpInfo.cy;         // Sy2

        GpiWCBitBlt (hpsPaint, hBitmap, 4, aptl, ROP_SRCCOPY, BBO_IGNORE);
        }

    SetFacenameSize (hpsPaint, MAKEFIXED (50, 0), "Helvetica");

    /*** Position des Titels oberhalb erster Zeile der Dialogbox berechnen ***/
    // 1. Oberkante bestimmen
    hwndCtrl = WinWindowFromID (hwnd, DID_LOGIN_TEXT_USER);
    WinQueryWindowPos (hwndCtrl, &sizeCtrl);
    y = sizeCtrl.y + sizeCtrl.cy;

    // 2. Position des Titels bestimmen
    y = (y + rcl.yTop)/2;

    // 3. Text zur Bestimmung der Schrifthîhe ausgeben
    rclText.xLeft   = rcl.xLeft;
    rclText.xRight  = rcl.xRight;
    rclText.yBottom = 0;
    rclText.yTop    = 500;
    WinLoadString (hab, NULLHANDLE, IDS_LOGONTITLE, 32, szText);
    WinDrawText (hpsPaint, -1, szText, &rclText, CLR_YELLOW, CLR_BACKGROUND,
                 DT_CENTER | DT_BOTTOM | DT_QUERYEXTENT);

    // 4. Textposition bestimmen und ausgeben
    cy = rclText.yTop - rclText.yBottom;
    y -= cy/2;
    rclText.xLeft   = rcl.xLeft;
    rclText.xRight  = rcl.xRight;
    rclText.yBottom = y;
    rclText.yTop    = y + cy;
    WinDrawText (hpsPaint, -1, szText, &rclText, CLR_YELLOW, CLR_BACKGROUND,
                 DT_CENTER | DT_BOTTOM);

#ifdef DEMO
    rclText.xLeft   = rcl.xLeft;
    rclText.xRight  = rcl.xRight;
    rclText.yBottom = y - cy;
    rclText.yTop    = y;
    WinLoadString (hab, NULLHANDLE, IDS_DEMOTITLE, 32, szText);
    WinDrawText (hpsPaint, -1, szText, &rclText, CLR_RED, CLR_BACKGROUND,
                 DT_CENTER | DT_BOTTOM);
#endif /* DEMO */

    GpiSetMix (hpsPaint, FM_DEFAULT);

    SetFacenameSize (hpsPaint, MAKEFIXED (10, 0), "System Proportional");
    hwndCtrl = WinWindowFromID (hwnd, DID_LOGIN_TEXT_PASSWORD);
    WinQueryWindowPos (hwndCtrl, &sizeCtrl);
    WinQueryWindowText (hwndCtrl, 32, szText);
    rclText.xLeft  = sizeCtrl.x;
    rclText.xRight = sizeCtrl.x + sizeCtrl.cx;
    rclText.yBottom = sizeCtrl.y;
    rclText.yTop    = sizeCtrl.y + sizeCtrl.cy;
    WinDrawText (hpsPaint, -1, szText, &rclText, CLR_WHITE, CLR_BACKGROUND,
                 DT_LEFT | DT_BOTTOM);

    hwndCtrl = WinWindowFromID (hwnd, DID_LOGIN_TEXT_USER);
    WinQueryWindowPos (hwndCtrl, &sizeCtrl);
    WinQueryWindowText (hwndCtrl, 32, szText);
    rclText.xLeft  = sizeCtrl.x;
    rclText.xRight = sizeCtrl.x + sizeCtrl.cx;
    rclText.yBottom = sizeCtrl.y;
    rclText.yTop    = sizeCtrl.y + sizeCtrl.cy;
    WinDrawText (hpsPaint, -1, szText, &rclText, CLR_WHITE, CLR_BACKGROUND,
                 DT_LEFT | DT_BOTTOM);

    WinEndPaint (hpsPaint);

    /* Setzen des Fokus */
    WinPostMsg (hwnd, WM_PAINTCOMPLETE, 0, 0);

    return;
    }

/*******************************************************************
   SamPaintComplete: Setzt das aktive Control
   Eingang: hwnd: Window-Handle des Logon-Fensters
 *******************************************************************/
VOID SamPaintComplete (HWND hwnd)
    {
    if (WinQueryDlgItemTextLength (hwnd, DID_LOGIN_USER))
        WinSetFocus (HWND_DESKTOP, WinWindowFromID (hwnd, DID_LOGIN_PASSWORD));

    return;
    }

/*******************************************************************
   SamInit: Initialisieren des Logon-Fensters:
   1.) Eingabefelder auf entsprechende LÑnge setzen
   2.) Parent auf HWND_OBJECT setzen
   3.) Pa·wortmodus prÅfen
   Eingang: hwnd: Window-Handle des Logon-Fensters
 *******************************************************************/
VOID SamInit (HWND hwnd)
    {
    ULONG ulc;
    CHAR  szUsername[CCHMAXUSERNAME];

    WinOpenWindowDC (hwnd);

    SetPos (hwnd);
    SetColors (hwnd);
    SetComboBox (hwnd);
    SetBitmap (hwnd);
    WinInvalidateRect (hwnd, NULL, TRUE);

    /* Entry-Felder initialisieren */
    WinSendDlgItemMsg (hwnd, DID_LOGIN_USER,     EM_SETTEXTLIMIT, MPFROMLONG (CCHMAXUSERNAME), MPVOID);
    WinSendDlgItemMsg (hwnd, DID_LOGIN_PASSWORD, EM_SETTEXTLIMIT, MPFROMLONG (CCHMAXUSERNAME), MPVOID);
    WinSetParent (hwnd, HWND_OBJECT, TRUE);

    flMode = 0;
    ulc    = sizeof (flMode);
    QueryLocINIData   (MAGPRO_SYSTEM_APP, MAGPRO_FLAG_KEY,     &flMode,    &ulc);
    QueryLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_LASTUSER, szUsername, CCHMAXUSERNAME);

    WinSetDlgItemText (hwnd, DID_LOGIN_USER, szUsername);
    return;
    }

/*******************************************************************
   SamLogoff: Der Benutzer hat sich von der ArbeitsoberflÑche
   abgemeldet. Aktionen:
   1.) Logoff-Skripte starten
   2.) aktuelle Shell schlie·en
   3.) WM_LOGON posten
   Eingang: hwnd: Window-Handle des Logon-Fensters
 *******************************************************************/
VOID SamLogoff (HWND hwnd)
    {
    HWND hwndMsg;
    ULONG idMsgString;

    /* Zuerst prÅfen, ob WPSac Åberhaupt installiert ist */
    /* ??? */
    WinAlarm (HWND_DESKTOP, WA_NOTE);

    /* aktuelle Shell schlie·en */
    pShareMem->szEnvironment[0] = '\0';
    pShareMem->szEnvironment[1] = '\0';
    pShareMem->szSystemIni[0]   = '\0';
    pShareMem->szUserIni[0]     = '\0';

    /* Ausloggen */
    idMsgString = IDS_USERLOGOFF;
    hwndMsg = WinLoadDlg (HWND_DESKTOP, HWND_DESKTOP, TimedMsgProc, NULLHANDLE, IDD_TIMEDMSG, &idMsgString);
    SingleSignon (QueryLocINIHandle (), strucUserData.szUser, FALSE);
    DosSleep (2000);                    // Message mindestens 2s anzeigen
    WinDestroyWindow (hwndMsg);

    strucUserData.ulFlag = ALIAS_USER;  // ???
    AliasFind (&strucUserData);         // ???
// ???            strcpy (szDesktop, strucUserData.szDesktop);
//???!!!--->nanu? woher kommt szDesktopPath            GetUserPath (strucUserData.szDesktop, szDesktopPath, CCHMAXPATH);
    PostSignonEnded (hwnd, WM_SAMMY_TERMINATE);

    /* SmartCenter sichern */
    SaveSCenter (szUserPath);

// ???      ChangeWPS (pShareMem->pszUserIni, pShareMem->pszSystemIni); Notwendig???
    strucUserData.szID[0]       = '\0';
    strucUserData.szPassword[0] = '\0';
    WinSetDlgItemText (hwnd, DID_LOGIN_PASSWORD, "");
    WinPostMsg (hwnd, WM_LOGON, MPVOID, MPVOID);

    return;
    }

/*******************************************************************
   SamLogon: Zeigt den Logon-Dialog an und startet die
   MagnetkartenunterstÅtzung, falls mîglich.
   Ein Einlogvorgang wird angezeigt Åber die Messages:
   Tastatur: SamCommand
   ID-Karte: SamIdCard
   Eingang: hwnd:       Window-Handle des Logon-Fensters
 *******************************************************************/
VOID SamLogon (HWND hwnd)
    {
    MAGNET magnet;

    WinSetParent (hwnd, HWND_DESKTOP, TRUE);
    WinSetFocus (HWND_DESKTOP, WinWindowFromID (hwnd, DID_LOGIN_USER));

    memset (&strucUserData, 0, sizeof (ALIASDATA));

    if (MCOpen ())
        {
        if (flMode & FLG_SYS_CARD_ONLY)
            WinSetDlgItemText (hwnd, DID_LOGIN_TEXTIDCARD, "(oder ID-Karte benÅtzen)");
        else
            WinSetDlgItemText (hwnd, DID_LOGIN_TEXTIDCARD, "(ID-Karte benÅtzen)");
        }
    else
        WinSetDlgItemText (hwnd, DID_LOGIN_TEXTIDCARD, "");

    /* Die Fehlerbehandlung fÅr MCStart erfolgt in der Routine! */
    magnet.usDID          = DID_LOGIN_PASSWORD;
    magnet.hwnd           = hwnd;
    magnet.ulNotification = WM_IDCARD;
    magnet.pszReadData    = strucUserData.szID;
    MCStart (&magnet);

    return;
    }

VOID SamLoginComplete (VOID)
    {
    if (HandleTodoLists () == FALSE)
        MessageBox (HWND_DESKTOP, IDS_ERRWRITELANPROFILE, MBTITLE_ERROR, MB_OK | MB_ERROR);

    return;
    }

VOID SamAppTerminateNotify (HAPP happ)
    {
    if (happ == happShell)
        {
        InitAuthorize (&strucUserData);

        happShell = NULLHANDLE;
        if (bStartShellEnable == TRUE)
            {
            DebugE (D_SAM, "SamAppTerminateNotify", "restart shell");
            happShell = StartShell ();
            }
        }

    return;
    }

/*******************************************************************
   SamIdCard: Es wurde eine Karte durch den Leser gezogen. Es mu·
   der Text aus den Eingabefeldern gelesen werden. Der Inhalt der
   ID-Karte steht im globalen Puffer strucUserData.szID.
   Einleiten des Einlogvorganges: WM_START_USER
   Eingang: hwnd:       Window-Handle des Logon-Fensters
 *******************************************************************/
VOID SamIdCard (HWND hwnd)
    {
    WinQueryWindowText (WinWindowFromID (hwnd, DID_LOGIN_USER),
                        CCHMAXUSERNAME,
                        strucUserData.szUser);

    WinQueryWindowText (WinWindowFromID (hwnd, DID_LOGIN_PASSWORD),
                        CCHMAXPASSWORD,
                        strucUserData.szPassword);

    if (strucUserData.szID[0] == '\0')
        return;

    MCAbort ();
    MCClose ();

    if (flMode & FLG_SYS_CARD_ONLY)
        {
        strucUserData.ulFlag = ALIAS_ID | ALIAS_WPSAC;
        WinPostMsg(hwnd, WM_START_USER, 0, 0);
        }
    else
        {
        strucUserData.ulFlag |= ALIAS_ID | ALIAS_WPSAC;
        if ((strucUserData.ulFlag & (ALIAS_ID | ALIAS_USER | ALIAS_PASSWORD)) ==
            ALIAS_ID | ALIAS_USER | ALIAS_PASSWORD)
            WinPostMsg (hwnd, WM_START_USER, 0, 0);
        }

    return;
    }

/*******************************************************************
   SamCommand: Es ist eine WM_COMMAND-Message aufgetreten. Mîgliche
   Ursachen:
   DID_OK: Es wurde der OK-Button gedrÅckt. Auslesen der
     Eingabefelder sowie mîglicherweise der ID-Karteninformation
     Einleiten des Einlogvorganges: WM_START_USER
   DID_OPION: Es wurde der Option-Button angewÑhlt
   IDM_*: Es wurde ein MenÅpunkt ausgewÑhlt
   Eingang: hwnd: Window-Handle des Logon-Fensters
            mp1:  Parameter 1 von WM_COMMAND
            mp2:  Parameter 2 von WM_COMMAND
 *******************************************************************/
VOID SamCommand (HWND hwnd, MPARAM mp1, MPARAM mp2)
    {
    HWND                hwndMenu;
    SWP                 swp;

    if (SHORT1FROMMP (mp2) == CMDSRC_PUSHBUTTON)
        {
        switch (SHORT1FROMMP (mp1))
            {
            case DID_OK:
                WinQueryWindowText (WinWindowFromID (hwnd, DID_LOGIN_USER),
                                    CCHMAXUSERNAME,
                                    strucUserData.szUser);

                WinQueryWindowText (WinWindowFromID (hwnd, DID_LOGIN_PASSWORD),
                                    CCHMAXPASSWORD,
                                    strucUserData.szPassword);

                if (flMode & FLG_SYS_CARD_ONLY)
                    {
                    if (!(strucUserData.ulFlag & ALIAS_ID))
                        strucUserData.ulFlag = ALIAS_USER | ALIAS_PASSWORD | ALIAS_WPSAC;
                    }
                else
                    strucUserData.ulFlag = ALIAS_ID | ALIAS_USER | ALIAS_PASSWORD | ALIAS_WPSAC;
// ???            DosPostEventSem (hevLoginDlg);

                MCAbort ();
                MCClose ();
                WinPostMsg (hwnd, WM_START_USER, 0, 0);
                break;

            case DID_OPTION:
                WinQueryWindowPos (WinWindowFromID (hwnd, DID_OPTION), &swp);
                hwndMenu = WinLoadMenu (hwnd, NULLHANDLE, IDM_LOGIN);
                WinPopupMenu (hwnd, hwnd, hwndMenu,
                    swp.x + swp.cx/2,
                    swp.y + swp.cy/2,
                    NULLHANDLE, PU_HCONSTRAIN | PU_VCONSTRAIN | PU_KEYBOARD | PU_MOUSEBUTTON1);
                break;
            }
        }
    else if (SHORT1FROMMP (mp2) == CMDSRC_MENU)
        {
        switch (SHORT1FROMMP (mp1))
            {
            case IDM_SHUTDOWN:
                WinShutdownSystem (hab, hmq);
                break;

            case IDM_CHGPWD:
                WinDlgBox (HWND_DESKTOP, hwnd, ChgPwdDlgProc, NULLHANDLE, IDD_EDIT, NULL);
                break;

            case IDM_VIEW:
                if (OptionDialog (hwnd))
                    {
                    SetColors (hwnd);
                    SetBitmap (hwnd);
                    WinInvalidateRect (hwnd, NULL, TRUE);
                    }
                break;

            case IDM_ABOUT:
                WinDlgBox (HWND_DESKTOP, hwnd, WinDefDlgProc, NULLHANDLE, IDD_ABOUTBOX, NULL);
                break;
            }
        }

    return;
    }

BOOL SamHelp (VOID)
    {
    if (hwndHelp)
        {
        WinSendMsg (hwndHelp,
            HM_DISPLAY_HELP,
            MPFROM2SHORT (PANEL_LOGIN, 0),
            MPFROMSHORT (HM_RESOURCEID));
        return TRUE;
        }
    return FALSE;
    }

/*******************************************************************
   SamStartUser: neuen Benutzer starten. Vorgang:
   1.) LAN-Directory in das lokale Directory kopieren
   2.) Benutzer suchen und Pa·wort prÅfen
   3.) Single-Signon-Scripts starten
   4.) RC-Dateien vorbereiten
   5.) WPS umschalten (PrfReset)
   6.) neue Shell starten
   7.) neue Objekte erzeugen
   Eingang: hwnd:       Window-Handle des Logon-Fensters
 *******************************************************************/
BOOL SamStartUser (HWND hwnd)
    {
    BOOL        bRet;
    ULONG       ul, ulRet;
    ULONG       idText;
    ULONG       ulAge, ulcDays;
    PSZ         pszTempEnv;
    static CHAR szResourceFile[CCHMAXPATH];
    static CHAR szDesktopPath[CCHMAXPATH];
    static CHAR szResourceFilename[CCHMAXPATHCOMP];

#ifdef DEMO
    {
        HWND  hwndMsg;
        ULONG idMsgString;

        idMsgString = IDS_DEMOVERSION;
        hwndMsg = WinLoadDlg (HWND_DESKTOP, HWND_DESKTOP, TimedMsgProc, NULLHANDLE, IDD_TIMEDMSG, &idMsgString);
        SingleSignon (QueryLocINIHandle (), strucUserData.szUser, TRUE);
        DosSleep (10000);                    // Message mindestens 2s anzeigen
        WinDestroyWindow (hwndMsg);
    }
#endif /* DEMO */

    /* Prelogon */
    MountLANDirectory ();

    /* LAN-Directory auf lokales Verzeichnis umkopieren */
    ul = CopyLANDir (szPathLoc, szPathLAN);
    if (ul)
        WinPostMsg (hwnd, WM_MESSAGEBOX, MPFROMLONG (ul), MPVOID);

    /* Prelogoff */
    UnmountLANDirectory ();

    /* User in Alias-Liste suchen: strucUserData wird vervollstÑndigt */
    DebugE (D_SAM, "StartUser", "Alias-Liste durchsuchen");
    ulRet = AliasFind (&strucUserData);

    /* Fehlermeldung im Fehlerfall ausgeben */
    switch (ulRet)
        {
        case STAT_NOUSERFOUND:
        case STAT_USERNOTFOUND:
            idText = IDS_USERNOTFOUND;
            break;

        case STAT_WRONGPASSWD:
            idText = IDS_WRONGPASSWORD;
            break;
        default:
            idText = 0;
        }
    if (idText)
        {
        strucUserData.szPassword[0] = strucUserData.szID[0] = '\0';
        WinSetDlgItemText (hwndLogin, DID_LOGIN_PASSWORD, "");
        WinPostMsg (hwndLogin, WM_MESSAGEBOX, (MPARAM)idText, MPVOID);
        return FALSE;
        }

    /* Benutzernamen sichern */
    WriteLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_LASTUSER, strucUserData.szUser);

// abort ();

    /* Ist das Pa·wort noch gÅltig? */
    DebugE (D_SAM, "StartUser", "Pa·wort prÅfen: abgelaufen?");
    if (ulRet == STAT_PASSWORDOK)
        {
        if (IsPwdAgeSet (strucUserData.szUser))
            {
            ulcDays = GetPwdValidIntvl ();
            ulAge   = GetPwdAge (strucUserData.szUser);

            if (ulcDays > 0 && ulAge > ulcDays)
                {
                /* Pa·wort ist ausgelaufen */
                WinDlgBox (HWND_DESKTOP, hwnd, ChgPwdDlgProc, NULLHANDLE, IDD_EDIT, strucUserData.szUser);
                return FALSE;
                }
            }
        else
            {
            /* Pa·wortalter ist noch nicht gesetzt */
            SetPwdAge (strucUserData.szUser);
            }
        }

    /* Logon-Dialog unsichtbar machen */
    WinSetParent (hwnd, HWND_OBJECT, TRUE);

    /* Single Signon */
    DebugE (D_SAM, "StartUser", "REXX-Signon starten");
    bRet = QueryLocINISize (strucUserData.szUser, NULL, &ul);
    if (bRet && ul)
        {
        HWND  hwndMsg;
        ULONG idMsgString;

        idMsgString = IDS_USERLOGON;

        hwndMsg = WinLoadDlg (HWND_DESKTOP, HWND_DESKTOP, TimedMsgProc, NULLHANDLE, IDD_TIMEDMSG, &idMsgString);
        SingleSignon (QueryLocINIHandle (), strucUserData.szUser, TRUE);
        DosSleep (2000);                    // Message mindestens 2s anzeigen
        WinDestroyWindow (hwndMsg);
        PostSignonEnded (hwndLogin, WM_LOGIN_COMPLETE);
        }

    /* kein Desktop angegeben => Standard-Desktop starten */
    if (!(strucUserData.ulFlag & ALIAS_DESKTOP))
        strucUserData.szDesktop[0] = '\0';

    // ???:
    // bei WPSam mu· eigentlich nur einmal PrepareDT aufgerufen werden.
    // Vielleicht lÑ·t sich der Rest danach auch fÅr WPSam weiterverwenden.
    // Sonst: eigene Routine fÅr WPSam/WPSac!
    /* Desktop vorbereiten, falls WPSam-Desktop */
    DebugE (D_SAM, "StartUser", "Desktop vorbereiten WPSam, Versuch 1");
    bRet = PrepareDT (strucUserData.szDesktop, &ul);
    if (!bRet && ul == IDS_DESKTPNOTFOUND)
        {
        DebugE (D_SAM, "StartUser", "Desktop vorbereiten Std");
        bRet = CreateDTDir (&strucUserData, &ul);
        if (bRet)
            {
            /* Zweiter Versuch */
            DebugE (D_SAM, "StartUser", "Desktop vorbereiten WPSam, Versuch 2");
            bRet = PrepareDT (strucUserData.szDesktop, &ul);
            }
        }

    /* Environment lesen (nur WPSAC) */
    pszTempEnv = CreateEnvironment (strucUserData.szUser);
    if (pszTempEnv)
        {
        strncpy (pShareMem->szEnvironment, pszTempEnv, CCHSHARE_INITOS2 - sizeof (SHARE1));
        FreeMem ((PPVOID)&pszTempEnv);
        }

    /* Admin-Flag im Shared-Memory-Bereich setzen, falls Benutzer Administrator ist */
    if (strucUserData.bIsAdmin)
        pShareMem->ulUserFlag |= USER_FLAG_ADMIN;
    else
        pShareMem->ulUserFlag &= ~USER_FLAG_ADMIN;

    if (!bRet)
        {
        WinPostMsg (hwndLogin, WM_MESSAGEBOX, (MPARAM)ul, MPVOID);
        return FALSE;
        }

    /* SmartCenter restaurieren */
    RestoreSCenter (szUserPath);

    DebugE (D_SAM, "StartUser", "WPS umschalten");
    if (!ChangeWPS ())
        return FALSE;

    /* Im Shared-Memory Startup-Folder setzen */
    memset (&pShareMem->hStartupFldr, 0, NMAX_WPSTRTUP*sizeof (HOBJECT));

    /* Authorize-Funktion initialisieren */ // nur WPSac ???
    DebugE (D_SAM, "StartUser", "Authorize initialisieren");
    InitAuthorize (&strucUserData);

    /* Shell starten */
    bStartShellEnable = TRUE;
    happShell = StartShell ();

    /* Warten, bis Workplace-Shell initialisiert ist */
    DebugE (D_SAM, "StartUser", "Warten auf SOM...");
    for (ul=0; WinQueryObject ("<WP_DESKTOP>") == NULLHANDLE && ul<600; ul++)
        {
        DosSleep (100);
        }
    DebugULd (D_SAM, "StartUser", "Wartezeit in 100ms (max 50)", ul);

    /* neue Objekte erzeugen (nur, wenn hiniAlias existiert) */ // nur WPSac== hiniAlias(?) ???
    if (QueryLocINIHandle ())
        {
        strcpy (szDesktopPath, szUserPath);
        strcpy (szResourceFile, szUserPath);
        strcat (szResourceFile, "\\");
        szResourceFilename[0] = '\0';
        QueryLocINIString (MAGPRO_DESKTOP_APP,
                           strucUserData.szDesktop,
                           szResourceFilename,
                           CCHMAXPATHCOMP);
        strcat (szResourceFile, szResourceFilename);        // LÑnge prÅfen???
        if (szResourceFilename[0])
            if (IsNewResourceFile (szDesktopPath, szResourceFilename, szResourceFile))
                {
                DosSleep (10000);                           // hoffentlich geht das auch ohne ???
                CreateObjects (szResourceFile);
                DosDelete (szResourceFile);                 // Ea-Logik drehen: Das Schreiben des Timestamp in "IsNewResourceFile" mu·
                }                                           // hinter DosDelete verlegt werden
        }

    return TRUE;
    }

VOID SamShell (ULONG ulEvent)
    {
    switch (ulEvent)
        {
        case SHELL_CLOSE:
            bStartShellEnable = FALSE;
            if (!WinTerminateApp (happShell))
                if (WinGetLastError (hab) ==  PMERR_CANNOT_STOP)
                    break;

//          /* ArbeitsoberflÑche sichern */
//          if (*szDesktopPath)
//              {
//              DosSleep (3000);    // ???
//              CopyWPS (hiniAlias, EIN, szDesktop, szDesktopPath);     // ??? WPS Copy
//              }

//          happShell = 0;
            break;
        }
    DebugE (D_SAM, "WM_SHELL", "abort SAMMY");

    return;
    }

/***********************************************************************
    D i a l o g  -  W i n d o w   P r o c e d u r e
    (Login Dialog)
 ***********************************************************************/
MRESULT EXPENTRY LoginDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    switch (msg)
        {
        case WM_INITDLG:
            DebugS (D_SAM, "WM_INITDLG");
            SamInit (hwnd);
            return (MRESULT)FALSE;

        // Ausloggen aus WPSac
        // mp1: ./.
        // mp2: ./.
        case WM_LOGOFF:
            DebugS (D_SAM, "WM_LOGOFF");
            SamLogoff (hwnd);
            return 0;

        case WM_PAINT:
            SamPaint (hwnd);
            return 0;

        case WM_PAINTCOMPLETE:
            SamPaintComplete (hwnd);
            return 0;

        // Logon-Fenster sichtbar machen, Magnetkartensupport starten
        // mp1: ./.
        // mp2: ./.
        case WM_LOGON:
            DebugS (D_SAM, "WM_LOGON");
            SamLogon (hwnd);
            return 0;

        case WM_START_USER:
            DebugS (D_SAM, "WM_START_USER");
            SamStartUser (hwnd);
            return 0;

        case WM_LOGIN_COMPLETE:
            DebugS (D_SAM, "WM_LOGIN_COMPLETE");
            SamLoginComplete ();
            return 0;

        case WM_APPTERMINATENOTIFY:
            DebugS (D_SAM, "WM_APPTERMINATENOTIFY");
            SamAppTerminateNotify ((HAPP)mp1);
            return 0;

        case WM_MESSAGEBOX:
            DebugS (D_SAM, "WM_MESSAGEBOX");
            MessageBox (hwnd, (ULONG)mp1, MBTITLE_ERROR, MB_OK | MB_ERROR | MB_SYSTEMMODAL);
            WinSendMsg (hwnd, WM_LOGON, MPVOID, MPVOID);
            return 0;

        case WM_SAMMY_TERMINATE:
            DebugS (D_SAM, "WM_SAMMY_TERMINATE");
            WinSendMsg (hwnd, WM_SHELL, (MPARAM) SHELL_CLOSE, 0);
            return 0;

        case WM_SHELL:
            DebugS (D_SAM, "WM_SHELL");
            SamShell ((ULONG)mp1);
            return 0;

        // Magnetkarten-Read-Thread hat Daten erhalten
        // mp1: ./.
        // mp2: ./.
        case WM_IDCARD:
            DebugS (D_SAM, "WM_IDCARD");
            SamIdCard (hwnd);
            return 0;

        case WM_FOCUSCHANGE:
            SamFocusChange (LONGFROMMP (mp1), SHORT1FROMMP (mp2));
            break;

        case WM_COMMAND:
            DebugS (D_SAM, "WM_COMMAND");
            SamCommand (hwnd, mp1, mp2);
            return 0;

        case WM_HELP:
            DebugS (D_SAM, "WM_HELP");
            if (SamHelp ())
                return 0;
            break;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

MRESULT EXPENTRY TimedMsgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    switch (msg)
        {
        case WM_INITDLG:
            {
            CHAR szMsg[CCHMAXMSG];
            WinLoadString (hab, NULLHANDLE, *((PULONG)PVOIDFROMMP (mp2)), CCHMAXMSG, szMsg);
            WinSetDlgItemText (hwnd, DID_TEXT, szMsg);
            return FALSE;
            }
        }
    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

