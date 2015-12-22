/*******************************************************************
   Allgemeine Definitionen fÅr SAM, WPSAM und INITOS2
   OS/2 - System Access Manager Version 2.1 - 9.7.1994
 *******************************************************************/
#ifndef _SAMGLOB_
#define _SAMGLOB_

/* SchlÅssel fÅr CRYPT */
#define ULKEY1          0xa4877120
#define ULKEY2          0xaf07ec71
#define DOCRYPT                                         /* aktiviert VerschlÅsselung */

#include "crypt.h"

/* StringlÑngen sind incl. '\0' angegeben */
#define CCHMAXNAMELEN       80              /* StringlÑnge fÅr User/Password             */
#define CCHMAXDOMAINNAME    32              /* Maximale LÑnge des Prelogon-DomÑnennamens */
#define CCHMAXUSERNAME      CCHMAXPATHCOMP  /* Maximale LÑnge des  Benutzernamens        */
#define CCHMAXPASSWORD      32              /* Maximale Pa·wortlÑnge                     */
#define CCHMAXTRACKDATA     128             /* Maximale LÑnge der Trackdaten (ID-Karte)  */
#define CCHMAXREGUSER       32              /* Maximale LÑnge des Registrierernamens     */
#define CCHMAXREGPWD        32              /* Maximale LÑnge des Registrierkennworts    */
#define CCHMAXMSG           256             /* Maximale MessagelÑnge (WinMessageBox)     */
#define CCHMAXRCLINE        4096            /* Maximale ZeilenlÑnge im User.RC           */
#define CCHMAXINT           12              /* Maximale StringlÑnge int                  */

/* Definitionen fÅr erweiterte Attribute */
#define EA_LONGNAME       ".LONGNAME"
#define EA_TYPENAME       ".TYPE"
#define EA_TYPEWPSAM      "WPSam"
#define EA_TYPEWPSAC      "WPSac"
#define EA_PWDNAME        "WPSAM.PASSWORD"
#define EA_USERRESOURCE   "WPSAM.USERRESOURCE"
#define EA_SYSTEMRESOURCE "WPSAM.SYSTEMRESOURCE"
#define EA_USERPROFILE    "WPSAM.USERPROFILE"
#define EA_SYSTEMPROFILE  "WPSAM.SYSTEMPROFILE"
#define EA_ENVNAME        "WPSAM.ENVIRONMENT"
#define EA_REGISTERNAME   "WPSAM.REGISTER"
#define EA_PROTSHELL      "WPSAM.PROTSHELL"
#define EA_FLAGS          "WPSAM.FLAGS"
#define EA_MODIFYDATE     "WPSAM.MODIFYDATE"

#define CCHMAXTYPENAME    max (EA_TYPEWPSAM, EA_TYPEWPSAC)

/* Umgebungs-Variable fÅr das fÅr die Pfade der INI-Dateien */
#define ENV_USER_INI     "USER_INI"                     /* Environment-Variable */
#define ENV_SYSTEM_INI   "SYSTEM_INI"                   /* Environment-Variable */

#endif /* _SAMGLOB_ */
