#define INCL_DOSFILEMGR
#define INCL_DOSMISC
#define INCL_DOSPROCESS
#include <os2.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

#define HEADING     "timestamp  tid  pid\n"

void wrtprintf (PCHAR szFormat, ...)
    {
    static CHAR szBuffer[512];
    va_list     parg;
    HFILE       hFile;
    ULONG       ulAction;
    ULONG       ulTime;
    PTIB        ptib;
    PPIB        ppib;

    DosOpen ("$$Sac$$.DBG", &hFile, &ulAction, 0, 0,
        OPEN_ACTION_CREATE_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
        OPEN_ACCESS_WRITEONLY | OPEN_SHARE_DENYNONE,
        NULL);

    if (ulAction == FILE_CREATED)
        DosWrite (hFile, HEADING, strlen (HEADING), &ulAction);
    else
        DosSetFilePtr (hFile, 0, FILE_END, &ulAction);

    DosGetInfoBlocks (&ptib, &ppib);
    DosQuerySysInfo (QSV_TIME_LOW, QSV_TIME_LOW, &ulTime, sizeof (ulTime));

    sprintf (szBuffer, "%.10ld %.4X-%.4X: ", ulTime, ptib->tib_ptib2->tib2_ultid, ppib->pib_ulpid);
    DosWrite (hFile, szBuffer, strlen (szBuffer), &ulAction);

    va_start (parg, szFormat);
    vsprintf (szBuffer, szFormat, parg);
    va_end (parg);

    DosWrite (hFile, szBuffer, strlen (szBuffer), &ulAction);
    DosClose (hFile);

    return;
    }
