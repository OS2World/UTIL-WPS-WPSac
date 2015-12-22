/******************************************************************************
*
*  Module Name: Profile
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef PROFILE_H
#define PROFILE_H

#include <REXXSAA.h>

class Profile
    {
    public:
                    Profile (void);
                    ~Profile (void);
        PSZ         queryProfile (PSZ szPath, ULONG ulLen, BOOL isLocalIni = TRUE);
        HINI        queryProfileHandle (void);
        BOOL        queryProfileSize (PSZ pszApp, PSZ pszKey, PULONG pulc);
        ULONG       queryProfileString (PSZ pszApp, PSZ pszKey, PSZ pszDest, ULONG ulc);
        BOOL        queryProfileData (PSZ pszApp, PSZ pszKey, PVOID pDest, PULONG pulc);
        BOOL        queryProfileCryptedData (PSZ pszApp, PSZ pszKey, PCHAR pDest, ULONG ulc);
        LONG        queryProfileInt (PSZ pszApp, PSZ pszKey, LONG lDefault);
        BOOL        writeProfileString (PSZ pszApp, PSZ pszKey, PSZ pszSource);
        BOOL        writeProfileCryptedData (PSZ pszApp, PSZ pszKey, PSZ pSource, ULONG ulc);

        VOID        readServiceData (PSZ pszUser, PSZ pszService, RXSTRING Argv[]);
        BOOL        readServiceDataString (PSZ pszUser, PSZ pszService, LONG lString, PRXSTRING prxString);
        VOID        writeServiceData (PSZ pszUser, PSZ pszService, RXSTRING Argv[]);
        BOOL        writeServiceDataString (PSZ pszUser, PSZ pszService, LONG lString, PRXSTRING prxString);

        VOID        setPwdAge (PSZ pszUser);
        ULONG       getPwdAge (PSZ pszUser);

        VOID        addToTodoList (PSZ pszUser, PSZ pszString);

    private:
        VOID        setTimestamp (PSZ pszApp);
        HINI        openLANProfile (VOID);
        BOOL        isLANProfile (VOID);

        HINI        hiniAlias;

    };

#endif /* PROFILE_H */
