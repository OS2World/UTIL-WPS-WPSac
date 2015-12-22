/******************************************************************************
*
*  Module Name: SamDlg
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef SAMDLG_H
#define SAMDLG_H

#define WM_IDCARD           WM_USER
#define WM_USER_LOGINDLG    WM_USER+1
#define WM_LOGOFF           WM_USER+2
#define WM_MESSAGEBOX       WM_USER+3       // mp1: IDS_<text> | mp2: MPVOID
#define WM_PRFRESET         WM_USER+4
#define WM_LOGON            WM_USER+5
#define WM_LOGIN_COMPLETE   WM_USER+6       // LoginDlgProc; mp1: MPVOID | mp2: MPVOID
#define WM_SAMMY_TERMINATE  WM_USER+7
#define WM_START_USER       WM_USER+8
#define WM_PAINTCOMPLETE    WM_USER+9
#define WM_SHELL            WM_USER+10      // mp1: SHELL_RUN, SHELL_CLOSE, SHELL_NAME
                                            // mp2: PSZ ShellName (if mp1==SHELL_NAME)

/* mp1 fÅr WM_SHELL */
#define SHELL_CLOSE      1

/* Funktionsprototypen */
MRESULT EXPENTRY TimedMsgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

#endif /* SAMDLG_H */
