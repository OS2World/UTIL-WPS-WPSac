#ifndef PREPARE_H
#define PREPARE_H

#include "SacAlias.h"

#define APPNAME     "->History"         /* Applikationsname */

#define DIR_ALL     "\\*"

/* RÅckgabewerte von GetDesktopType */
#define DTTYPE_NONE     0
#define DTTYPE_WPSAC    1
#define DTTYPE_WPSAM    2

/* Definitionen fÅr erweiterte Attribute */
#define CCHMAXEALEN     256             /* Maximale LÑnge eines EAT_ASCII-EAs */

#define USTAT_INITIALIZED   0x0001                  // Benutzer bereits initialisiert
#define USTAT_PRIVATE       0x0002                  // privater Benutzer */

#define TXT_WRONGPASSWORD   "Falsches Kennwort"     // History
#define TXT_SETUP           "Initialisierung"       // History
#define TXT_PRFRESET        "Profil umgeschaltet"   // History
#define TXT_DESKTOP         "ArbeitsoberflÑche_"

ULONG   GetDesktopType (PCHAR pszDesktop);
BOOL    PrepareDT (PSZ szUser, PULONG pulErrId);
BOOL    CreateDTDir (PALIASDATA pAlias, PULONG pulErrId);
APIRET  GetUserPath (PCHAR, PCHAR, ULONG);

#endif /* PREPARE_H */
