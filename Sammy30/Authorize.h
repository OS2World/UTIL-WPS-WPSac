/******************************************************************************
*
*  Module Name: Sammy
*
*  OS/2 Workplace Shell Access Manager
*
******************************************************************************/
#ifndef AUTHORIZE_H
#define AUTHORIZE_H

#include "SamGlob.h"
#include "SacAlias.h"

BOOL LoadAuthorize (VOID);
VOID UnloadAuthorize (VOID);
BOOL CallAuthorize (PSZ pszGrp, PSZ pszUsr);
VOID InitAuthorize (ALIASDATA *pLoginData);

#endif /* AUTHORIZE_H */
