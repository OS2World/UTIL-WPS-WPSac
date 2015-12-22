/***************************************************************************
 *
 * PROGRAMMNAME: SAMMY
 * -------------
 *
 * MODULNAME:    Loginout
 * ----------
 *
 * VERSION:      3.0
 * --------
 *
 * BESCHREIBUNG:
 * -------------
 *  Abarbeitung von Login-Skripten durch Aufruf des OS/2-REXX-Interpreters.
 *  Ein Login-Skript kann aus beliebig vielen REXX-Skripten bestehen.
 *
 * FUNKTIONEN:   AddTIDValue
 * -----------   GetRexxVariable
 *               SetRexxVariable
 *               IOExit
 *               IniExit
 *               System
 *               SacPasswdAge
 *               SacConfirmPwd
 *               SacCreatePwd
 *               thExecScript
 *               SingleSignon
 *               PostSignonEnded
 *
 * COMPILER:
 * ---------
 *  IBM Visual Age C++ 3.0
 *
 * LIBRARIES:
 * ----------
 *  CUTIL.LIB     -   Hilfsroutinen
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
#define INCL_DOSMEMMGR
#define INCL_DOSMODULEMGR
#define INCL_DOSSEMAPHORES
#define INCL_DOSPROCESS
#define INCL_ERRORS
#define INCL_REXXSAA
#define INCL_WINSHELLDATA
#include <os2.h>
#include <REXXSAA.H>
#include <stdlib.H>
#include <string.h>
#include <malloc.h>
#include <limits.h>
#include <time.h>

#include "CreatePwd.h"
#include "debug.h"
#include "magpro2.h"
#include "profile.h"
#include "rxout.h"
#include "SamMem.h"
#include "sammy.h"

#define CCHMAXTEXT              64
#define CCHMAXHANDLER           16
#define STACKSIZE               0x8000

#define REXX_SUBC_LOGON         "Logon"
#define REXX_FUNC_CREATEPWD     "SacCreatePwd"
#define REXX_FUNC_PASSWDAGE     "SacPasswdAge"
#define REXX_FUNC_CONFIRMPWD    "SacConfirmPwd"
#define REXX_EXIT_INIEXIT       "IniExit"
#define REXX_EXIT_IOEXIT        "IOExit"
#define REXX_VAR_USERNAME       "USERNAME"
#define REXX_VAR_PASSWORD       "PASSWORD"
#define REXX_VAR_DOMAIN         "DOMAIN"
#define REXX_VAR_WPSAC_USERNAME "WPSAC_USERNAME"
#define REXX_VAR_WPSAC_SERVICE  "WPSAC_SERVICE"

#define PWD_DEFAULTLEN          6

typedef struct
    {
    HINI     hini;                      // INI-File Handle
    LONG     lScript;                   // Scripttyp: LOGON oder LOGOFF
    HEV      hev;                       // Event-Semaphore, die Ende der Prozedur anzeigt
    PSZ      pszUser;                   // WPSac-Username
    PSZ      pszService;                // Script-Name
    RXSTRING arxString[MAXSTRING];      // Service-Data Strings
    } SIGNONARG, *PSIGNONARG;

typedef struct
    {
    HWND    hwndSay;
    HWND    hwndTrc;
    } OUTHWND, *POUTHWND;

HMUX hMux = NULLHANDLE;

/********************************************************************
 *  HÑngt an einen String die Thread-ID an
 *  Eingang: psz: Eingangsstring
 *  Ausgang: psz: Eingangsstring + TID
 *  Return:  psz
 ********************************************************************/
PCHAR AddTIDValue (PCHAR psz)
    {
    CHAR szTid[5];          // Puffer fÅr TID in ASCII-Darstellung, max. USHORT!
    PTIB ptib = NULL;
    PPIB ppib = NULL;

    DosGetInfoBlocks (&ptib, &ppib);
    strcat (psz, _itoa (ptib->tib_ptib2->tib2_ultid & 0xFFFF, szTid, 16));

    return psz;
    }

/********************************************************************
 *  Rexx-Variable abfragen
 *  Eingang: name:  Name der Variablen
 *  Ausgang: value: Inhalt der Variablen
 *  Return: RÅckgabewert von RexxVariablePool
 ********************************************************************/
INT GetRexxVariable (PSZ name, PSZ *value)
    {
    ULONG    rc;
    SHVBLOCK block;

    block.shvcode = RXSHV_SYFET;
    block.shvret  = RXSHV_OK;
    block.shvnext = NULL;

    MAKERXSTRING (block.shvname, name, strlen (name));

    MAKERXSTRING (block.shvvalue, NULL, 0);

    block.shvnamelen  = strlen (name);
    block.shvvaluelen = 0;
    rc = RexxVariablePool (&block);

    if (rc == 0)
        {
        /* Da RXSTRING '\0' nicht umfa·t, mu· der String umkopiert werden */
        *value = AllocMem (block.shvvaluelen+1);
        strcpy (*value, block.shvvalue.strptr);
        DebugE (D_SAM, "FreeMem", "RexxVar1");
        FreeMem ((PPVOID)&block.shvvalue.strptr);
        DebugE (D_SAM, "FreeMem", "RexxVar2");
        }
    else
        value = NULL;

    return rc;
    }

/********************************************************************
 *  Rexx-Variable setzen
 *  Eingang: name:  Name der Variablen
 *           value: Inhalt der Variablen
 ********************************************************************/
INT SetRexxVariable (PSZ name, PSZ value)
    {
    SHVBLOCK block;

    block.shvcode = RXSHV_SYSET;
    block.shvret  = RXSHV_OK;
    block.shvnext = NULL;

    MAKERXSTRING (block.shvname, name, strlen (name));

    MAKERXSTRING (block.shvvalue, value, strlen (value));

    block.shvnamelen  = strlen (name);
    block.shvvaluelen = strlen (value);
    return RexxVariablePool (&block);
    }

/********************************************************************
 *  REXX-Exit-Handler: IO => Ausgaben von Trace und Say
 *  Eingang: ExitNumber:  ./.
 *           Subfunction: RXSIOTRC oder RXSIOSAY
 *           ParmBlock:   Parameter fÅr die Subfunction
 *  Return:  RXEXIT_HANDLED / RXEXIT_NOTHANDLED
 ********************************************************************/
LONG IOExit (LONG ExitNumber, LONG Subfunction, PEXIT ParmBlock)
    {
    RXSIOSAY_PARM *prxsiosay;
    USHORT usFlag;
    PCHAR  userdata[2];
    CHAR   szText[CCHMAXTEXT];
    CHAR   szIoExit[CCHMAXHANDLER] = REXX_EXIT_IOEXIT;

    RexxQueryExit (AddTIDValue (szIoExit), NULL, &usFlag, (PBYTE)userdata);

    switch (Subfunction)
        {
        case RXSIOTRC:
            prxsiosay = (RXSIOSAY_PARM *)ParmBlock;

            strcpy (szText, "Trace: ");
            strncat (szText, userdata[0], CCHMAXTEXT-sizeof ("Trace: "));
            szText[CCHMAXTEXT-1] = '\0';
            PostTextToWindow (&((POUTHWND)userdata[1])->hwndTrc,
                              prxsiosay->rxsio_string.strptr, szText, 0xFF0000);
            return RXEXIT_HANDLED;

        case RXSIOSAY:
            prxsiosay = (RXSIOSAY_PARM *)ParmBlock;

            strcpy (szText, "Ausgaben: ");
            strncat (szText, userdata[0], CCHMAXTEXT-sizeof ("Ausgaben: "));
            szText[CCHMAXTEXT-1] = '\0';
            PostTextToWindow (&((POUTHWND)userdata[1])->hwndSay,
                              prxsiosay->rxsio_string.strptr, szText, 0xFFFFC8);
            return RXEXIT_HANDLED;
        }
    return RXEXIT_NOT_HANDLED;
    }

/********************************************************************
 *  REXX-Exit-Handler: Init => Initialisierung der REXX-Variablen
 *     "USERNAME", "PASSWORD", "DOMAIN", "WPSAC_USERNAME" und
 *     "WPSAC_SERVICE"
 *  Eingang: ExitNumber:  ./.
 *           Subfunction: ./.
 *           ParmBlock:   ./.
 *  Return:  RXEXIT_HANDLED
 ********************************************************************/
LONG IniExit (LONG ExitNumber, LONG Subfunction, PEXIT ParmBlock)
    {
    USHORT     usFlag;
    PSIGNONARG userdata[2];
    CHAR       szIniExit[CCHMAXHANDLER] = REXX_EXIT_INIEXIT;

    RexxQueryExit (AddTIDValue (szIniExit), NULL, &usFlag, (PBYTE)userdata);

    SetRexxVariable (REXX_VAR_USERNAME,       userdata[0]->arxString[USERNAME].strptr);
    SetRexxVariable (REXX_VAR_PASSWORD,       userdata[0]->arxString[PASSWORD].strptr);
    SetRexxVariable (REXX_VAR_DOMAIN,         userdata[0]->arxString[DOMAIN].strptr);
    SetRexxVariable (REXX_VAR_WPSAC_USERNAME, userdata[0]->pszUser);
    SetRexxVariable (REXX_VAR_WPSAC_SERVICE,  userdata[0]->pszService);

    return RXEXIT_HANDLED;
    }

/********************************************************************
 *  REXX-Subcommand-Handler: Default
 *  Eingang: pCommand: Kommando fÅr den Subcommand-Handler
 *           pFlags:   Ergebnis-Flags
 *  Ausgang: pRetstr:  RÅckgabewert von system()
 *  Return:  RÅckgabewert von system()
 ********************************************************************/
ULONG System (PRXSTRING pCommand, PUSHORT pFlags, PRXSTRING pRetstr)
    {
    int rc;

    rc = system (pCommand->strptr);

    *pFlags = (rc == -1 ? RXSUBCOM_ERROR : RXSUBCOM_OK);
    pRetstr->strlength = strlen (_itoa (rc, pRetstr->strptr, 10));

    return (ULONG)rc;
    }

/********************************************************************
 *  REXX-Funktion: Pa·wortalter in Tagen bestimmen
 *  Eingang: Name:      ./.
 *           Argc:      ./.
 *           Argv:      ./.
 *           Queuename: ./.
 *  Ausgang: pRetstr:   Alter in Tagen
 *  Return:  0
 ********************************************************************/
ULONG SacPasswdAge (PSZ Name, LONG Argc, RXSTRING Argv[], PSZ Queuename, PRXSTRING pRetstr)
    {
    ULONG    ulc;
    INT      rc;
    PSZ      pszUsername;
    PSZ      pszService;
    LONG     lAge;
    time_t   timePwd;
    RXSTRING arxString[MAXSTRING];

    /* User/Script bestimmen */
    rc  = GetRexxVariable (REXX_VAR_WPSAC_USERNAME, &pszUsername);
    rc |= GetRexxVariable (REXX_VAR_WPSAC_SERVICE,  &pszService);

    /* Erzeugungszeit des Pa·wortes lesen */
    if (rc == 0)
        {
        ReadServiceData (pszUsername, pszService, arxString);
        timePwd = atoi (arxString[PWDAGE].strptr);
        for (ulc = 0; ulc < MAXSTRING; ulc++)
            {
            DebugE (D_SAM, "FreeMem", "RexxPwd1");
            DosFreeMem (arxString[ulc].strptr);
            DebugE (D_SAM, "FreeMem", "RexxPwd2");
            }
        lAge = timePwd ? (LONG)difftime (time (NULL), timePwd) / (60*60*24) : 0;
        _itoa (lAge, pRetstr->strptr, 10);
        pRetstr->strlength = strlen (pRetstr->strptr);
        }

    return 0;
    }

/********************************************************************
 *  REXX-Funktion: Neues Pa·wort bestÑtigen. Dazu werden in den
 *  Logon-Skript-Daten (s. enum _string) die EintrÑge fÅr das
 *  aktuelle Pa·wort durch die des neuen ersetzt.
 *  Eingang: Name:      ./.
 *           Argc:      ./.
 *           Argv:      ./.
 *           Queuename: ./.
 *  Ausgang: pRetstr:   "0"
 *  Return:  0
 ********************************************************************/
ULONG SacConfirmPwd (PSZ Name, LONG Argc, RXSTRING Argv[], PSZ Queuename, PRXSTRING pRetstr)
    {
    PSZ      pszUsername;
    PSZ      pszService;
    ULONG    ulc;
    INT      rc;
    RXSTRING arxString[MAXSTRING];

    /* User/Script bestimmen */
    rc  = GetRexxVariable (REXX_VAR_WPSAC_USERNAME, &pszUsername);
    rc |= GetRexxVariable (REXX_VAR_WPSAC_SERVICE,  &pszService);

    if (rc == 0)
        {
        ReadServiceData (pszUsername, pszService, arxString);
        arxString[PASSWORD] = arxString[NEWPASSWORD];
        arxString[PWDAGE]   = arxString[NEWPWDAGE];
        WriteServiceData (pszUsername, pszService, arxString);
        }

    for (ulc = 0; ulc < MAXSTRING; ulc++)
        {
        DebugE (D_SAM, "FreeMem", "Confirm1");
        DosFreeMem (arxString[ulc].strptr);
        DebugE (D_SAM, "FreeMem", "Confirm2");
        }

    pRetstr->strptr[0] = '0';
    pRetstr->strptr[1] = '\0';
    pRetstr->strlength = 1;

    return 0;
    }

/********************************************************************
 *  REXX-Funktion: Erzeugung eines Pa·wortes. Das neue Pa·wort wird
 *  einschlie·lich der Erzeugungszeit in den Logon-Skript-Daten
 *  (s. enum _string) abgelegt.
 *  Eingang: Name:      ./.
 *           Argc:      Zahl der Strings in Argv
 *           Argv:      Argumente an SacCreatePwd
 *           Queuename: ./.
 *  Ausgang: pRetstr:   neues Pa·wort
 *  Return:  0
 ********************************************************************/
ULONG SacCreatePwd (PSZ Name, LONG Argc, RXSTRING Argv[], PSZ Queuename, PRXSTRING pRetstr)
    {
    PSZ      pszUsername;
    PSZ      pszService;
    INT      rc;
    RXSTRING rxAge;
    ULONG    ulPwdLen = PWD_DEFAULTLEN;
    ULONG    ulAlpha  = 0;
    ULONG    ulNum    = 0;

    /* Bestimmen der Pa·wort-LÑngenparameter */
    if (Argc > 0)
        ulPwdLen = atoi (Argv[0].strptr);
    if (Argc > 1)
        ulAlpha  = atoi (Argv[1].strptr);
    if (Argc > 2)
        ulNum    = atoi (Argv[2].strptr);

    /* PrÅfung, ob LÑngenangaben korrekt sind */
    if (ulPwdLen > 255 || ulAlpha + ulNum > ulPwdLen)
        {
        pRetstr->strlength = 0;
        pRetstr->strptr    = NULL;
        return 0;
        }

    createPwd (pRetstr->strptr, ulPwdLen-ulAlpha-ulNum, ulAlpha, ulNum);

    /* Erzeugungszeit des Pa·wortes */
    DosAllocMem ((PPVOID)&rxAge.strptr, 11, PAG_COMMIT | PAG_READ | PAG_WRITE);
    _itoa (time (NULL), rxAge.strptr, 10);
    rxAge.strlength = strlen (rxAge.strptr);

    /* User/Script bestimmen */
    rc  = GetRexxVariable (REXX_VAR_WPSAC_USERNAME, &pszUsername);
    rc |= GetRexxVariable (REXX_VAR_WPSAC_SERVICE,  &pszService);

    /* Neues Pa·wort schreiben */
    if (rc == 0)
        {
        WriteServiceDataString (pszUsername, pszService, NEWPASSWORD, pRetstr);
        WriteServiceDataString (pszUsername, pszService, NEWPWDAGE,   &rxAge);
        }

    return 0;
    }

/********************************************************************
 *  REXX-Funktion: Logon am LAN-Server, falls LSPrelog vorhanden ist
 *  Eingang: Name:      ./.
 *           Argc:      Zahl der Strings in Argv (=3)
 *           Argv:      Argumente: Userid, Password, Domain
 *           Queuename: ./.
 *  Ausgang: pRetstr:   RÅckgabewert von DoLogon
 *  Return:  0 = ok; 1 = Fehler
 ********************************************************************/
ULONG SacLSLogon (PSZ Name, LONG Argc, RXSTRING Argv[], PSZ Queuename, PRXSTRING pRetstr)
    {
    APIRET   rc;
    ULONG    ulRc;
    HMODULE  hModule;
    CHAR     szError[CCHMAXPATHCOMP];
    BOOL (* APIENTRY pfnLogon)(PSZ, PSZ, PSZ);

    /* PrÅfen der Parameterzahl */
    if ((Argc != 3) ||
        RXNULLSTRING (Argv[0]) || RXZEROLENSTRING (Argv[0]) ||
        RXNULLSTRING (Argv[2]) || RXZEROLENSTRING (Argv[2]))
        return 1;

    /* PrÅfen, ob DLL ladbar */
    rc = DosLoadModule (szError, CCHMAXPATHCOMP, "LSPRELOG", &hModule);
    if (rc != NO_ERROR)
        return 1;

    rc = DosQueryProcAddr (hModule, 0, "DoLogon",  (PFN *)&pfnLogon);
    if (rc)
        {
        DosFreeModule (hModule);
        return 1;
        }

    /*  Aufruf von DoLogon */
    ulRc = (pfnLogon)(Argv[0].strptr,
                      (RXNULLSTRING (Argv[1]) || RXZEROLENSTRING (Argv[1])) ? "" : Argv[1].strptr,
                      Argv[2].strptr);
    DosFreeModule (hModule);

    /* RÅckgabewert ausfÅllen */
    _itoa (ulRc, pRetstr->strptr, 10);
    pRetstr->strlength = strlen (pRetstr->strptr);

    return 0;
    }

/********************************************************************
 *  REXX-Funktion: Logoff am LAN-Server, falls LSPrelog vorhanden ist
 *  Eingang: Name:      ./.
 *           Argc:      Zahl der Strings in Argv (=2)
 *           Argv:      Argumente: Userid, Domain
 *           Queuename: ./.
 *  Ausgang: pRetstr:   RÅckgabewert von DoLogoff
 *  Return:  0 = ok; 1 = Fehler
 ********************************************************************/
ULONG SacLSLogoff (PSZ Name, LONG Argc, RXSTRING Argv[], PSZ Queuename, PRXSTRING pRetstr)
    {
    APIRET   rc;
    ULONG    ulRc;
    HMODULE  hModule;
    CHAR     szError[CCHMAXPATHCOMP];
    BOOL (* APIENTRY pfnLogoff)(PSZ, PSZ);

    /* PrÅfen der Parameterzahl */
    if ((Argc != 2) ||
        RXNULLSTRING (Argv[0]) || RXZEROLENSTRING (Argv[0]) ||
        RXNULLSTRING (Argv[1]) || RXZEROLENSTRING (Argv[1]))
        return 1;

    /* PrÅfen, ob DLL ladbar */
    rc = DosLoadModule (szError, CCHMAXPATHCOMP, "LSPRELOG", &hModule);
    if (rc != NO_ERROR)
        return 1;

    rc  = DosQueryProcAddr (hModule, 0, "DoLogoff",  (PFN *)&pfnLogoff);
    if (rc)
        {
        DosFreeModule (hModule);
        return 1;
        }

    /*  Aufruf von DoLogon */
    ulRc = (pfnLogoff)(Argv[0].strptr,
                       Argv[1].strptr);
    DosFreeModule (hModule);

    /* RÅckgabewert ausfÅllen */
    _itoa (ulRc, pRetstr->strptr, 10);
    pRetstr->strlength = strlen (pRetstr->strptr);

    return 0;
    }

/********************************************************************
 *  Thread zur Abarbeitung einer einzelnen REXX-Prozedur innerhalb
 *  des Logon-Skriptes
 *  Eingang: pThArg:  Zeiger auf Struktur vom Typ SIGNONARG
 ********************************************************************/
void _Optlink thExecScript (void *pThArg)
    {
    ULONG     ulSize, ulc;
    BOOL      bRC;
    PSZ       pszCommand;
    SHORT     rc;
    PCHAR     userdata[2];
    RXSTRING  rx_return;
    RXSTRING  instore[2];
    RXSYSEXIT rxsExit[3];
    LONG      lScript       = ((PSIGNONARG)pThArg)->lScript;
    OUTHWND   strucHwnd     = {NULLHANDLE, NULLHANDLE};
    CHAR      szIoExit[CCHMAXHANDLER]  = REXX_EXIT_IOEXIT;
    CHAR      szIniExit[CCHMAXHANDLER] = REXX_EXIT_INIEXIT;

    ReadServiceData (((PSIGNONARG)pThArg)->pszUser,
                     ((PSIGNONARG)pThArg)->pszService,
                     ((PSIGNONARG)pThArg)->arxString);

    /* REXX-Script lesen */
    if (((PSIGNONARG)pThArg)->arxString[lScript].strptr)
        {
        bRC = PrfQueryProfileSize (((PSIGNONARG)pThArg)->hini,
                                   MAGPRO_SCRIPT_APP,
                                   ((PSIGNONARG)pThArg)->arxString[lScript].strptr,
                                   &ulSize);
        }
    else
        bRC = FALSE;

    /* REXX-Script starten */
    if (bRC && ulSize)
        {
        pszCommand = (PSZ)malloc (1024); //AllocMem (ulSize+1);
        PrfQueryProfileString (((PSIGNONARG)pThArg)->hini,
                               MAGPRO_SCRIPT_APP,
                               ((PSIGNONARG)pThArg)->arxString[lScript].strptr,
                               NULL,
                               pszCommand,
                               ulSize+1);
        pszCommand[ulSize] = '\0';

        instore[0].strlength = ulSize;                  // ASCII-REXX-Script
        instore[0].strptr    = (PCHAR)pszCommand;
        instore[1].strlength = 0;                       // keine Token-Version
        instore[1].strptr    = NULL;

        userdata[0] = pThArg;
        userdata[1] = NULL;
        RexxRegisterExitExe (AddTIDValue (szIniExit), (PFN)&IniExit, (PBYTE)userdata);

        userdata[0] = ((PSIGNONARG)pThArg)->arxString[lScript].strptr,
        userdata[1] = (PCHAR)&strucHwnd;
        RexxRegisterExitExe (AddTIDValue (szIoExit),  (PFN)&IOExit,  (PBYTE)userdata);

        rxsExit[0].sysexit_name = szIniExit;            // Exit Handler: Initialisierung
        rxsExit[0].sysexit_code = RXINI;
        rxsExit[1].sysexit_name = szIoExit;             // Exit Handler: stdout
        rxsExit[1].sysexit_code = RXSIO;
        rxsExit[2].sysexit_code = RXENDLST;

        ulc = RexxStart (0, NULL, REXX_SUBC_LOGON, instore, REXX_SUBC_LOGON,
                   RXCOMMAND, rxsExit, &rc, &rx_return);

        DebugULx (D_SAM, "RexxStart", "return", ulc);

        RexxDeregisterExit (szIoExit,  NULL);
        RexxDeregisterExit (szIniExit, NULL);

        DebugE (D_SAM, "FreeMem", "DestroyScript1");
        FreeMem ((PPVOID)&pszCommand);
        DebugE (D_SAM, "FreeMem", "DestroyScript2");
        if (instore[1].strptr)
            {
            DebugE (D_SAM, "FreeMem", "DestroyInstore1");
            DosFreeMem (instore[1].strptr);
            DebugE (D_SAM, "FreeMem", "DestroyInstore2");
            }
        if (rx_return.strptr)
            DosFreeMem (rx_return.strptr);
        }

    DosPostEventSem (((PSIGNONARG)pThArg)->hev);

    DebugE (D_SAM, "FreeMem", "Exec1");
    FreeMem ((PPVOID)&((PSIGNONARG)pThArg)->pszUser);
    DebugE (D_SAM, "FreeMem", "Exec2");
    FreeMem ((PPVOID)&((PSIGNONARG)pThArg)->pszService);
    DebugE (D_SAM, "FreeMem", "Exec3");
    for (ulc = 0; ulc < MAXSTRING; ulc++)
        if (((PSIGNONARG)pThArg)->arxString[ulc].strptr)
            DosFreeMem  (((PSIGNONARG)pThArg)->arxString[ulc].strptr);
    DebugE (D_SAM, "FreeMem", "Exec4");
    FreeMem ((PPVOID)&pThArg);
    DebugE (D_SAM, "FreeMem", "Exec5");
    return;
    }

/********************************************************************
 *  Abarbeiten eines Logon-Skriptes
 *  Eingang: hini:    Handle der (lokalen) INI-Datei
 *           pszUser: Benutzername
 *           bLogin:  TRUE: Login, FALSE: Logout
 ********************************************************************/
VOID SingleSignon (HINI hini, PSZ pszUser, BOOL bLogin)
    {
    PSZ        szKeyNames;
    USHORT     usFlags;
    ULONG      aulUserdata[2];
    ULONG      ulSize, ulIndex, ulSem;
    BOOL       bRC;
    SEMRECORD  arSem[64];
    PSIGNONARG pThArg;

    /* Rexx vorbereiten */
    if (RexxQuerySubcom (REXX_SUBC_LOGON, NULL, &usFlags, (PBYTE)aulUserdata) == RXSUBCOM_NOTREG)
        RexxRegisterSubcomExe (REXX_SUBC_LOGON, (PFN)&System, NULL);

    if (RexxQueryFunction (REXX_FUNC_CREATEPWD) == RXFUNC_NOTREG)
        RexxRegisterFunctionExe (REXX_FUNC_CREATEPWD,  (PFN)&SacCreatePwd);
    if (RexxQueryFunction (REXX_FUNC_PASSWDAGE) == RXFUNC_NOTREG)
        RexxRegisterFunctionExe (REXX_FUNC_PASSWDAGE,  (PFN)&SacPasswdAge);
    if (RexxQueryFunction (REXX_FUNC_CONFIRMPWD) == RXFUNC_NOTREG)
        RexxRegisterFunctionExe (REXX_FUNC_CONFIRMPWD, (PFN)&SacConfirmPwd);

    /* Logon-Prozeduren auslesen */
    bRC = PrfQueryProfileSize (hini, pszUser, NULL, &ulSize);
    if (!bRC || !ulSize)
        return;

    szKeyNames = (PSZ)AllocMem (ulSize);
    PrfQueryProfileString (hini, pszUser, NULL, NULL, szKeyNames, ulSize);

    /* Schleife Åber alle Logon-Prozeduren */
    ulSem = 0;
    hMux  = NULLHANDLE;
    for (ulIndex = 0; szKeyNames[ulIndex] != '\0';)
        {
        if (strncmp (&szKeyNames[ulIndex], MAGPRO_SERVICE_KEY, 2) == 0)
            {
            /* Event-Semaphore erzeugen */
            DosCreateEventSem (NULL, (PHEV)&arSem[ulSem].hsemCur, 0, FALSE);
            arSem[ulSem].ulUser = ulSem;

            /* REXX-Prozedur starten */
            pThArg = (PSIGNONARG)AllocMem (sizeof (SIGNONARG));
            pThArg->hini       = hini;
            pThArg->pszUser    = AllocMem (strlen (pszUser) + 1);
            pThArg->pszService = AllocMem (strlen (&szKeyNames[ulIndex]) + 1);
            pThArg->lScript    = bLogin ? LOGIN : LOGOUT;
            pThArg->hev        = (HEV)arSem[ulSem].hsemCur;
            strcpy (pThArg->pszUser,    pszUser);
            strcpy (pThArg->pszService, &szKeyNames[ulIndex]);

            _beginthread (thExecScript, NULL, STACKSIZE, pThArg);

            ulSem++;
            }
        ulIndex += strlen (&szKeyNames[ulIndex]) + 1;
        }

    /* Multiplex-Semaphore erzeugen */
    if (ulSem > 0)
        DosCreateMuxWaitSem (NULL, &hMux, ulSem, arSem, DCMW_WAIT_ALL);

    DebugE (D_SAM, "FreeMem", "KeyNames1");
    FreeMem ((PPVOID)&szKeyNames);
    DebugE (D_SAM, "FreeMem", "KeyNames2");
    return;
    }

/********************************************************************
 *  Wartet auf das Beenden aller Rexx-Prozeduren und Postet anschl.
 *  eine Message
 *  Eingang: hwnd: Ziel-Fenster des Message
 *           msg:  Message-Id
 ********************************************************************/
VOID PostSignonEnded (HWND hwnd, ULONG msg)
    {
    ULONG  ulUser;

    if (hMux)
        DosWaitMuxWaitSem (hMux, SEM_INDEFINITE_WAIT, &ulUser);

    if (hwnd)
        WinPostMsg (hwnd, msg, MPVOID, MPVOID);

    return;
    }

