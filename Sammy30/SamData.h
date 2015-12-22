#ifndef SAMDATA_H
#define SAMDATA_H

#include "samglob.h"
#include "SamMem.h"
#include "SacAlias.h"

extern HAB      hab;                        // Sammy Anchor Block handle

extern LONG     lGlobalRetCode;             // RÅckgabewert von Sammy

extern PSHARE1  pShareMem;

/** WPSac-spezifische Variable **/
extern CHAR     szPathLoc[CCHMAXPATH];      // lokaler Pfad fÅr INI-Datei, *sac/*drc-Dateien
extern CHAR     szPathLAN[CCHMAXPATH];      // LAN-Pfad fÅr INI-Datei, *sac/*drc-Dateien

extern ALIASDATA strucUserData;
extern ULONG    flMode;                     // Modi entsprechend MAGPRO_FLAG_KEY (magpro2.h)

extern BOOL     bStartShellEnable;
extern HAPP     happShell;

extern HMQ      hmq;
extern HWND     hwndLogin;                  // HWND des Login-Fensters
extern HWND     hwndHelp;                   // Handle vom Helpmanager
#endif /* SAMDATA_H */
