#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "samglob.h"
#include "magpro2.h"
#include "Profile.hpp"

/* Key fÅr die VerschlÅsselung */
ULONG key[2] = {ULKEY1, ULKEY2};

int main (int argc, char *argv[])
    {
    CHAR  sz[CCHMAXPASSWORD];
    ULONG ulc;

    if (argc != 2)
        {
        printf ("Syntax: %s <username>\n", argv[0]);
        return 0;
        }

    Profile profile;
    memset (sz, 0, CCHMAXPASSWORD);

    printf ("--- Encrypting password ---\n");

    ulc = CCHMAXPASSWORD;
    profile.queryProfileData (argv[1], MAGPRO_PASSWORD_KEY, sz, &ulc);
    if (sz[ulc-1] != '\0')
        sz[ulc] = '\0';

    printf ("actual password: %s\n", sz);

    profile.writeProfileCryptedData (argv[1], MAGPRO_PASSWORD_KEY, sz, CCHMAXPASSWORD);

    printf ("--- Encrypting service data string ---\n");
    if (profile.queryProfileSize (argv[1], NULL, &ulc) == FALSE)
        return 0;

    PCHAR psz        = (PCHAR)malloc (ulc);
    PCHAR pszService = psz;
    profile.queryProfileData (argv[1], NULL, pszService, &ulc);

    for (int i = 0; i < MAXSTRING; i++)
        {
        if (strncmp (pszService, MAGPRO_SERVICE_KEY, 2) == 0)
            {
            // Service string gefunden; verschlÅsseln
            profile.queryProfileSize (argv[1], pszService, &ulc);
            ulc =(((ulc + 7)>>3)<<3);
            if (ulc)
                {
                printf ("Encrypting %s\n", pszService);
                ULONG ulc1;
                PCHAR pszServiceString = (PCHAR)malloc (ulc);
                memset (pszServiceString, 0, ulc);

                profile.queryProfileData (argv[1], pszService, pszServiceString, &ulc1);
                profile.writeProfileCryptedData (argv[1], pszService, pszServiceString, ulc);

                free (pszServiceString);
                }
            }
        pszService += strlen (pszService) + 1;
        }

    return 0;
    }
