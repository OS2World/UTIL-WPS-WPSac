#define INCL_WIN
#define INCL_PM
#define INCL_DOS

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "SamGlob.h"
#include "Profile.h"
#include "Tester.h"

HAB     hab;
HMQ     hmq;
HWND    hwndLogin;

/* Key fÅr die VerschlÅsselung */
ULONG key[2] = {ULKEY1, ULKEY2};

#ifdef DEBUG
ULONG   ulDebugMask = -1;
#endif // DEBUG

MRESULT EXPENTRY TesterWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);
void SingleSignon (HINI hini, PSZ szUser, BOOL bLogin);

int main (void)
    {
    QMSG        qmsg;
    HWND        hwndTester;
    ULONG       fcf = FCF_TITLEBAR   | FCF_SIZEBORDER | FCF_SYSMENU  | FCF_MINMAX | FCF_ICON |
                      FCF_VERTSCROLL | FCF_HORZSCROLL | FCF_TASKLIST | FCF_SHELLPOSITION;

    hab = WinInitialize (0);
    hmq = WinCreateMsgQueue (hab, 0);

    OpenLocINI ();

    WinRegisterClass (hab, "TESTER", TesterWndProc, CS_SAVEBITS, 0);

    hwndLogin = WinCreateStdWindow (HWND_DESKTOP,       // Parent
                                    0,                  // Frame-window style
                                    &fcf,               // Frame-creation flags
                                    "TESTER",           // Client class
                                    "Tester",           // Titlebar text
                                    0,                  // Client-window style
                                    NULLHANDLE,         // Resource id
                                    IDD_MAIN,           // Frame-window id
                                    &hwndTester);       // Client-window handle

    /* Message-Schleife */
    while (WinGetMsg (hab, &qmsg, 0L, 0L, 0L))
         WinDispatchMsg (hab, &qmsg);

    WinDestroyMsgQueue(hmq);
    WinTerminate(hab);

    return 0;
    }

MRESULT EXPENTRY TesterWndProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    HPS hps;
    RECTL rcl;

    switch (msg)
        {
        case WM_CREATE:
            WinPostMsg (hwnd, WM_USER, MPVOID, MPVOID);
            return FALSE;

        case WM_PAINT:
            hps = WinBeginPaint (hwnd, NULLHANDLE, &rcl);
            WinFillRect (hps, &rcl, CLR_WHITE);
            WinEndPaint (hps);
            return 0;

        case WM_USER:
            WinAlarm (HWND_DESKTOP, WA_NOTE);
            SingleSignon (QueryLocINIHandle (), "TINA", TRUE);
            WinAlarm (HWND_DESKTOP, WA_NOTE);
            return 0;
        }

    return WinDefWindowProc (hwnd, msg, mp1, mp2);
    }
