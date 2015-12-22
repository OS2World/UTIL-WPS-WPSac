#define INCL_DOSPROCESS
#include <os2.h>

#include "samglob.h"
#include "SamMem.h"
#include "SacAlias.h"

HAB      hab = 0;                       // Sammy Anchor Block handle

LONG     lGlobalRetCode = 0;            // RÅckgabewert von Sammy

PSHARE1  pShareMem = NULL;

/** WPSac-spezifische Variable **/
CHAR     szPathLoc[CCHMAXPATH];         // lokaler Pfad fÅr INI-Datei, *sac/*drc-Dateien
CHAR     szPathLAN[CCHMAXPATH];         // LAN-Pfad fÅr INI-Datei, *sac/*drc-Dateien

ALIASDATA strucUserData;
ULONG    flMode = 0;                    // Modi entsprechend MAGPRO_FLAG_KEY (magpro2.h)

BOOL     bStartShellEnable = FALSE;
HAPP     happShell = NULLHANDLE;

HMQ      hmq = NULLHANDLE;
HWND     hwndLogin;                     // HWND des Login-Fensters
HWND     hwndHelp = NULLHANDLE;         // Handle vom Helpmanager
