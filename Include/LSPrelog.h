#ifndef LSPRELOG_H
#define LSPRELOG_H

#define SIGNATURE_LS        "LS:"   /* Prelogon-Signatur fÅr LAN Server */

/* Fehlercodes */
#define PRLERR_OK               0
#define PRLERR_INVDOMAIN        1
#define PRLERR_INVPATH          2
#define PRLERR_NOPRELOGUSER     3
#define PRLERR_INVPASSWORD      4
#define PRLERR_LOGON            5
#define PRLERR_LOGOFF           6

/* Funktionsprototypen */
ULONG APIENTRY DoPreLogon (VOID);
ULONG APIENTRY DoPreLogoff (VOID);
ULONG APIENTRY DoLogon (PSZ pszUserid, PSZ pszPassword, PSZ pszDomain);
ULONG APIENTRY DoLogoff (PSZ pszUserid, PSZ pszDomain);

#endif /* LSPRELOG_H */
