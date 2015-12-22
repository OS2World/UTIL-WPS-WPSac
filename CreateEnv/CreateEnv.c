/*
 *
 *   Module Name: MAGclass.c
 *
 *   Register Class of OS/2 Workplace Shell Access Control
 *
 *   Return 0: ok | 1: Fehler
 */

#define INCL_WIN
#define INCL_PM
#define INCL_DOS

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

HAB hab;

void Anleitung (char *string)
    {
    printf ("Syntax: %s <profile_path>\n\n", string);
    return;
    }

int main (int argc, char *argv[])
    {
    HINI  hini;
    BOOL  bRC;
    PSZ   psz, pszBuffer;

    if (argc == 1)
        {
        Anleitung (argv[0]);
        return 1;
        }

    hab = WinQueryAnchorBlock (HWND_DESKTOP);

    hini = PrfOpenProfile (hab, argv[1]);
    if (hini == NULLHANDLE)
        {
        printf ("Error opening file %s\n", argv[1]);
        return 1;
        }

    pszBuffer = psz = malloc (65536);

    while (!feof (stdin))
        {
        fgets (psz, 65536-strlen (pszBuffer), stdin);
        psz += strlen (psz);
        }

    PrfWriteProfileString (hini, "_Env", "NewEnv", pszBuffer);
    PrfCloseProfile (hini);

    free (pszBuffer);

    return 0;
    }
