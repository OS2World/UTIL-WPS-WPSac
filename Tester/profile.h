/******************************************************************************
*
*  Module Name: Sammy
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef PROFILE_H
#define PROFILE_H

#include <REXXSAA.H>

PSZ     QueryProfile (PSZ szPath, ULONG ulLen, BOOL isLocalIni);

VOID    ReadServiceData (PSZ pszUser, PSZ pszService, RXSTRING Argv[]);
BOOL    ReadServiceDataString (PSZ pszUser, PSZ pszService, LONG lString, PRXSTRING prxString);
VOID    WriteServiceData (PSZ pszUser, PSZ pszService, RXSTRING Argv[]);
BOOL    WriteServiceDataString (PSZ pszUser, PSZ pszService, LONG lString, PRXSTRING prxString);
VOID    AddToTodoList (PSZ pszUser, PSZ pszString);
ULONG   GetPwdValidIntvl (VOID);
BOOL    IsPwdAgeSet (PSZ pszUser);
VOID    SetPwdAge (PSZ pszUser);
ULONG   GetPwdAge (PSZ pszUser);
BOOL    OpenLocINI (VOID);
HINI    QueryLocINIHandle (VOID);
BOOL    QueryLocINISize (PSZ pszApp, PSZ pszKey, PULONG pulc);
ULONG   QueryLocINIString (PSZ pszApp, PSZ pszKey, PSZ pszDest, ULONG ulc);
BOOL    QueryLocINIData (PSZ pszApp, PSZ pszKey, PVOID pDest, PULONG pulc);
BOOL    QueryLocINICryptedData (PSZ pszApp, PSZ pszKey, PCHAR pDest, ULONG ulc);
LONG    QueryLocINIInt (PSZ pszApp, PSZ pszKey, LONG lDefault);
BOOL    WriteLocINIString (PSZ pszApp, PSZ pszKey, PSZ pszSource);
BOOL    WriteLocINICryptedData (PSZ pszApp, PSZ pszKey, PSZ pSource, ULONG ulc);
VOID    CloseLocINI (VOID);
LONG    QueryLocINIStringFlag (PSZ pszApp, PSZ pszKey,
                               PSZ pszDest, ULONG ulc,
                               PULONG pulFlag, ULONG ulFlag);
VOID    SaveLocalINIEntries (VOID);
BOOL    RestoreLocalINIEntries (VOID);
BOOL    WriteAdminFlag (PSZ pszUser, BOOL bIsAdmin);
BOOL    ReadAdminFlag (PSZ pszUser);
BOOL    HandleTodoLists (VOID);
BOOL    HandleTodoList (PSZ pszUser);
BOOL    CreateCheck1User (VOID);

#endif /* PROFILE_H */
