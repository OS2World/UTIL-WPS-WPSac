/******************************************************************************
*
*  Module Name: Sammy
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef LANDRIVE_H
#define LANDRIVE_H

VOID  MountLANDirectory (VOID);
VOID  Wait4LANDirectory (VOID);
ULONG CopyLANDir (PSZ pszDst, PSZ pszSrc);
VOID  UnmountLANDirectory (VOID);
BOOL  CopyFiles (PSZ pszDst, PSZ pszSrc, PSZ pszFiles);

#endif /* LANDRIVE_H */
