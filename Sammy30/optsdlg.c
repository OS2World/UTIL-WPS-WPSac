/***************************************************************************
 *
 * PROGRAMMNAME: SAMMY
 * -------------
 *
 * MODULNAME:    OptsDlg
 * ----------
 *
 * VERSION:      3.0
 * --------
 *
 * BESCHREIBUNG:
 * -------------
 *  Dialog-Procedure fÅr Option-Dialog
 *
 * FUNKTIONEN:
 * -----------
 *
 * COMPILER:
 * ---------
 *  IBM Visual Age C++ 3.0
 *
 * LIBRARIES:
 * ----------
 *
 * CHANGE LOG:
 * -----------
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *  3.00    21.02.97  Erster Release
 *
 *  Copyright (C) 1994...1997 noller & breining software
 *
 ******************************************************************************/
#define INCL_DOSPROCESS
#define INCL_WIN

#include <os2.h>
#include <stdlib.h>
#include <string.h>

#include "samglob.h"
#include "magpro2.h"
#include "profile.h"
#include "optsdlg.h"
#include "SammyDlg.h"
#include "Sammy.h"

MRESULT EXPENTRY OptionDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
MRESULT OptionCreate (HWND hwnd);
MRESULT OptionCommand (HWND hwnd, MPARAM mp1, MPARAM mp2);

static ULONG ulColor;
static CHAR  szBitmapName[CCHMAXPATH];

/*******************************************************************\
    OptionDialog: ôffnet den Optionen-Dialog.
    Eingang: hwnd:  Window-Handle des Owner
    Return:  TRUE:  eine Option wurde verstellt
             FALSE: keine énderungen
\*******************************************************************/
BOOL OptionDialog (HWND hwnd)
    {
    ULONG ulTmpColor;
    CHAR  szTmpBitmapName[CCHMAXPATH];
    CHAR  szColor[CCHMAXINT];

    QueryLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGCLR, szColor, CCHMAXINT);
    QueryLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGBMP, szBitmapName, CCHMAXPATH);

    if (szColor[0] == '\0')
        ulColor = CLR_DARKBLUE;
    else
        ulColor = atoi (szColor);

    ulTmpColor = ulColor;
    strcpy (szTmpBitmapName, szBitmapName);

    WinDlgBox (HWND_DESKTOP, hwnd, OptionDlgProc, NULLHANDLE, IDD_VIEW, NULL);

    _itoa (ulColor, szColor, 10);

    WriteLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGCLR, szColor);
    WriteLocINIString (MAGPRO_SYSTEM_APP, MAGPRO_KEY_SAMMY_BGBMP, szBitmapName);

    if ((strcmp (szTmpBitmapName, szBitmapName) == 0) && (ulTmpColor == ulColor))
        return FALSE;
    else
        return TRUE;
    }

/***********************************************************************
    D i a l o g  -  W i n d o w   P r o c e d u r e
    (Login Dialog)
 ***********************************************************************/
MRESULT EXPENTRY OptionDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    switch (msg)
        {
        case WM_INITDLG:
            return OptionCreate (hwnd);

        case WM_COMMAND:
            if (OptionCommand (hwnd, mp1, mp2) == 0)
                return 0;
            break;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

MRESULT OptionCreate (HWND hwnd)
    {
    SHORT x, y;

    /* Farbknîpfe initialisieren */
    for (y=0; y<2; y++)
        for (x=0; x<8; x++)
            WinSendMsg (WinWindowFromID (hwnd, DID_VALUE_COLOR),
                        VM_SETITEM,
                        MPFROM2SHORT (y+1, x+1),
                        MPFROMLONG (x+y*8));

    /* Bitmap-Eingabefeld initialisieren */
    WinSetDlgItemText (hwnd, DID_ENTER_FILE, szBitmapName);

    return FALSE;
    }

MRESULT OptionCommand (HWND hwnd, MPARAM mp1, MPARAM mp2)
    {
    ULONG ulIndex;
    ULONG ulColor;
    HWND  hwndVS;

    if (SHORT1FROMMP (mp2) == CMDSRC_PUSHBUTTON)
        {
        switch (SHORT1FROMMP (mp1))
            {
            case DID_OK:
                hwndVS  = WinWindowFromID (hwnd, DID_VALUE_COLOR);
                ulIndex = (ULONG)WinSendMsg (hwndVS, VM_QUERYSELECTEDITEM, MPVOID, MPVOID);
                ulColor = (ULONG)WinSendMsg (hwndVS, VM_QUERYITEM, MPFROMLONG (ulIndex), MPVOID);
                WinDismissDlg (hwnd, ulColor);
                return (MPARAM)0;

            case DID_SEARCH_FILE:
                {
                FILEDLG fileDlg;

                fileDlg.cbSize          = sizeof (FILEDLG);
                fileDlg.fl              = FDS_OPEN_DIALOG | FDS_CENTER;
                fileDlg.pszTitle        = NULL;
                fileDlg.pszOKButton     = NULL;
                fileDlg.pfnDlgProc      = NULL;
                fileDlg.pszIType        = NULL;
                fileDlg.papszITypeList  = NULL;
                fileDlg.pszIDrive       = NULL;
                fileDlg.papszIDriveList = NULL;
                fileDlg.hMod            = NULLHANDLE;
                strcpy (fileDlg.szFullFile, szBitmapName);
                WinFileDlg (HWND_DESKTOP, hwnd, &fileDlg);
                if (fileDlg.lReturn == DID_OK)
                    {
                    strcpy (szBitmapName, fileDlg.szFullFile);
                    WinSetDlgItemText (hwnd, DID_ENTER_FILE, szBitmapName);
                    }
                }
                return (MPARAM)0;

            case DID_CANCEL:
                return (MPARAM)1;
            }
        return (MPARAM)0;
        }

    return (MPARAM)1;
    }
