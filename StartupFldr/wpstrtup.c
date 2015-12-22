/*
 *  This file was generated by the SOM Compiler and Emitter Framework.
 *  Generated using:
 *      SOM Emitter emitctm: 2.42
 */
#pragma info(noeff)

#define INCL_DOSMODULEMGR
#define INCL_DOSPROCESS
#define INCL_WINPOINTERS

#ifndef SOM_Module_wpstrtup_Source
#define SOM_Module_wpstrtup_Source
#endif
#define WPStartup2_Class_Source
#define M_WPStartup2_Class_Source

#include <os2.h>

#include "wpstrtup.ih"
#include "SamGlob.h"
#include "SamMem.h"
#include "dlg.h"
#include "debug.h"

int SOMLINK myReplacementForSOMOutChar (char);
MRESULT EXPENTRY DbgDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

HMODULE   hmod           = NULLHANDLE;
PSHARE1   pShareInitOS2  = NULL;
const PSZ szDefaultTitle = DEFAULT_TITLE;

SOM_Scope BOOL SOMLINK wpStartup2_isSet (WPStartup2 *somSelf)
    {
    ULONG   ulc;
    HOBJECT hObj;
    BOOL    bRC = FALSE;

    /* WPStartup2Data *somThis = WPStartup2GetData (somSelf); */
    WPStartup2MethodDebug ("WPStartup2", "wpStartup2_isSet");

    if (pShareInitOS2)
        {
        hObj = _wpQueryHandle (somSelf);
        for (ulc = 0; ulc < NMAX_WPSTRTUP; ulc++)
            if (pShareInitOS2->hStartupFldr[ulc] == hObj)
                bRC = TRUE;
        }

    DebugULx (D_SOM, "_isSet", "check for object ", somSelf);
    DebugULx (D_SOM, "_isSet", "result was ", bRC);
    return bRC;
    }

SOM_Scope BOOL SOMLINK wpStartup2_markSet (WPStartup2 *somSelf,
                                           HOBJECT hObj)
    {
    ULONG ulc;
    BOOL  bRC = FALSE;

    /* WPStartup2Data *somThis = WPStartup2GetData(somSelf); */
    WPStartup2MethodDebug("WPStartup2","wpStartup2_markSet");
    somSelf;

    if (pShareInitOS2)
        {
        for (ulc = 0; ulc < NMAX_WPSTRTUP; ulc++)
            if (pShareInitOS2->hStartupFldr[ulc] == NULLHANDLE)
                {
                pShareInitOS2->hStartupFldr[ulc] = hObj;
                bRC = TRUE;
                }
        }

    DebugULx (D_SOM, "_markSet", "mark as set for object ", somSelf);
    DebugULx (D_SOM, "_markSet", "result was ", bRC);
    return bRC;
    }

SOM_Scope BOOL SOMLINK wpStartup2_wpMenuItemSelected (WPStartup2 *somSelf,
                                                      HWND hwndFrame,
                                                      ULONG ulMenuId)
    {
    /* WPStartup2Data *somThis = WPStartup2GetData (somSelf); */
    WPStartup2MethodDebug ("WPStartup2", "wpStartup2_wpMenuItemSelected");

    switch (ulMenuId)
        {
        case IDM_ABOUTBOX:
            WinDlgBox (HWND_DESKTOP, hwndFrame, WinDefDlgProc, hmod, IDD_ABOUTBOX, NULL);
            return TRUE;

        #ifdef DEBUG
        case IDM_OPENDEBUG:
            return _wpViewObject (somSelf, hwndFrame, PRIMARY_DEBUG, 0) != NULLHANDLE;
        #endif

        default:
            return parent_wpMenuItemSelected (somSelf, hwndFrame, ulMenuId);
        }
    }

SOM_Scope BOOL SOMLINK wpStartup2_wpModifyPopupMenu (WPStartup2 *somSelf,
                                                     HWND hwndMenu,
                                                     HWND hwndCnr,
                                                     ULONG iPosition)
    {
    /* WPStartup2Data *somThis = WPStartup2GetData (somSelf); */
    WPStartup2MethodDebug ("WPStartup2", "wpStartup2_wpModifyPopupMenu");

    _wpInsertPopupMenuItems (somSelf, hwndMenu, 0, hmod, ID_ABOUTBOX, WPMENUID_PRIMARY);

    #ifdef DEBUG
    _wpInsertPopupMenuItems (somSelf, hwndMenu, 0, hmod, ID_OPENDEBUG, WPMENUID_PRIMARY);
    #endif

    return parent_wpModifyPopupMenu (somSelf, hwndMenu, hwndCnr, iPosition);
    }

SOM_Scope HWND SOMLINK wpStartup2_wpOpen (WPStartup2 *somSelf,
                                          HWND hwndCnr, ULONG ulView,
                                          ULONG param)
    {
    /* WPStartup2Data *somThis = WPStartup2GetData (somSelf); */
    WPStartup2MethodDebug ("WPStartup2", "wpStartup2_wpOpen");

    switch (ulView)
        {
        #ifdef DEBUG
        case PRIMARY_DEBUG:
            {
            ULONG ulResult;

            SOMOutCharRoutine = myReplacementForSOMOutChar;

            ulResult = WinDlgBox (HWND_DESKTOP, HWND_DESKTOP,
                (PFNWP)DbgDlgProc, hmod, IDD_DEBUG, NULL);

            SOM_TraceLevel  = (ulResult >> SHFT_TRACE)  & 3;
            SOM_WarnLevel   = (ulResult >> SHFT_WARN)   & 3;
            SOM_AssertLevel = (ulResult >> SHFT_ASSERT) & 3;
            }
            break;
        #endif

        default:
            return WPStartup2_parent_WPFolder_wpOpen (somSelf, hwndCnr, ulView, param);
        }

    return NULLHANDLE;
    }


SOM_Scope void SOMLINK wpStartup2_wpObjectReady (WPStartup2 *somSelf,
                                                 ULONG ulCode,
                                                 WPObject* refObject)
    {
    HOBJECT      hObj;
    WPObject    *pObj;
    HWND         hwndRet;
    somId        somId_ViewObject;
    somId        somId_UnlockObject;
    somTD_WPObject_wpViewObject viewObject;
    somTD_WPObject_wpUnlockObject unlockObject;

    /* WPStartup2Data *somThis = WPStartup2GetData(somSelf); */
    WPStartup2MethodDebug("WPStartup2","wpStartup2_wpObjectReady");

    // Wenn der Benutzer neu gestartet wurde, m�ssen jetzt
    // alle Objekte des Folders ge�ffnet werden (wpOpen)
    if (!_isSet (somSelf))
        {
        hObj = _wpQueryHandle (somSelf);
        DebugULx (D_SOM, "_wpObjectReady", "HOBJECT", hObj);
        if (_markSet (somSelf, hObj))
            {
            if (_wpPopulate (somSelf, 0, NULL, FALSE))
                {
                somId_ViewObject = SOM_IdFromString ("wpViewObject");
                somId_UnlockObject = SOM_IdFromString ("wpUnlockObject");

                for (pObj = _wpQueryContent (somSelf, NULL, QC_FIRST);
                     pObj != NULL;
                     pObj = _wpQueryContent (somSelf, pObj, QC_NEXT))
                    {
                    boolean rc;
                    DebugULx (D_SOM, "wpObjectReady", "pObj", pObj);
//                  rc = _somFindMethod (pObj, somId_ViewObject, (somMethodPtr *)&viewObject);
//                  DebugULx (D_SOM, "wpObjectReady", "rc", rc);
//                  DebugULx (D_SOM, "wpObjectReady", "ptr", viewObject);
//                  rc = _somFindMethodOk (pObj, somId_ViewObject, (somMethodPtr *)&viewObject);
//                  DebugULx (D_SOM, "wpObjectReady", "orc", rc);
//                  DebugULx (D_SOM, "wpObjectReady", "optr", viewObject);
//                  viewObject = _somLookupMethod (pObj, somId_ViewObject);
//                  DebugULx (D_SOM, "wpObjectReady", "lptr", viewObject);
//                  viewObject = somResolveByName (pObj, "wpViewObject");
//                  DebugULx (D_SOM, "wpObjectReady", "rptr", viewObject);
//                  if (viewObject)
//                      {
//                      DebugULx (D_SOM, "wpObjectReady", "MethodPtr", viewObject);
//                      viewObject (pObj, NULLHANDLE, OPEN_DEFAULT, 0);
                        _somDispatch (pObj, (somToken *)&hwndRet, somId_ViewObject, pObj, NULLHANDLE, OPEN_DEFAULT, 0);
//                      DebugULx (D_SOM, "wpObjectReady", "retCode", hwndRet);
//                      }
//                  unlockObject = somResolveByName (pObj, "wpUnlockObject");
//                  if (unlockObject)
//                      unlockObject (pObj);
                    _somDispatch (pObj, (somToken *)&hwndRet, somId_UnlockObject, pObj);
                    }
//                DebugE (D_SOM, "_wpObjectReady", "now free memory");
//                _somFree (somId_ViewObject);
//                _somFree (somId_UnlockObject);
//                DebugE (D_SOM, "_wpObjectReady", "now free memory done");
                }
            }
        }

    WPStartup2_parent_WPFolder_wpObjectReady (somSelf, ulCode, refObject);
    }

SOM_Scope HMODULE SOMLINK wpStartup2M_clsQueryModuleHandle (M_WPStartup2 *somSelf)
    {
    /* M_WPStartup2Data *somThis = M_WPStartup2GetData (somSelf); */
    M_WPStartup2MethodDebug ("M_WPStartup2", "wpStartup2M_clsQueryModuleHandle");
    somSelf;

    if (hmod == NULLHANDLE)
        {
        /* Bestimmen der eigenen Module-handle */
        zString zsPathName;
        zsPathName = _somLocateClassFile (SOMClassMgrObject, SOM_IdFromString (CLASSNAME),
                                          WPStartup2_MajorVersion, WPStartup2_MinorVersion);

        if (DosQueryModuleHandle (zsPathName, &hmod) )
            {
            DebugE (D_SOM, "_clsQueryModuleHandle", "Failed to load module");
            hmod = NULLHANDLE;
            }
        }   /* end if (hmod == NULLHANDLE) */
    /* Return statement to be customized: */
    return hmod;
    }

SOM_Scope void SOMLINK wpStartup2M_wpclsInitData (M_WPStartup2 *somSelf)
    {
    /* M_WPStartup2Data *somThis = M_WPStartup2GetData (somSelf); */
    M_WPStartup2MethodDebug ("M_WPStartup2", "wpStartup2M_wpclsInitData");

    DebugE (D_SOM, "_wpclsInitData", "enter parent method");
    M_WPStartup2_parent_M_WPFolder_wpclsInitData (somSelf);
    DebugE (D_SOM, "_wpclsInitData", "parent method done");

    _clsQueryModuleHandle (somSelf);

    /* Zugriff auf Shared memory schaffen */
    if (!pShareInitOS2)
        {
        if (DosGetNamedSharedMem ((PPVOID)&pShareInitOS2,
                                  SHARE_INITOS2,
                                  PAG_READ | PAG_WRITE))
            pShareInitOS2 = NULL;
        else
            pShareInitOS2->ulActive |= ACTIVE_WPSAC;
        }
    DebugE (D_SOM, "_wpclsInitData", "done");
    }


SOM_Scope PSZ SOMLINK wpStartup2M_wpclsQueryTitle (M_WPStartup2 *somSelf)
    {
    /* M_WPStartup2Data *somThis = M_WPStartup2GetData (somSelf); */
    M_WPStartup2MethodDebug ("M_WPStartup2", "wpStartup2M_wpclsQueryTitle");
    somSelf;

    return szDefaultTitle;
    }

SOM_Scope ULONG SOMLINK wpStartup2M_wpclsQueryIconData (M_WPStartup2 *somSelf,
                                                        PICONINFO pIconInfo)
    {
    /* M_WPStartup2Data *somThis = M_WPStartup2GetData (somSelf); */
    M_WPStartup2MethodDebug ("M_WPStartup2", "wpStartup2M_wpclsQueryIconData");

    if (pIconInfo != NULL)
        {
        pIconInfo->cb      = sizeof (ICONINFO);
        pIconInfo->fFormat = ICON_RESOURCE;
        pIconInfo->hmod    = _clsQueryModuleHandle (somSelf);
        pIconInfo->resid   = IDD_ICON;
        }

    return sizeof (ICONINFO);
    }


SOM_Scope ULONG SOMLINK wpStartup2M_wpclsQueryIconDataN (M_WPStartup2 *somSelf,
                                                         ICONINFO* pIconInfo,
                                                         ULONG ulIconIndex)
    {
    /* M_WPStartup2Data *somThis = M_WPStartup2GetData(somSelf); */
    M_WPStartup2MethodDebug ("M_WPStartup2", "wpStartup2M_wpclsQueryIconDataN");
    ulIconIndex;

    DebugE (D_SOM, "_wpclsQueryIconDataN", "Enter");
    if (pIconInfo != NULL)
        {
        pIconInfo->cb      = sizeof (ICONINFO);
        pIconInfo->fFormat = ICON_RESOURCE;
        pIconInfo->hmod    = _clsQueryModuleHandle (somSelf);
        pIconInfo->resid   = IDD_ICON2;
        }

    DebugE (D_SOM, "_wpclsQueryIconDataN", "Exit");
    return sizeof (ICONINFO);
    }

