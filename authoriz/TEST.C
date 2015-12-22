#define INCL_DOS
#include <os2.h>
#include <stdio.h>
#include "NB_BTB.h"
#define ERR_BUF_LEN 100

main(int argc, char *argv[], char *envp[])
{
    HMODULE hmAuthorize;
    CHAR    chErrorText[ERR_BUF_LEN];
    PFN     pfnAuthorize;
    APIRET  rc;
    OBJDESCR od;

    rc = DosLoadModule(chErrorText, ERR_BUF_LEN, "AUTHORIZ", &hmAuthorize);

    if (rc)
    {
        printf("Load Problem: %s\n", chErrorText);
        return 2;
    } /* endif */

    rc = DosQueryProcAddr(hmAuthorize, 0, "Authorize", &pfnAuthorize);

    if (rc)
    {
        printf("Query Proc Problem: %d\n", rc);
        return 2;
    } /* endif */

    od.szObjectClass  = "<WPDesktop>";
    od.szTitle        = "OS/2 2.0 Arbeitsoberfl„che";
    od.szType         = "Blafasel";
    od.szPhysName     = "OS!2 2.0 A";
    od.szDefaultTitle = "";
 
    (*pfnAuthorize) (&od);

    return DosFreeModule(hmAuthorize);
}
