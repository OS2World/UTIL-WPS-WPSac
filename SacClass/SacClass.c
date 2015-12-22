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

HAB     hab;

void Anleitung (char *string)
    {
    printf ("Syntax: %s [REGISTER|REPLACE|DEREGISTER|DEREPLACE]\n\n", string);
    printf ("        REPLACE    includes REGISTER\n");
    printf ("        DEREGISTER includes DEREPLACE\n");
    return;
    }

int main (int argc, char *argv[])
    {
    ULONG ulReg;
    BOOL  bRC;

    if (argc == 1)
        {
        Anleitung (argv[0]);
        return 1;
        }

    if (strcmp (strupr (argv[1]), "REPLACE") == 0)
        ulReg = 1;
    else if (strcmp (strupr (argv[1]), "DEREPLACE") == 0)
        ulReg = 2;
    else if (strcmp (strupr (argv[1]), "REGISTER") == 0)
        ulReg = 3;
    else if (strcmp (strupr (argv[1]), "DEREGISTER") == 0)
        ulReg = 4;
    else
        {
        Anleitung (argv[0]);
        return 1;
        }

    hab = WinInitialize(0);

    switch (ulReg)
        {
        case 2:
            bRC = WinReplaceObjectClass ("WPObject", "WPSac", FALSE);
            break;

        case 4:
            WinReplaceObjectClass ("WPObject", "WPSac", FALSE);
            bRC = WinDeregisterObjectClass ("WPSac");
            break;

        case 3:
            bRC = WinRegisterObjectClass ("WPSac", "wpsac");
            break;

        case 1:
            WinRegisterObjectClass ("WPSac", "wpsac");
            bRC = WinReplaceObjectClass ("WPObject", "WPSac", TRUE);
            break;
        }

    WinAlarm (HWND_DESKTOP, bRC ? WA_NOTE : WA_ERROR);
    WinTerminate(hab);

    return bRC ? 0 : 1;
    }
