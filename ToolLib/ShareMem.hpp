/******************************************************************************
*
*  Module Name: ShareMem
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef SHAREMEM_H
#define SHAREMEM_H

#include <IBase.hpp>
#include "SamGlob.h"

/**********************************************/
/* Definitionen fÅr den Shared Memory-Bereich */
/**********************************************/

#define CCHSHARE_INITOS2  65536                         /* Size of shared memory */
#define SHARE_INITOS2    "\\SHAREMEM\\WPSam.InitOS2"    /* Shared-Memory Name   */

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
typedef struct _SHARE
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
    } SHARE;
typedef SHARE *PSHARE;

class ShareMem
    {
    public:
                        ShareMem (void);
                        ~ShareMem (void);

        IBase::Boolean  isAdmin (void);

    private:
        PSHARE          pShareMem;
    };


#endif /* SHAREMEM_H */
