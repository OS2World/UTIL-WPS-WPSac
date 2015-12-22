/***************************************************************************\
 *
 * PROGRAM NAME: SAMMY
 * -------------
 *
 * REVISION LEVEL: 4.0
 * ---------------
 *
 * MODULE NAME: ChangePwd
 * ------------
 *
 * WHAT THIS PROGRAM DOES:
 * -----------------------
 *  OS/2 Workplace Shell Access Manager:
 *  Umschalten zwischen den Shells fÅr WPSam und WPSac
 *
 * ROUTINES:
 * ---------
 *    ChgPwdDlgProc ()
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *  IBM Visual Age C++ Compiler Version 3.0
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
 *  4.00    11-12-94  Version mit WPSac-UnterstÅtzung
 *
 *  Copyright (C) 1994...1999 noller & breining software
 *
\******************************************************************************/
#define INCL_WIN
#define INCL_DOS
#include <os2.h>
#include <string.h>

#include "SamGlob.h"
#include "SamData.h"
#include "ChangePwd.h"
#include "SacAlias.h"
#include "Sammy.h"          // AliasFind
#include "SammyDlg.h"

/*******************************************************************\
    ChgPwdInit: Initialisierung des Dialogs (WM_INITDLG)
    Eingang: hwnd:    Window-Handle des Dialogs
             pszUser: Zeiger auf String mit Benutzername / NULL
\*******************************************************************/
VOID ChgPwdInit (HWND hwnd, PSZ pszUser)
    {
    if (pszUser)
        {
        WinShowWindow (WinWindowFromID (hwnd, DID_TXT_PWDEXPIRED), TRUE);
        WinSetDlgItemText (hwnd, DID_EDIT_USER, pszUser);
        }

    return;
    }

/*******************************************************************\
    ChgPwdCommand: Behandlung von WM_COMMAND
    Eingang: hwnd:         Window-Handle des Dialogs
             usSourceType: Ursache der Message
             usCmdValue:   Control-Id
    Return: TRUE:  0 an zurÅckgeben
            FALSE: WinDefDlgProc rufen (->DismissDlg)
\*******************************************************************/
BOOL ChgPwdCommand (HWND hwnd, USHORT usSourceType, USHORT usCmdValue)
    {
    ULONG     ulRet;
    ULONG     idText;
    ALIASDATA strucLogin;
    CHAR      szPwd1[CCHMAXPASSWORD], szPwd2[CCHMAXPASSWORD];

    if (usSourceType == CMDSRC_PUSHBUTTON)
        {
        switch (usCmdValue)
            {
            case DID_OK:
                /* Benutzer/Pa·wort aus Eingabefeld abfragen */
                memset (&strucLogin, '\0', sizeof (ALIASDATA));
                WinQueryDlgItemText (hwnd, DID_EDIT_USER, CCHMAXUSERNAME, strucLogin.szUser);
                ulRet = WinQueryDlgItemText (hwnd, DID_EDIT_OLDPWD, CCHMAXPASSWORD, strucLogin.szPassword);
                strucLogin.ulFlag = ALIAS_USER | (ulRet ? ALIAS_PASSWORD : 0);

                /* Benutzer in Datenbank suchen und verifizieren */
                ulRet = AliasFind (&strucLogin);

                /* neue Pa·wîrter verarbeiten */
                switch (ulRet)
                    {
                    case STAT_PASSWORDOK:
                    case STAT_NOPASSWORD:
                        WinQueryDlgItemText (hwnd, DID_EDIT_PASSWORD,     CCHMAXPASSWORD, szPwd1);
                        WinQueryDlgItemText (hwnd, DID_EDIT_VERIFICATION, CCHMAXPASSWORD, szPwd2);
                        WinUpper (hab, 0, 0, szPwd1);
                        WinUpper (hab, 0, 0, szPwd2);
                        idText = strcmp (szPwd1, szPwd2) ? IDS_DIFFERENTPWDS : 0;
                        break;

                    case STAT_NOUSERFOUND:
                    case STAT_USERNOTFOUND:
                        idText = IDS_USERNOTFOUND;
                        break;

                    case STAT_WRONGPASSWD:
                        idText = IDS_WRONGPASSWORD;
                        break;
                    }

                /* Fehlermeldung ausgeben */
                if (idText)
                    {
                    MessageBox (hwnd, idText, MBTITLE_ERROR, MB_OK | MB_ERROR);
                    return TRUE;
                    }

                /* neues Pa·wort sichern und beenden */
                AliasSaveNewPassword (strucLogin.szUser, szPwd1);
                return FALSE;

            case DID_CANCEL:
                return FALSE;
            }
        }

    return TRUE;
    }

/***********************************************************************
    D i a l o g  -  W i n d o w   P r o c e d u r e
    (Kennwort Ñndern)
 ***********************************************************************/
MRESULT EXPENTRY ChgPwdDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
    {
    switch (msg)
        {
        case WM_INITDLG:
            ChgPwdInit (hwnd, (PSZ)mp2);
            return (MRESULT)TRUE;

        case WM_COMMAND:
            if (ChgPwdCommand (hwnd, SHORT1FROMMP (mp2), SHORT1FROMMP (mp1)))
                return (MRESULT)0;
            break;
        }

    return WinDefDlgProc (hwnd, msg, mp1, mp2);
    }

