#ifndef SACALIAS_H
#define SACALIAS_H

/* Benutzerstatus */
#define STAT_PASSWORDOK     0                       // Kennwort ok.
#define STAT_NOPASSWORD     1                       // kein Kennwort benîtigt
#define STAT_ROOTUSER       2                       // Default-Benutzer starten
#define STAT_NOUSERFOUND    3                       // Kein Benutzer gefunden
#define STAT_USERNOTFOUND   4                       // Benutzer nicht gefunden
#define STAT_WRONGPASSWD    5                       // Kennwort falsch

typedef struct _ALIASDATA
    {
    ULONG  ulFlag;                          /* Flags             */
    CHAR   szUser[CCHMAXPATHCOMP];          /* User              */
    CHAR   szPassword[CCHMAXPASSWORD];      /* Pa·wort           */
    CHAR   szID[CCHMAXTRACKDATA];           /* Karten-ID         */
    CHAR   szDesktop[CCHMAXUSERNAME];       /* Desktop-Container */
    CHAR   szSacUser[CCHMAXPATH];           /* Parser-File       */
    CHAR   szSacGroup[CCHMAXPATH];          /* User-Gruppe       */
    BOOL   bIsAdmin;                        /* Administratorflag */
    } ALIASDATA;
typedef ALIASDATA *PALIASDATA;

/* ALIASDATA.ulFlag: gÅltige Strings */
#define ALIAS_USER      0x0001              /* szUser                 */
#define ALIAS_PASSWORD  0x0002              /* szPassword             */
#define ALIAS_ID        0x0004              /* szID                   */
#define ALIAS_DESKTOP   0x0008              /* szDesktop              */
#define ALIAS_SAC_USER  0x0010              /* szSacUser              */
#define ALIAS_SAC_GROUP 0x0020              /* szSacGroup             */
#define ALIAS_WPSAC     0x8000              /* action caused by WPSac */



VOID  QueryAlias (LONG lUser, PSZ pszUser);
ULONG AliasFind (PALIASDATA pLoginData);
VOID  AliasSaveNewPassword (PSZ pszUser, PSZ pszPassword);

#endif /* SACALIAS_H */
