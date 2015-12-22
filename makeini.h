/******************************************************************************
*
*  Module Name: MakeIni
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef MAKEINI_H
#define MAKEINI_H

/* šbergabeparameter an und Erfolgsmeldung von createIni */
typedef struct _USER
    {
    PCHAR       pszUserName;                /* User-Name                       */
    PCHAR       pszUserPath;                /* User-Name incl. Pfad            */
    PCHAR       pszUserResource;            /* Resource-File                   */
    PCHAR       pszUserProfile;             /* UserProfile                     */
    PCHAR       pszSystemResource;          /* Resource-File                   */
    PCHAR       pszSystemProfile;           /* SystemProfile                   */
    PCHAR       pszDesktopName;             /* Name des Desktop-Verzeichnisses */
    RESULTCODES rcSuccess;
    ULONG       ulErrorMsg;                 /* Message bei Fehler, sonst 0     */
    } USER;
typedef USER *PUSER;

/* Fehlercodes von createIni */
#define ERR_NOUSERRESOURCE      1           /* => IDS_NOUSERRESOURCE     */
#define ERR_NOSYSTEMRESOURCE    2           /* => IDS_NOSYSTEMRESOURCE   */
#define ERR_NOUSERINICREATED    3           /* => IDS_NOUSERINICREATED   */
#define ERR_NOSYSTEMINICREATED  4           /* => IDS_NOSYSTEMINICREATED */
#define ERR_NODESKINRC          5           /* => IDS_NODESKINRC         */
#define ERR_PATHTOOLONG         6           /* => IDS_PATHTOOLONG        */



VOID createIni (PUSER pUser);

#endif /* MAKEINI_H */
