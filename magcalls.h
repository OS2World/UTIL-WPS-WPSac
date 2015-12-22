/************************************************************************
 *
 * FILENAME: MAGCALLS.H
 * ---------
 *
 * VERSION: 3.00.00
 * --------
 *
 * DESCRIPTION:
 * ------------
 *   C-Include file for the interface between MAGCALLS.DLL and
 *   application program. Is necessary for application development.
 *   Enhanced track support has been added to address more than
 *   three tracks.
 *
 * COMPILE REQUIREMENTS:
 * ---------------------
 *   IBM Visual Age C++ C 6.0 rsp. IBM Visual Age C++ for OS/2
 *   OS/2 Linker LINK.EXE rsp. ILINK.EXE
 *
 * NEEDED LIBRARIES:
 * -----------------
 *   MAGCALLS.LIB - Import-Library
 *
 *  Copyright (C) noller & breining software 1993...1998
 *
 ***************************************************************************/
/***************************************************************************
 *
 * Conversion to INC-Datei:
 *    h2inc -c -d -m -o magcalls.inc magcalls.h
 *
 ***************************************************************************/

#ifndef __magcalls_h
  #define __magcalls_h

#ifndef __32BIT__
#define APIRET16    USHORT
#define APIENTRY16  APIENTRY
#define _Seg16      far
#endif

/*@***************************************************************************/
/*@     common definitions                                                   */
/*@***************************************************************************/
#define MODE_RETURN_ALL 0
#define MODE_RETURN_ONE 1

#define SLEEP_INTERVAL 100              /*@ Time in ms for wait-interval for data */
#define ABORT_INTERVAL 250              /*@ Time in ms fr abort-acknowledge      */

/* Configuration status */
#define CFG_PARMERR     0x8000          /* wrong parameter in CONFIG.SYS */
#define CFG_NOHARDW     0x4000          /* hardware not found */
#define CFG_PONERRS     0x2000          /* power-on errors */
#define CFG_DDERROR     0x1000          /* device-driver not loaded */
#define CFG_READT1      0x0040          /* track 1 readable */
#define CFG_READT2      0x0020          /* track 2 readable */
#define CFG_READT3      0x0010          /* track 3 readable */
#define CFG_ENCT1       0x0004          /* track 1 writeable */
#define CFG_ENCT2       0x0002          /* track 2 writeable */
#define CFG_ENCT3       0x0001          /* track 3 writeable */
#define CFGE_READT1     0x0100          /* track 1 is readable  */
#define CFGE_READT2     0x0200          /* track 2 is readable  */
#define CFGE_READT3     0x0400          /* track 3 is readable  */
#define CFGE_READT4     0x0800          /* track 4 is readable  */
#define CFGE_READT5     0x1000          /* track 5 is readable  */
#define CFGE_READT6     0x2000          /* track 6 is readable  */
#define CFGE_ENCT1      0x0001          /* track 1 is writeable */
#define CFGE_ENCT2      0x0002          /* track 2 is writeable */
#define CFGE_ENCT3      0x0004          /* track 3 is writeable */
#define CFGE_ENCT4      0x0008          /* track 4 is writeable */
#define CFGE_ENCT5      0x0010          /* track 5 is writeable */
#define CFGE_ENCT6      0x0020          /* track 6 is writeable */

typedef SHANDLE HMAG;
typedef HMAG *PHMAG;

typedef USHORT OPMODE;

#pragma pack(2)
typedef struct _TRACKPARM
    {
    BYTE bParity;
    BYTE bCharDef;
    BYTE bPSOMChar;
    BYTE bASOMChar;
    BYTE bPEOMChar;
    BYTE bAEOMChar;
    BYTE bFormCtl;
    BYTE bZeroNum;
    } TRACKPARM;
typedef TRACKPARM *PTRACKPARM;

typedef struct _TRACKPARMS
    {
    TRACKPARM trk1prm;
    TRACKPARM trk2prm;
    TRACKPARM trk3prm;
    } TRACKPARMS;
typedef TRACKPARMS *PTRACKPARMS;

/* Track config status for enhanced track support */
typedef struct _ENHCONFIGSTAT
    {
    USHORT usDriverStatus;              /* definitions see CFG_* values >= 0x100 */
    USHORT usTrackStatus;               /* definitions see CFGE_* values         */
    } ENHCONFIGSTAT;
typedef ENHCONFIGSTAT *PENHCONFIGSTAT;

#pragma pack()

/*@***************************************************************************/
/*@     MAGCALLS internal definitions                                        */
/*@***************************************************************************/
/*@ Werte fr _ulFlag und ulFlag */
#define CLASSNAME       "WPMag"         /*@ Klassenname */

#define OPEN_ABOUT      OPEN_USER+1     /*@ View Id: Produktinformation */
#define OPEN_DEBUG      OPEN_USER+2     /*@ View Id: Debug-Panel        */

#define INI_APPNAME     CLASSNAME       /*@ Application-name fr INI-Datei */
#define INI_KEY_FLAGS   "Flags"         /*@ Key fr _ulFlag       */
#define INI_KEY_TIMEOUT "Timeout"       /*@ Key fr _ulRdTimeout  */
#define INI_KEY_WRMODE  "WriteMode"     /*@ Key fr _bWriteMode   */
#define INI_KEY_MAP     "TrackMap"      /*@ Key fr _bMapArray    */
#define FLAG_IS3        0x80000000      /*@ 173/174 */

#define DEFAULT_WRMODE  TRUE            /*@ Default bWriteMode    */
#define DEFAULT_FLAG    0               /*@ Default ulFlag        */
#define DEFAULT_TIMEOUT 1               /*@ Default ulRdTimeout   */
#define DEFAULT_MAP     "0 1 2"         /*@ Default bMapArray     */
#define MAX_TIMEOUT     5               /*@ Max von ulRdTimeout   */


/*@***************************************************************************/
/*@     definitions of interface to device driver                            */
/*@***************************************************************************/
#define UNITNAME        "MAGCOM$"       /*@*/
#define IOCTL_MAGCRD    0x8F            /*@ IOCtl category */
/*@ function codes:
 *@ MAGCRD_STATUS:    Parm = REQ_*
 *@                   Data = &bytes: structure STAT_DATA
 *@ MAGCRD_STATUS2:   Parm = REQ_PRM_*
 *@                   Data = &bytes: structure STAT_DATA2
 *@ MAGCRD_READ:      Parm = &struc: structure RD_PARM
 *@                   Data = &bytes: data buffer
 *@ MAGCRD_WRITE:     Parm = &struc: structure RD_PARM
 *@                   Data = &bytes: data buffer
 *@ MAGCRD_CONFIG:    Parm = NULL
 *@                   Data = &struc: structure TRACKPARMS
 *@ MAGCRD_CONFIG2:   Parm = REQ_PRM_*
 *@                   Data = &struc: structure TRACKPARM
 *@*/
#define MAGCRD_STATUS       0x60        /*@ OLD */
#define MAGCRD_READ         0x61        /*@*/
#define MAGCRD_STATUS2      0x62        /*@*/
#define MAGCRD_CONFIG       0x40        /*@ OLD */
#define MAGCRD_WRITE        0x41        /*@*/
#define MAGCRD_CONFIG2      0x42        /*@*/

/*@ Werte fr Parameterbyte in MAGCRD_STATUS2 und MAGCRD_READ: 'Typ der Statusanfrage' */
/*@ Ist REQ_PRM_AVAIL nicht gesetzt, wird der Lesevorgang gestoppt                     */
#define REQ_PRM_TRACK1      0x01        /*@ read request for track 1             (R/C) */
#define REQ_PRM_TRACK2      0x02        /*@ read request for track 2           (S/R/C) */
#define REQ_PRM_TRACK3      0x03        /*@ read request for track 3           (S/R/C) */
#define REQ_PRM_TRACK4      0x04        /*@ read request for track 4             (R/C) */
#define REQ_PRM_TRACK5      0x05        /*@ read request for track 5             (R/C) */
#define REQ_PRM_TRACK6      0x06        /*@ read request for track 6             (R/C) */
#define REQ_PRM_POLL        0x08        /*@ status request for data and        (S/R/C) */
                                        /*@ switch to track 2/3 if old HW              */
#define REQ_PRM_WRITE       0x20        /*@ write still active?                (S)     */
#define REQ_PRM_AVAIL       0x40        /*@ track contains data?               (S)     */
#define REQ_PRM_DEVCAP      0x80        /*@ request for STAT_DATA.strucConfig  (S)     */

/*@ Werte fr Byte bAvail in MAGCRD_STATUS2 */
#define REQ_DAT_TRACK1      0x01        /*@ read request for track 1           (S)   */
#define REQ_DAT_TRACK2      0x02        /*@ read request for track 2           (S)   */
#define REQ_DAT_TRACK3      0x04        /*@ read request for track 3           (S)   */
#define REQ_DAT_TRACK4      0x08        /*@ read request for track 4           (S)   */
#define REQ_DAT_TRACK5      0x10        /*@ read request for track 5           (S)   */
#define REQ_DAT_TRACK6      0x20        /*@ read request for track 6           (S)   */
#define REQ_DAT_WR_PEND     0x80        /*@ write action pending ?             (S)   */

#define BUFFSIZE    512                 /*@ buffersize of 1 Track */

#pragma pack(1)                         /*@*/
typedef struct _STAT_DATA               /*@ OLD */
    {                                   /*@*/
    BYTE bAvail;                        /*@ data read; see RDTRK_* below */
    USHORT usConfig;                    /*@ device configuration */
    } STAT_DATA;                        /*@*/

typedef struct _STAT_DATA2              /*@*/
    {                                   /*@*/
    BYTE          bAvail;               /*@ data read; see REQ_DAT_* above */
    ENHCONFIGSTAT strucConfig;          /*@ device configuration */
    } STAT_DATA2;                       /*@*/

typedef struct _RD_PARM                 /*@*/
    {                                   /*@*/
    BYTE   bTrack;                      /*@ tracknumber: REQ_DAT_TRACKx + REQ_DAT_POLL*/
    USHORT usLen;                       /*@ Input: bufferlength; Output: byte in buffer */
    USHORT usErr;                       /*@ errorcode (MAGERR_*) */
    } RD_PARM;                          /*@*/

#pragma pack()                          /*@*/

/*@***************************************************************************/
/*@     definitions of interface to application program                      */
/*@***************************************************************************/
/* ReadTracks/EncodeTracks word in MagReadData */
#define RDTRK_WR_PEND   0x20            /*@ OLD write action */
#define RDTRK_MULTI     0x08            /*@ OLD function call writes >1 tracks */
#define RDTRK_TRACK1    0x04            /* function call reads track 1 */
#define RDTRK_TRACK2    0x02            /* function call reads track 2 */
#define RDTRK_TRACK3    0x01            /* function call reads track 3 */

/* ReadTracks/EncodeTracks word in MagEnhReadData */
#define RDTRK_EMULTI    0x80            /*@ function call writes >1 tracks */
#define RDTRK_ETRACK1   0x01            /* function call reads track 1 */
#define RDTRK_ETRACK2   0x02            /* function call reads track 2 */
#define RDTRK_ETRACK3   0x04            /* function call reads track 3 */
#define RDTRK_ETRACK4   0x08            /* function call reads track 4 */
#define RDTRK_ETRACK5   0x10            /* function call reads track 5 */
#define RDTRK_ETRACK6   0x20            /* function call reads track 6 */

#define SVAL_BLANK      0x80            /* track is empty, no SOM found */
#define SVAL_READERR    0x40            /* read error encountered */
#define SVAL_TRACK1     0x04            /* data of track 1 */
#define SVAL_TRACK2     0x02            /* data of track 2 */
#define SVAL_TRACK3     0x01            /* data of track 3 */

/* Comment the following lines for H2INC */
#if (RDTRK_TRACK1!=SVAL_TRACK1)
    (RDTRK_TRACK2!=SVAL_TRACK2)
    (RDTRK_TRACK3!=SVAL_TRACK3)
#   error RDTRK_value does not match SVAL_value
#endif

#pragma pack(2)

/* READ DATA format */
typedef struct _READDATA
    {
    BYTE LVal;                          /* Record length */
    BYTE SVal;                          /* Status field */
    BYTE bBuffer;                       /* Data Area */
    } READDATA;
typedef READDATA *PREADDATA;

/* ENCODE DATA format */
typedef struct _WRITEDATA
    {
    BYTE LVal;                          /* Record length */
    BYTE TVal;                          /* Track Number */
    BYTE bBuffer;                       /* Data Area */
    } WRITEDATA;
typedef WRITEDATA *PWRITEDATA;

/* ERRORSTAT structure */
typedef struct _ERRORSTAT
    {
    BYTE bcrTrk1;                       /* Track-1 read error count */
    BYTE bcrTrk2;                       /* Track-2 read error count */
    BYTE bcrTrk3;                       /* Track-3 read error count */
    BYTE bceTrk1;                       /* Track-1 encode error count */
    BYTE bceTrk2;                       /* Track-2 encode error count */
    BYTE bceTrk3;                       /* Track-3 encode error count */
    } ERRORSTAT;
typedef ERRORSTAT *PERRORSTAT;

/* Track error status for enhanced track support */
typedef struct _TRACKERRORSSTAT
    {
    BYTE bcrTrack;                      /* Track read error count   */
    BYTE bceTrack;                      /* Track encode error count */
    } TRACKERRORSSTAT;
typedef TRACKERRORSSTAT *PTRACKERRORSSTAT;

#pragma pack ()

/*@***************************************************************************/
/*@     function prototypes                                                  */
/*@***************************************************************************/
/* standard functionprototypes */
APIRET16 APIENTRY16 MagOpen (PHMAG _Seg16 phMag);
APIRET16 APIENTRY16 MagClose (HMAG hMag);
APIRET16 APIENTRY16 MagLoadDevParms (HMAG hMag, PTRACKPARMS _Seg16 pTrackParms);
APIRET16 APIENTRY16 MagSetOperationMode (HMAG hMag, OPMODE opMode);
APIRET16 APIENTRY16 MagEncodeData (HMAG hMag, USHORT usLen, PBYTE _Seg16 pBuffer);
APIRET16 APIENTRY16 MagResetDevice (HMAG hMag);
APIRET16 APIENTRY16 MagAbort (HMAG hMag);
APIRET16 APIENTRY16 MagReadDevParms (HMAG hMag, PTRACKPARMS _Seg16 pTrackParms, PUSHORT _Seg16 pusSize);
APIRET16 APIENTRY16 MagReadErrorStats (HMAG hMag, PERRORSTAT _Seg16 pBuffer, PUSHORT _Seg16 pusSize);
APIRET16 APIENTRY16 MagReadConfigStatus (HMAG hMag, PUSHORT _Seg16 pusConfig);
APIRET16 APIENTRY16 MagReadData (HMAG hMag, USHORT usTrack, PBYTE _Seg16 pBuffer, PUSHORT _Seg16 pusSize);

/* enhanced track support */
APIRET16 APIENTRY16 MagLoadTrackParms (HMAG hMag, USHORT usTrack, PTRACKPARM _Seg16 pTrackParm);
APIRET16 APIENTRY16 MagReadTrackParms (HMAG hMag, USHORT usTrack, PTRACKPARM _Seg16 pTrackParm);
APIRET16 APIENTRY16 MagReadTrackErrorStats (HMAG hMag, USHORT usTrack, PTRACKERRORSSTAT _Seg16 pBuffer);
APIRET16 APIENTRY16 MagEnhReadConfigStatus (HMAG hMag, PENHCONFIGSTAT _Seg16 pBuffer);
APIRET16 APIENTRY16 MagEnhEncodeData (HMAG hMag, USHORT usLen, PBYTE _Seg16 pBuffer);
APIRET16 APIENTRY16 MagEnhReadData (HMAG hMag, USHORT usTrack, PBYTE _Seg16 pBuffer, PUSHORT _Seg16 pusSize);
APIRET16 APIENTRY16 MagSetMapping (CHAR cTrack1, CHAR cTrack2, CHAR cTrack3);

/*@ internal functions */
VOID saveClassData (VOID);              /*@*/
VOID loadClassData (VOID);              /*@*/
VOID saveParms (VOID);                  /*@*/
VOID loadParms (VOID);                  /*@*/

/* error codes */
#define MAGERR_OK           0       /* no error */
#define MAGERR_NOSOM        1       /*@ internal use */
#define MAGERR_NOEOM        2       /*@ internal use */
#define MAGERR_OPENERR      110     /* error during open */     // OPEN
#define MAGERR_NOTATTCH     1538    /* no hardware */           // ENCODE,RESET,READ
#define MAGERR_HARDERR      1539    /* hardware fault */        // ENCODE,READ
#define MAGERR_HINVALID     1540    /* invalid handle */        // CLOSE,LOAD,MODE,ENCODE,RESET,ABORT,RMODE,ERROR,CONFIG,READ
#define MAGERR_NOTAVL       1541    /* not available */         // LOAD,MODE,ENCODE,RESET,READ
#define MAGERR_RDTRACK      1542    /* invalid track */         // READ
#define MAGERR_WRTRACK      1543    /* invalid track */         // ENCODE
#define MAGERR_ERRENC       1544    /* data error */            // ENCODE
#define MAGERR_T1PINV       1545    /* Trk1 prm invalid */      // LOAD
#define MAGERR_T2PINV       1546    /* Trk2 prm invalid */      // LOAD
#define MAGERR_T3PINV       1547    /* Trk3 prm invalid */      // LOAD
#define MAGERR_T4PINV       1560    /* Trk4 prm invalid */      // LOAD (non standard extension)
#define MAGERR_T5PINV       1561    /* Trk5 prm invalid */      // LOAD (non standard extension)
#define MAGERR_T6PINV       1562    /* Trk6 prm invalid */      // LOAD (non standard extension)
#define MAGERR_PARM         1563    /* invalid parameter */     // extensions (non standard extension)
#define MAGERR_ISOPEN       1564    /* device already open */   // SETMAPPING
#define MAGERR_BUFFSIZ      1548    /* buffer too small */      // ENCODE,RMODE,ERROR,READ
#define MAGERR_ERRTEST      1549    /* error self test */       // RESET
#define MAGERR_ENDEDAPP     1550    /* end caused by appl */    // ENCODE,READ
#define MAGERR_ENDEDDLL     1551    /* end caused by DLL */     // OPEN,ENCODE,RESET,ABORT,READ

#endif /* ifndef __magcalls_h */

