#define INCL_DOS
#include <os2.h>
#include <stdio.h>
#define ERR_BUF_LEN 100

main(int argc, char *argv[], char *envp[])
{
    HMODULE hmAuthorize;
    CHAR    chErrorText[ERR_BUF_LEN];
    PFN     pfnInit;
    APIRET  rc;

    printf("Authorize initializer 1.0\n\n");

    if (argc<2)
    {
        printf("Usage:\n");
        printf("init <groupfile.sac> [<userfile.sac>]\n");
        printf("Where <groupfile.sac> and <userfile.sac> are files created by WP_PARSE.\n");
        return 1;
    } /* endif */

    rc = DosLoadModule(chErrorText, ERR_BUF_LEN, "AUTHORIZ", &hmAuthorize);

    if (rc)
    {
        printf("Load Problem: %s\n", chErrorText);
        return 2;
    } /* endif */

    rc = DosQueryProcAddr(hmAuthorize, 0, "TestInit2", &pfnInit);

    if (rc)
    {
        printf("Query Proc Problem: %d\n", rc);
        return 2;
    } /* endif */

    printf("Program blocks now until WPS claims shared memory segment\n");
    (*pfnInit) (argv[1], argv[2]);

    return DosFreeModule(hmAuthorize);
}
