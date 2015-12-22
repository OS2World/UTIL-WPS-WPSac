/***************************************************************************
 *
 * PROGRAM NAME: LSPrelog.dll
 * -------------
 *
 * REVISION LEVEL: 4.0
 * ---------------
 *
 * WHAT THIS PROGRAM DOES:
 * -----------------------
 *  OS/2 Workplace Shell Access Manager:
 *  Umschalten zwischen den Shells fÅr WPSam und WPSac
 *
 * ROUTINES:
 * ---------
 *  ReadProfile ()
 *  DoPreLogon ()
 *  DoPreLogoff ()
 *  DoLogon ()
 *  DoLogoff ()
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *  IBM C/C++ Set/2 Compiler Version 2.0
 *  IBM OS/2 2.1 Programmers Toolkit
 *
 * REQUIRED FILES:
 * ---------------
 *
 * REQUIRED LIBRARIES:
 * -------------------
 *  OS2386.LIB    -   OS/2 32-Bit import library
 *  CUTIL.LIB     -   Hilfsroutinen
 *  UPM.LIB       -   UPM library
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
/*------- OS/2 include files -----------------------------------------------*/
#define INCL_DOSFILEMGR
#define INCL_WINCOUNTRY
#define INCL_WINSHELLDATA
#include <os2.h>

/*------- UPM include files ------------------------------------------------*/
#include <upm.h>

/*------- C include files --------------------------------------------------*/
#include <string.h>
#include <ctype.h>

#include "magpro2.h"
#include "samglob.h"
#include "crypt.h"
#include "LSPrelog.h"
#include "debug.h"

#define CLASSNAME   "LSPrelog"

ULONG key[2] = {ULKEY1, ULKEY2};

#ifdef DEBUG
ULONG   ulDebugMask = -1;
#endif // DEBUG

/* Variable fÅr Prelogon */
static CHAR szUserid[CCHMAXNAMELEN];            // Username
static CHAR szPassword[CCHMAXNAMELEN];          // Passwort
static CHAR szPrelogonDomain[CCHMAXDOMAINNAME]; // DomÑne (fÅr LS mit vorgestelltem "LS:"

HINI OpenProfile (VOID)
    {
    CHAR  szProfilePath[CCHMAXPATH];
    HAB   hab;
    ULONG ulLenPath, ulLenFile;

    /* 1. INI-Pfad holen */
    ulLenPath = PrfQueryProfileString (HINI_SYSTEMPROFILE,
                                       OS2SYS_APP_NAME,
                                       OS2SYS_KEY_CLNT_LOCAL,
                                       NULL,
                                       szProfilePath,
                                       CCHMAXPATH);
    szProfilePath[ulLenPath] = '\0';

    /* 2. INI-Datei îffnen */
    if (szProfilePath[0] != '\0')
        {
        szProfilePath[ulLenPath] = '\\';
        ulLenFile = PrfQueryProfileString (HINI_SYSTEMPROFILE,
                                           OS2SYS_APP_NAME,
                                           OS2SYS_KEY_INI_FILE,
                                           NULL,
                                           szProfilePath+ulLenPath+1,
                                           CCHMAXPATH-ulLenPath-1);
        if (ulLenFile > 0)
            {
            szProfilePath[ulLenPath + ulLenFile + 1] = '\0';
            hab = WinQueryAnchorBlock (HWND_DESKTOP);
            return PrfOpenProfile (hab, szProfilePath);
            }
        }

    return NULLHANDLE;
    }


ULONG ReadProfile (VOID)
    {
    HINI  hini;
    ULONG ulLength;

    hini = OpenProfile ();

    /* Lesen der Prelogon-DomÑne */
    ulLength = PrfQueryProfileString (hini,
                                      MAGPRO_PRELOGON_APP,
                                      MAGPRO_KEY_PRLOG_DOMAIN,
                                      NULL,
                                      szPrelogonDomain,
                                      CCHMAXDOMAINNAME);

    if (ulLength < strlen (SIGNATURE_LS))
        return PRLERR_INVDOMAIN;

    if (memcmp (szPrelogonDomain, SIGNATURE_LS, strlen (SIGNATURE_LS)) != 0)
        return PRLERR_INVDOMAIN;

    memmove (szPrelogonDomain, &szPrelogonDomain[3], strlen (&szPrelogonDomain[2]));

    /* Lesen des Prelogon-Users */
    ulLength = PrfQueryProfileString (hini,
                                      MAGPRO_PRELOGON_APP,
                                      MAGPRO_KEY_PRLOG_USER,
                                      NULL,
                                      szUserid,
                                      CCHMAXNAMELEN);

    if (ulLength == 0)
        return PRLERR_NOPRELOGUSER;

    /* Lesen des (optionalen) Prelogon-Passwortes */
    PrfQueryProfileString (hini,
                           MAGPRO_PRELOGON_APP,
                           MAGPRO_KEY_PRLOG_PWD,
                           "",
                           szPassword,
                           CCHMAXNAMELEN);

    CRYPT_STRING (szPassword, CCHMAXNAMELEN, CA_Decrypt);
    if (memchr (szPassword, '\0', CCHMAXNAMELEN) == NULL)
        return PRLERR_INVPASSWORD;

    return PRLERR_OK;
    }

ULONG APIENTRY DoPreLogon (VOID)
    {
    ULONG  rc;
    USHORT usRc;

    rc = ReadProfile ();
    if (rc != PRLERR_OK)
        return rc;

    WinUpper (WinQueryAnchorBlock (HWND_DESKTOP),   /* password must be UPPERCASE */
              0, 0,
              szPassword);
    DebugE (D_SAM, "Logon for User", szUserid);
    DebugE (D_SAM, "Logon for Passwd", szPassword);
    DebugE (D_SAM, "Logon for Domain", szPrelogonDomain);
    usRc = u32eulgn (szUserid,                      /* userid     */
                     szPassword,                    /* password   */
                     szPrelogonDomain,              /* remotename */
                     (unsigned short)UPM_DOMAIN,    /* remotetype */
                     UPM_FL_DOMVER);                /* flags      */

    if (usRc)
        return PRLERR_LOGON;

    return PRLERR_OK;
    }

ULONG APIENTRY DoPreLogoff (VOID)
    {
    USHORT usRc;

    DebugS (D_SAM, "DoPreLogoff entered");

    DebugE (D_SAM, "Logoff for User", szUserid);
    DebugE (D_SAM, "Logoff for Domain", szPrelogonDomain);

    usRc = u32eulgf (szUserid,                      /* userid     */
                     szPrelogonDomain,              /* remotename */
                     UPM_DOMAIN_MAX_FORCE);         /* remotetype */

    DebugULd (D_SAM, "DoPreLogoff", "return value from u32eulgf", usRc);

    if (usRc)
        return PRLERR_LOGOFF;

    return PRLERR_OK;
    }

ULONG APIENTRY DoLogon (PSZ pszUserid, PSZ pszPassword, PSZ pszDomain)
    {
    USHORT usRc;

    WinUpper (WinQueryAnchorBlock (HWND_DESKTOP),   /* password must be UPPERCASE */
              0, 0,
              szPassword);
    usRc = u32eulgn (pszUserid,                     /* userid     */
                     pszPassword,                   /* password   */
                     pszDomain,                     /* remotename */
                     (unsigned short)UPM_DOMAIN,    /* remotetype */
                     UPM_FL_DOMVER);                /* flags      */

    return (ULONG)usRc;
    }

ULONG APIENTRY DoLogoff (PSZ pszUserid, PSZ pszDomain)
    {
    USHORT usRc;

    usRc = u32eulgf (pszUserid,                     /* userid     */
                     pszDomain,                     /* remotename */
                     UPM_DOMAIN_MAX_FORCE);         /* remotetype */

    return (ULONG)usRc;
    }

