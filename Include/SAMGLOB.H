/*******************************************************************
   Allgemeine Definitionen f�r SAM, WPSAM und INITOS2
   OS/2 - System Access Manager Version 2.1 - 9.7.1994
 *******************************************************************/
#ifndef _SAMGLOB_
#define _SAMGLOB_

/* Schl�ssel f�r CRYPT */
#define ULKEY1          0xa4877120
#define ULKEY2          0xaf07ec71
#define DOCRYPT                                         /* aktiviert Verschl�sselung */

#include "crypt.h"

/* Stringl�ngen sind incl. '\0' angegeben */
#define CCHMAXNAMELEN       80              /* Stringl�nge f�r User/Password             */
#define CCHMAXDOMAINNAME    32              /* Maximale L�nge des Prelogon-Dom�nennamens */
#define CCHMAXUSERNAME      CCHMAXPATHCOMP  /* Maximale L�nge des  Benutzernamens        */
#define CCHMAXPASSWORD      32              /* Maximale Pa�wortl�nge                     */
#define CCHMAXTRACKDATA     128             /* Maximale L�nge der Trackdaten (ID-Karte)  */
#define CCHMAXREGUSER       32              /* Maximale L�nge des Registrierernamens     */
#define CCHMAXREGPWD        32              /* Maximale L�nge des Registrierkennworts    */
#define CCHMAXMSG           256             /* Maximale Messagel�nge (WinMessageBox)     */
#define CCHMAXRCLINE        4096            /* Maximale Zeilenl�nge im User.RC           */
#define CCHMAXINT           12              /* Maximale Stringl�nge int                  */

/* Definitionen f�r erweiterte Attribute */
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

/* Umgebungs-Variable f�r das f�r die Pfade der INI-Dateien */
#define ENV_USER_INI     "USER_INI"                     /* Environment-Variable */
#define ENV_SYSTEM_INI   "SYSTEM_INI"                   /* Environment-Variable */

#endif /* _SAMGLOB_ */
