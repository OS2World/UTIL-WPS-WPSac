/******************************************************************************
*
*  Module Name: SamMem
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef SAMMEM_H
#define SAMMEM_H

/**********************************************/
/* Definitionen fÅr den Shared Memory-Bereich */
/**********************************************/

#define CCHSHARE_INITOS2  65536                         /* Size of shared memory */
#define SHARE_INITOS2    "\\SHAREMEM\\WPSam.InitOS2"    /* Shared-Memory Name   */

/* Flags fÅr die ulFlag-Variable (WPSam) */
#define OTHERPROTSHELL  0x0001          /* Other ProtShell              */
#define PWDENABLE       0x0002          /* Password enabled             */
#define RENENABLE       0x0004          /* Rename enabled               */
#define MOVEENABLE      0x0008          /* Move enabled                 */
#define COPYENABLE      0x0010          /* Copy enabled                 */
#define DELENABLE       0x0020          /* Delete enabled               */
#define DROPALLOW       0x0040          /* Allow drop into me           */
#define PRIVATESYS      0x0080          /* Private system profile       */
#define WPSDISABLED     0x0100          /* User-WPS disabled            */
#define SETUPPASSED     0x0200          /* wpSetup was running at least one time */
#define ININITOS2MENU   0x0400          /* User is visible in Init-OS/2 menu     */
#define CLOSEAPPL       0x0800          /* Close applications when leaving       */

/* Flags fÅr ulActive in SHARE1 */
#define ACTIVE_INITOS2  0x00000001      /* InitOS2 ist aktiv  */
#define ACTIVE_SAMMY    0x00000002      /* Sammy ist aktiv    */
#define ACTIVE_WPSAM    0x00000004      /* WPSam ist aktiv    */
#define ACTIVE_WPSAC    0x00000008      /* WPSac ist aktiv    */
#define ACTIVE_UPROFILE 0x00000010      /* Uprofile ist aktiv */

/* Flags fÅr den aktuellen Benutzer */
#define USER_FLAG_ADMIN 0x00000001      /* Benutzer ist Administrator */

#define NMAX_WPSTRTUP   10

/* Struktur fÅr Shared-Memory SHARE_INITOS2 */
typedef struct _SHARE1
    {
    CHAR    szRegFile[CCHMAXPATH];          /* Die EAs von RegFile enthalten Reg.String   */

    CHAR    szRootUserIni[CCHMAXPATH];
    CHAR    szRootSystemIni[CCHMAXPATH];
    ULONG   ulFlag_Root;                    /* WPSam-Flags des Rootdesktops               */

    CHAR    szUserIni[CCHMAXPATH];          /* Userprofile fÅr prfReset                   */
    CHAR    szSystemIni[CCHMAXPATH];        /* Systemprofile fÅr prfReset                 */
    ULONG   ulFlag;                         /* WPSam-Flags des startenden Users           */

    ULONG   ulUserFlag;                     /* Flags des aktuellen Benutzers              */
    ULONG   ulActive;                       /* Aktive Programme im System                 */
    HWND    hwndSammy;
    HOBJECT hStartupFldr[NMAX_WPSTRTUP];    /* HOBJECT bereits gestarteter Startup-Folder */
    CHAR    szEnvironment[1];               /* Environment fÅr Shell                      */
    } SHARE1;
typedef SHARE1 *PSHARE1;



PVOID AllocMem (ULONG ulSize);
VOID  FreeMem (PPVOID pMem);
APIRET InitSharedMem (HWND hwnd);

#endif /* SAMMEM_H */
