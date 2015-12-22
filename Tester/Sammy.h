/******************************************************************************
*
*  Module Name: Sammy
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef SAMMY_H
#define SAMMY_H

#include <rexxsaa.h>

#include "samglob.h"

#define CLASSNAME        "Sammy"

#define QUE_NAME         "\\QUEUES\\SAMMY.QUE"

#define EA_MARRY         "SAMMY.MARRY"

/* App/Keynames fÅr Replace von WPObject durch WPSac */
#define INI_REPL_APP     "PM_Workplace:ReplaceList"
#define INI_REPL_KEY     "WPOBJECT"
#define INI_REPL_DATA    "WPSac"

#define TITLELEN         64      /* LÑnge des HELPWINDOWTITLEs */
#define LIBNAMELEN       32      /* LÑnge des HELPLIBRARYNAMEs */

#define APP_PMSHELL      "PMSHELL.EXE"
#define APP_CMD          "CMD.EXE"

/* FehlerrÅckgabecodes von Sammy */
#define SAMERR_OK       0
#define SAMERR_SHMEM    1
#define SAMERR_SEM      2
#define SAMERR_PRFRESET 3

/* Neue Environment-Variablen */
#define ENV_SAMWORKPLACE  "SAMWORKPLACE"    /* Workplaceshell */

/* Titel fÅr MessageBox-Funktion */
#define MBTITLE_ERROR   -1

/* Semaphoren */
#define HEV_SAMMY        "\\SEM32\\WPSam.Sammy"         /* prfReset starten */
#define HEV_PRFRESETLOCK "\\SEM32\\WPSam.LockSammy"     /* prfReset blockieren */

/*****************************************/
/*          Funktionsprototypen          */
/*****************************************/
void             MessageBox (HWND, ULONG, ULONG, ULONG);
BOOL             ChangeWPS (VOID);
HAPP             StartShell (VOID);

#endif /* SAMMY_H */
