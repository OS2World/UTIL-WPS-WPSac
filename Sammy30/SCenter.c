/***************************************************************************
 *
 * PROGRAM NAME: SAMMY.C
 * -------------
 *
 * REVISION LEVEL: 3.0
 * ---------------
 *
 * WHAT THIS PROGRAM DOES:
 * -----------------------
 *  OS/2 Workplace Shell Access Manager:
 *  Umschalten zwischen den Shells fÅr WPSam und WPSac
 *
 * ROUTINES:
 * ---------
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *  IBM C/C++ Set/2 Compiler Version 2.0
 *  IBM OS/2 2.1 Programmers Toolkit
 *
 * REQUIRED FILES:
 * ---------------
 *  SAMMY.C       -   Quelldatei
 *  SAMMY.RC      -   Resource-Datei
 *  SAMMY.DLG     -   Resource-Datei
 *  SAMMY.H       -   Allgemeine Definitionen
 *  SAMMYDLG.H    -   Definitionen fÅr Resourcen
 *  SAMMYPAN.H    -   Definitionen fÅr Hilfe-Panels
 *  SAMGLOB.H     -   Gemeinsame Definitionen fÅr InitOS2, Sammy, WPSam, WPSac
 *
 * REQUIRED LIBRARIES:
 * -------------------
 *  OS2386.LIB    -   OS/2 32-Bit import library
 *  CUTIL.LIB     -   Hilfsroutinen
 *
 * CHANGE LOG:
 * -----------
 *
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *  3.00    11-12-94  Version mit WPSac-UnterstÅtzung
 *
 *  Copyright (C) 1994 Noller & Breining Software
 *
 ******************************************************************************/
#define INCL_DOSMISC
#include <os2.h>
#include <string.h>

#include "SCenter.h"
#include "LANDrive.h"

static const CHAR szDllPath[] = "?:\\OS2\\DLL";

/* Zu sichernde Dateien fÅr die SmartCenter-Konfiguration */
static const PCHAR ppszSCConfigFiles[] =
    {
    "SCENTER.CFG",
    "DOCK*.CFG",
    ""
    };

/*******************************************************************
   RestoreSCenter: Restaurieren der SmartCenter-Konfiguration
   Quellpfad: szUserPath
   Zielpfad:  ?:\os2\dll
   Eingang: szUserPath: Quellpfad
 *******************************************************************/
VOID RestoreSCenter (PSZ szUserPath)
    {
    ULONG   ulBootDrive;
    ULONG   ulIndex;
    CHAR    szSourceDir[CCHMAXPATH];
    CHAR    szDestDir[CCHMAXPATH];

    /* Boot-Laufwerk bestimmen */
    DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof (ULONG));

    /* Zielpfad bestimmen */
    strcpy (szDestDir, szDllPath);
    szDestDir[0] = (CHAR)(ulBootDrive - 1) + 'A';     // Boot-Laufwerk eintragen

    /* Abarbeiten der Konfigurationsdateien */
    strcpy (szSourceDir, szUserPath);
    ulIndex = 0;
    while (*ppszSCConfigFiles[ulIndex] != '\0')
        {
        CopyFiles (szDestDir, szSourceDir, ppszSCConfigFiles[ulIndex]);
        ulIndex++;
        }

    return;
    }

/*******************************************************************
   SaveSCenter: Sichern der SmartCenter-Konfiguration
   Quellpfad: ?:\os2\dll
   Zielpfad:  szUserPath
   Eingang: szUserPath: Zielpfad
 *******************************************************************/
VOID SaveSCenter (PSZ szUserPath)
    {
    ULONG   ulBootDrive;
    ULONG   ulIndex;
    CHAR    szSourceDir[CCHMAXPATH];
    CHAR    szDestDir[CCHMAXPATH];

    /* Boot-Laufwerk bestimmen */
    DosQuerySysInfo (QSV_BOOT_DRIVE, QSV_BOOT_DRIVE, &ulBootDrive, sizeof (ULONG));

    /* Quellpfad bestimmen */
    strcpy (szSourceDir, szDllPath);
    szSourceDir[0] = (CHAR)(ulBootDrive - 1) + 'A';     // Boot-Laufwerk eintragen

    /* Abarbeiten der Konfigurationsdateien */
    strcpy (szDestDir, szUserPath);
    ulIndex = 0;
    while (*ppszSCConfigFiles[ulIndex] != '\0')
        {
        CopyFiles (szDestDir, szSourceDir, ppszSCConfigFiles[ulIndex]);
        ulIndex++;
        }

    return;
    }

