#define INCL_DOS
#include <OS2.H>
#include "WP_Parse.h"
#include "NB_BTB.h"
#include <string.h>

#define SHAREDMEMNAME "\\SHAREMEM\\AUTHORIZ"
#define SEMAPHORENAME "\\SEM32\\AUTHORIZ"
#define APPNAME       "AUTHORIZE"
#define LOCALPATH     "SAMLOCALPATH"
#define REMOTEPATH    "SAMREMOTEPATH"

#define UNLOCK        0
#define LOCK          1
#define DRIVELOCK     0x40

#define PARMBLOCKSIZE 2

#define GROUPFILE_OK  1
#define USERFILE_OK   2
#define NO_GROUPFILE  4
#define NO_USERFILE   8

static BYTE *pbLockMem = NULL;
static PVOID pvShMem = NULL;

BOOL Unlock(void);
BOOL Lock(void);

BOOL wildcomp(BYTE *pbPattern, BYTE *pbSample, USHORT usPatternLen)
{
    do
    {
        if (*pbPattern == '*')
        {
            while (*pbPattern == '*')
                { pbPattern++; usPatternLen--; }
            if (*pbPattern == '\0')
                return TRUE;

            for (;;)
            {
                while ( (*pbSample) && (*pbSample != *pbPattern) )
                    pbSample++;
                if (*pbSample == '\0')
                    return FALSE;
                if (wildcomp(pbPattern, pbSample, usPatternLen))
                    return TRUE;
                pbSample++;
            }
        }
        else if ((*pbPattern == '?') || (*pbPattern == *pbSample))
        {
            pbPattern++;
            pbSample++;
            usPatternLen--;
        }
        else
            return FALSE;
    }
    while ((*pbPattern != '\0') || (*pbSample != '\0'));

    return TRUE;
}

BOOL StringCheck(AccessObject *pao, OBJDESCR *objdescr)
{
    if (pao->usClassLen > 0)
    {
        if (!wildcomp( ((BYTE *) pao) + pao->usClassOffset, objdescr->szObjectClass, pao->usClassLen))
            return FALSE;
    }

    if (pao->usTitleLen > 0)
    {
        if (!wildcomp( ((BYTE *) pao) + pao->usTitleOffset, objdescr->szTitle, pao->usTitleLen))
            return FALSE;
    }

    if (pao->usTypeLen > 0)
    {
        if (!wildcomp( ((BYTE *) pao) + pao->usTypeOffset, objdescr->szType, pao->usTypeLen))
            return FALSE;
    }

    if (pao->usPhysNameLen > 0)
    {
        if (!wildcomp( ((BYTE *) pao) + pao->usPhysNameOffset, objdescr->szPhysName, pao->usPhysNameLen))
            return FALSE;
    }

    if (pao->usDefTitleLen > 0)
    {
        if (!wildcomp( ((BYTE *) pao) + pao->usDefTitleOffset, objdescr->szDefaultTitle, pao->usDefTitleLen))
            return FALSE;
    }
    return TRUE;
}


BOOL Authorize(OBJDESCR *objdescr)
{
    AccessObject *pao;
    HEV    hevWait = 0;
    APIRET rc;

    objdescr->ulAND_CTXT  = 0xFFFFFFFF;
    objdescr->ulOR_CTXT   = 0;
    objdescr->ulAND_STYLE = 0xFFFFFFFF;
    objdescr->ulOR_STYLE  = 0;
    objdescr->ulMisc      = DONT_ASK_ANY_MORE;

    if (pvShMem == NULL)
    {
        rc = DosGetNamedSharedMem(&pvShMem, SHAREDMEMNAME, PAG_READ | PAG_WRITE);
        if (rc)
            goto Fail;
        rc = DosOpenEventSem(SEMAPHORENAME, &hevWait);
        if (rc == 0)
        {
            DosPostEventSem(hevWait);
            DosCloseEventSem(hevWait);
        }
    } /* endif */

    pao = (AccessObject *) pvShMem;

    while (pao->usType != 0)
    {
        if (StringCheck (pao, objdescr))
        {
            objdescr->ulAND_CTXT  = pao->ulContext_or |
                (objdescr->ulAND_CTXT & pao->ulContext_and) ;
            objdescr->ulOR_CTXT   = pao->ulContext_or |
                (objdescr->ulOR_CTXT  & pao->ulContext_and) ;
            objdescr->ulAND_STYLE = pao->ulStyle_or  |
                (objdescr->ulAND_STYLE & pao->ulStyle_and) ;
            objdescr->ulOR_STYLE  = pao->ulStyle_or  |
                (objdescr->ulOR_STYLE  & pao->ulStyle_and) ;
            objdescr->ulMisc      = pao->ulMisc_or  |
                (objdescr->ulMisc      & pao->ulMisc_and) ;
        }

        pao = (AccessObject *)(((BYTE *) pao) + pao->usLen);
    } /* endwhile */

    return TRUE;

Fail:
/* Don't know what to do */
    return FALSE;
}

/******************************************************************************/

BOOL Init(PCHAR szFileName)
{
    HFILE  hfProfile;
    ULONG  ulAction;
    ULONG  ulFileSize, ulDummy;
    PBYTE  pbShMem;
    APIRET rc;

    pvShMem = NULL;

    if (szFileName[0] == '\0')
    { /* Blank all Settings */
        rc = DosGetNamedSharedMem((PVOID *)&pbShMem, SHAREDMEMNAME, PAG_READ | PAG_WRITE);
        if (rc != 0)
        {
            rc = DosAllocSharedMem((PVOID *)&pbShMem, SHAREDMEMNAME, 2,
                                    PAG_COMMIT | PAG_READ | PAG_WRITE);
            if (rc)
               return FALSE;
        }
        else
            DosFreeMem (pbShMem);                       // Open count von DosGetNamedSharedMem dekrementieren

        *(pbShMem)     = 0;
        *(pbShMem + 1) = 0;
        return TRUE;

    } /* Blank all Settings */

    rc = DosOpen(szFileName, &hfProfile, &ulAction, 0, FILE_NORMAL,
                 OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                 OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYWRITE | OPEN_ACCESS_READONLY, NULL);
    if (rc)
       return FALSE;

    DosSetFilePtr(hfProfile, 0, FILE_END, &ulFileSize);
    DosSetFilePtr(hfProfile, 0, FILE_BEGIN, &ulDummy);

    rc = DosGetNamedSharedMem((PVOID *)&pbShMem, SHAREDMEMNAME, PAG_READ | PAG_WRITE);
    if (rc == 0)
    {
        DosFreeMem((PVOID)pbShMem);
        DosFreeMem((PVOID)pbShMem);
    }

    rc = DosAllocSharedMem((PVOID *)&pbShMem, SHAREDMEMNAME, ulFileSize + 2,
                           PAG_COMMIT | PAG_READ | PAG_WRITE);
    if (rc)
        goto CloseFile;

    rc = DosRead(hfProfile, pbShMem, ulFileSize, &ulDummy);
    if (rc)
        ulFileSize = 0;

    *(pbShMem + ulFileSize)     = 0;
    *(pbShMem + ulFileSize + 1) = 0;

CloseFile:
    DosClose(hfProfile);

    return (rc) ? FALSE : TRUE;
}

ULONG getSize(HFILE hf)
{
    FILESTATUS3 fs3;
    APIRET rc;

    rc = DosQueryFileInfo(hf, FIL_STANDARD, &fs3, sizeof(fs3));
    if (rc)
        return 0;

    return fs3.cbFile;
}

/******************************************************************************/

BOOL Init2(PSZ pszGroupFile, PSZ pszUserFile)
{
    HFILE  hfGProfile, hfUProfile;
    ULONG  ulAction, ulOpenFlags;
    ULONG  ulFileSize, ulBytesRead;
    PBYTE  pbShMem, pbPoi, pbLockPoi;
    WPParseObject wpo;
    APIRET rc;

    pvShMem         = NULL;
    ulOpenFlags     =   0 ;

    if (pszGroupFile[0] != '\0')
    {
       rc = DosOpen(pszGroupFile, &hfGProfile, &ulAction, 0, FILE_NORMAL,
                    OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                    OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYWRITE | OPEN_ACCESS_READONLY, NULL);
       if (rc == 0)
           ulOpenFlags |= GROUPFILE_OK;
    }
    else
       ulOpenFlags |= NO_GROUPFILE;

    if (pszUserFile[0] != '\0')
    {
       rc = DosOpen(pszUserFile, &hfUProfile, &ulAction, 0, FILE_NORMAL,
                    OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                    OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYWRITE | OPEN_ACCESS_READONLY, NULL);
       if (rc == 0)
          ulOpenFlags |= USERFILE_OK;
    }
    else
       ulOpenFlags |= NO_USERFILE;

    if (ulOpenFlags == 0)
       return FALSE;

    ulFileSize = 0;
    if (ulOpenFlags & GROUPFILE_OK)
        ulFileSize += getSize(hfGProfile);
    if (ulOpenFlags & USERFILE_OK)
        ulFileSize += getSize(hfUProfile);

    rc = DosGetNamedSharedMem((PVOID *)&pbShMem, SHAREDMEMNAME, PAG_READ | PAG_WRITE);
    if (rc == 0)
    {
        DosFreeMem((PVOID)pbShMem);
        DosFreeMem((PVOID)pbShMem);
    }

    rc = DosAllocSharedMem((PVOID *)&pbShMem, SHAREDMEMNAME,
                           ulFileSize + sizeof(WPParseObject), PAG_COMMIT | PAG_READ | PAG_WRITE);
    if (rc)
        goto CloseFiles;
    pbPoi = pbShMem;

//  if (pbLockMem)
//  {
//      Unlock();
//      DosFreeMem(pbLockMem);
//      pbLockMem = NULL;
//  } /* endif */

//  rc = DosAllocMem((PVOID *)&pbLockMem,
//                         ulFileSize + sizeof(WPParseObject), PAG_COMMIT | PAG_READ | PAG_WRITE);
//  if (rc)
//      goto CloseFiles;
//  pbLockPoi = pbLockMem;

    if (ulOpenFlags & GROUPFILE_OK)
    {
        while (( (rc = DosRead(hfGProfile, &wpo, sizeof(WPParseObject), &ulBytesRead)) == 0)
           &&  (ulBytesRead == sizeof(WPParseObject)))
        {
            switch (wpo.usType)
            {
                case ACCESS_OBJECT:
                    memcpy(pbPoi, &wpo, sizeof(WPParseObject));
                    pbPoi += sizeof(WPParseObject);
                    DosRead(hfGProfile, pbPoi, ((wpo.usLen) - sizeof (WPParseObject)),
                            &ulBytesRead);
                    pbPoi += ulBytesRead;
                    break;

                case LOCK_OBJECT:
//                  memcpy(pbLockPoi, &wpo, sizeof(WPParseObject));
//                  pbLockPoi += sizeof(WPParseObject);
//                  DosRead(hfGProfile, pbLockPoi, ((wpo.usLen) - sizeof (WPParseObject)),
//                          &ulBytesRead);
//                  pbLockPoi += ulBytesRead;
                    break;

                default:
                    DosSetFilePtr(hfGProfile, ((wpo.usLen) - sizeof (WPParseObject)),
                                  FILE_CURRENT, &ulBytesRead);
                    break;
            } /* endswitch */
        } /* endwhile */
    }

    if (ulOpenFlags & USERFILE_OK)
    {
        while (( (rc = DosRead(hfUProfile, &wpo, sizeof(WPParseObject), &ulBytesRead)) == 0)
           &&  (ulBytesRead == sizeof(WPParseObject)))
        {
            switch (wpo.usType)
            {
                case ACCESS_OBJECT:
                    memcpy(pbPoi, &wpo, sizeof(WPParseObject));
                    pbPoi += sizeof(WPParseObject);
                    DosRead(hfUProfile, pbPoi, ((wpo.usLen) - sizeof (WPParseObject)),
                            &ulBytesRead);
                    pbPoi += ulBytesRead;
                    break;

                case LOCK_OBJECT:
//                  memcpy(pbLockPoi, &wpo, sizeof(WPParseObject));
//                  pbLockPoi += sizeof(WPParseObject);
//                  DosRead(hfUProfile, pbLockPoi, ((wpo.usLen) - sizeof (WPParseObject)),
//                          &ulBytesRead);
//                  pbLockPoi += ulBytesRead;
                    break;

                default:
                    DosSetFilePtr(hfGProfile, ((wpo.usLen) - sizeof (WPParseObject)),
                                  FILE_CURRENT, &ulBytesRead);
                    break;
            } /* endswitch */
        } /* endwhile */
    }

    wpo.usType = 0;
    wpo.usLen  = 0;
    memcpy(pbPoi,     &wpo, sizeof(WPParseObject));
//  memcpy(pbLockPoi, &wpo, sizeof(WPParseObject));

    rc         = 0;

CloseFiles:
    if (ulOpenFlags & GROUPFILE_OK)
        DosClose(hfGProfile);

    if (ulOpenFlags & USERFILE_OK)
        DosClose(hfUProfile);

    return (rc==0) ? TRUE : FALSE;
}

/*******************************************************************************/

BOOL Lock()
{
    BYTE   bDataPacket, abParmPacket[PARMBLOCKSIZE];
    ULONG  ulAction, ulParmSize, ulDataSize;
    PBYTE  pbFileName;
    LockObject *plo;
    APIRET rc;

    plo = (LockObject *) pbLockMem;
    if (plo == NULL)
        return FALSE;

    while (plo->usType != 0)
    {
        pbFileName = (PBYTE) &(plo->FileName);

        if ((strlen(pbFileName) == 2) && (pbFileName[1] == ':'))
        {
//          rc = DosOpen(pbFileName, &(plo->hfile), &ulAction, 0, FILE_NORMAL,
//                       OPEN_ACTION_FAIL_IF_NEW  | OPEN_ACTION_OPEN_IF_EXISTS,
//                       OPEN_FLAGS_DASD          | OPEN_FLAGS_FAIL_ON_ERROR |
//                       OPEN_SHARE_DENYREADWRITE | OPEN_ACCESS_READONLY, NULL);
//          if (rc == 0)
//          {
                abParmPacket[0] = LOCK;
                abParmPacket[1] = pbFileName[0] - 'A';
                ulParmSize      = 2;
                ulDataSize      = 1;
                bDataPacket     = 0;
                rc = DosDevIOCtl ((HFILE) -1, 8, DRIVELOCK,
                                  abParmPacket, PARMBLOCKSIZE,       &ulParmSize,
                                  &bDataPacket, sizeof(bDataPacket), &ulDataSize);
                plo->hfile = (HFILE) -1;
//              if (rc != 0)
//                  DosClose(plo->hfile);
//          }
        }
        else
        {
            rc = DosOpen(pbFileName, &(plo->hfile), &ulAction, 0, FILE_NORMAL,
                         OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
                         OPEN_FLAGS_FAIL_ON_ERROR | OPEN_SHARE_DENYREADWRITE |
                         OPEN_ACCESS_READONLY, NULL);
        } /* endif */
        if (rc != 0)
            plo->hfile = NULLHANDLE;

        plo = (LockObject *)(((BYTE *) plo) + plo->usLen);
    } /* endwhile */

    return TRUE;
}

/*******************************************************************************/

BOOL Unlock()
{
    BYTE   bDataPacket, abParmPacket[PARMBLOCKSIZE];
    ULONG  ulParmSize, ulDataSize;
    PBYTE  pbFileName;
    LockObject *plo;

    plo = (LockObject *) pbLockMem;
    if (plo == NULL)
        return FALSE;

    while (plo->usType != 0)
    {
        if (plo->hfile != NULLHANDLE)
        {
            pbFileName = (PBYTE) &(plo->FileName);

            if ((strlen(pbFileName) == 2) && (pbFileName[1] == ':'))
            {
                abParmPacket[0] = UNLOCK;
                abParmPacket[1] = pbFileName[0] - 'A';
                ulParmSize      = 2;
                ulDataSize      = 1;
                bDataPacket     = 0;
                DosDevIOCtl ((HFILE) -1, 8, DRIVELOCK,
                             abParmPacket, PARMBLOCKSIZE,       &ulParmSize,
                             &bDataPacket, sizeof(bDataPacket), &ulDataSize);
            }
            else
                DosClose(plo->hfile);
        }


        plo = (LockObject *)(((BYTE *) plo) + plo->usLen);
        plo->hfile = NULLHANDLE;
    } /* endwhile */

    return TRUE;
}

/*******************************************************************************/

BOOL TestInit(PCHAR szFileName)
{
    HEV    hevWait;
    APIRET rc;

    if (Init(szFileName))
    {
        DosCreateEventSem(SEMAPHORENAME, &hevWait, 0, 0);
        rc = DosWaitEventSem(hevWait, SEM_INDEFINITE_WAIT);
        DosCloseEventSem(hevWait);
        return (rc) ? FALSE : TRUE;
    } /* endif */
    return FALSE;
}

/*******************************************************************************/

BOOL TestInit2(PSZ pszGroupFile, PSZ pszUserFile)
{
    HEV    hevWait;
    APIRET rc;

    if (Init2(pszGroupFile, pszUserFile))
    {
        Lock();
        DosCreateEventSem(SEMAPHORENAME, &hevWait, 0, 0);
        rc = DosWaitEventSem(hevWait, SEM_INDEFINITE_WAIT);
        DosCloseEventSem(hevWait);
        Unlock();
        return (rc) ? FALSE : TRUE;
    } /* endif */
    return FALSE;
}

