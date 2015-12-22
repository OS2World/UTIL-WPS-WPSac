/******************************************************************************
*
*  Module Name: MagSupp
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef MAGSUPP_H
#define MAGSUPP_H

/* Kartenleserunterst�tzung */
typedef struct _MAGNET
    {
    HWND      hwnd;                         /* Window Handle                      */
    ULONG     ulNotification;               /* WM_USER-Wert f�r Read-Notification */
    USHORT    usDID;                        /* aktives Entry-Feld                 */
    PCHAR     pszReadData;                  /* Puffer f�r (konvertierten) String  */
    } MAGNET;

typedef MAGNET *PMAGNET;



BOOL MCInit (void);
BOOL MCFree (void);
BOOL MCOpen (void);
BOOL MCClose (void);
BOOL MCAbort (void);
BOOL MCStart (PMAGNET);
BOOL MCRead (PMAGNET);
void _Optlink thMCRead (void *);

#endif /* MAGSUPP_H */
