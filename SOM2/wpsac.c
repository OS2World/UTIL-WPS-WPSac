/***************************************************************************
 *
 * PROGRAMMNAME: WPSAC
 * -------------
 *
 * MODULNAME:    WPSac
 * ----------
 *
 * VERSION:      3.0
 * --------
 *
 * BESCHREIBUNG:
 * -------------
 *  Workplace-Shell Access Controll: Ableitung der Klasse WPObject
 *
 * FUNKTIONEN:
 * -----------
 *
 * COMPILER:
 * ---------
 *  IBM Visual Age C++ 3.0
 *
 * LIBRARIES:
 * ----------
 *
 * CHANGE LOG:
 * -----------
 *  Ver.    Date      Comment
 *  ----    --------  -------
 *  3.00    24.02.97  Erster Release
 *
 *  Copyright (C) 1994...1997 noller & breining software
 *
 ******************************************************************************/
#pragma info(noeff)

#define INCL_WIN
#define INCL_DOS
#define INCL_PM
#define INCL_WPCLASS

#ifndef SOM_Module_wpsac_Source
#define SOM_Module_wpsac_Source
#endif

#define wpSac_Class_Source
#define M_wpSac_Class_Source

#include <os2.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <memory.h>

#include "WPSac.ih"

#include "samglob.h"
#include "SamMem.h"
#include "debug.h"
#include "WPSacDlg.h"
#include "authoriz.h"

#define WM_LOGOFF           WM_USER+2       // tempor„r von hier???

/* Deklaration der Authorisierungsfunktion */
BOOL (* APIENTRY pfnAuthorize) (OBJDESCR *objdescr);

int SOMLINK myReplacementForSOMOutChar (char);
MRESULT EXPENTRY DbgDlgProc (HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2);

/***********************************************************************
       Globale Daten, auf die von allen Instanzen
       gemeinsam zugegriffen werden kann
 ***********************************************************************/
#define LDERRSIZE   128                 /* L„nge des Object-Buffers von DosLoadModule */

CHAR    LoadErr[LDERRSIZE];
HMODULE hmod                   = NULLHANDLE;
HMODULE hmodAuthoriz           = NULLHANDLE;
PSHARE1 pShareInitOS2          = 0;
somId   somId_QueryRealName    = NULL;
somId   somId_QueryType        = NULL;
somId   somId_IsCurrentDesktop = NULL;
somId   somId_QueryStyle       = NULL;
somId   somId_SetStyle         = NULL;

/********************************************************************
 *  Es werden durch Aufruf eines externen Authorisierungsprozesses
 *  folgende Variablen bestimmt:
 *  _ulAND_CTXT, _ulOR_CTXT   werden von wpFilterPopupMenu zur
 *                            Manipulierung des Kontextmenus
 *                            herangezogen
 *  ulAND_STYLE, ulOR_STYLE   werden an Prozedur NewStyle bergeben,
 *                            die dann den Style des Objekts
 *                            neu setzt
 *  _ulMisc                   sonstige Flags
 ********************************************************************/
SOM_Scope void SOMLINK wpSac_AccessControl (WPSac *somSelf)
    {
    OBJDESCR objdescr;
    CHAR     szRealName[CCHMAXPATH];
    ULONG    ulRealName;
    ULONG    ulAND_STYLE;
    ULONG    ulOR_STYLE;
    PCHAR    (* ptrQueryType) (WPSac *somSelf);
    BOOL     (* ptrQueryRealName) (WPSac *somSelf, CHAR *szRealName, ULONG *pcb, BOOL fQualified);

    SOMClass  *MetaClass = SOM_GetClass (somSelf);
    WPSacData *somThis   = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac","wpSac_AccessControl");

    /* Muá das aktuelle Objekt noch nach Zugriffsinformationen gefragt werden? */
    if ((_ulMisc & DONT_ASK_ANY_MORE) && !(_ulFlag & ACCESSCONTROL_NECESSARY))
        {
        DebugE (D_SOM, _wpQueryTitle (somSelf), "Don't ask any more");
        return;
        }

    /* Initialisieren der Variablen */
    _ulFlag                &= ~ACCESSCONTROL_NECESSARY;
    _ulMisc                 =  0;
    _ulOR_CTXT              =  0;
    _ulAND_CTXT             = ~0;
    ulOR_STYLE              =  0;
    ulAND_STYLE             = ~0;
    *szRealName             = '\0';
    ulRealName              = CCHMAXPATH;

    /* RealName bestimmen */
    _somFindMethod (MetaClass, somId_QueryRealName, (somMethodPtr *)&ptrQueryRealName);
    if (ptrQueryRealName != NULL)
        {
        if (!ptrQueryRealName (somSelf, szRealName, &ulRealName, TRUE))
            goto Exit;
        if (*szRealName == '\0')
            goto Exit;
        }

    /* Type bestimmen */
    _somFindMethod (MetaClass, somId_QueryType, (somMethodPtr *)&ptrQueryType);
    if (ptrQueryType != NULL)
        {
        objdescr.szType = ptrQueryType (somSelf);
        if (objdescr.szType == NULL)
            objdescr.szType = "";
        }

    /* OBJDESCR-Struktur fllen */
    objdescr.szTitle        = _wpQueryTitle(somSelf);
    objdescr.szDefaultTitle = _wpclsQueryTitle (MetaClass);
    objdescr.szPhysName     = szRealName;
    objdescr.szObjectClass  = _somGetClassName (somSelf);
    objdescr.ulVersion      = OBJDESCR_VERSION;
    objdescr.ulMisc         = _ulMisc;

    if (objdescr.szTitle == NULL || *objdescr.szTitle == '\0')
       goto Exit;

    /* Authorisierungsberprfung */
    if (hmodAuthoriz != NULLHANDLE)
        {
        DebugE (D_SOM, objdescr.szTitle, "pfnAuthorize");

        if (pfnAuthorize (&objdescr) && (objdescr.ulVersion == OBJDESCR_VERSION))
            {
            DebugE (D_SOM, "pfnAuthorize", "TRUE");
            _ulMisc      = objdescr.ulMisc;
            _ulAND_CTXT  = objdescr.ulAND_CTXT;
            _ulOR_CTXT   = objdescr.ulOR_CTXT;
            ulAND_STYLE  = objdescr.ulAND_STYLE;
            ulOR_STYLE   = objdescr.ulOR_STYLE;
            }
        else
            DebugE (D_SOM, "pfnAuthorize", "FALSE");
        }

Exit:
    _NewStyle (somSelf, ulAND_STYLE, ulOR_STYLE);

    if (_ulMisc & DONT_ASK_ANY_MORE)
        _wpSaveDeferred (somSelf);

    return;
    }

/********************************************************************
 *  Diese Funktion setzt den Style des Objektes neu.
 *  Hierzu wird der Style zun„chst mit _ulRestore_Style_AND/OR wieder
 *  zurck- und dann mit ulAND/OR_STYLE neu gesetzt.
 *  Die Instanzvariablen _ulRestore_Style_AND/OR werden dann neu
 *  bestimmt, so daá der ursprngliche Style wieder rekonstruiert
 *  werden kann.
 *  Dabei werden jedoch nur die tats„chlich ge„nderten Bits
 *  bercksichtigt (um dem Objekt m”glichst viele Freiheiten
 *  zu lassen).
 ********************************************************************/
SOM_Scope void SOMLINK wpSac_NewStyle (WPSac *somSelf,
                                       ULONG ulAND_STYLE,
                                       ULONG ulOR_STYLE)
    {
    ULONG ulCleanStyle;
    ULONG ulNewStyle;
    ULONG ulOldStyle;
    BOOL  (* ptrSetStyle) (WPSac *somSelf, ULONG ulNewStyle);
    ULONG (* ptrQueryStyle) (WPSac *somSelf);

    SOMClass  *MetaClass = SOM_GetClass (somSelf);
    WPSacData *somThis = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac", "wpSac_NewStyle");

    if ((somId_SetStyle == NULL) || (somId_QueryStyle == NULL))
       {
       DebugE (D_SOM, _wpQueryTitle (somSelf), "String ID for Style function == NULL");
       return;
       }

    _somFindMethod (MetaClass, somId_QueryStyle, (somMethodPtr *)&ptrQueryStyle);
    _somFindMethod (MetaClass, somId_SetStyle, (somMethodPtr *)&ptrSetStyle);

    if ((ptrSetStyle == NULL) || (ptrQueryStyle == NULL))
       {
       DebugE (D_SOM, _wpQueryTitle (somSelf), "Style function not found");
       return;
       }

    ulOldStyle           = ptrQueryStyle (somSelf);
    ulCleanStyle         = (ulOldStyle & _ulRestore_Style_AND) | _ulRestore_Style_OR;
    ulNewStyle           = (ulCleanStyle & ulAND_STYLE) | ulOR_STYLE;
    _ulRestore_Style_AND = ~((ulCleanStyle ^ ulNewStyle) & ulNewStyle);
    _ulRestore_Style_OR  = (ulCleanStyle ^ ulNewStyle) & ulCleanStyle;

    if (ulOldStyle != ulNewStyle)
       ptrSetStyle (somSelf, ulNewStyle);

    return;
    }

/********************************************************************
 *  Diese Funktion dient der Initialisierung der Instanzdaten
 ********************************************************************/
SOM_Scope void SOMLINK wpSac_wpInitData (WPSac *somSelf)
    {
    WPSacData *somThis = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac", "wpSac_wpInitData");

    parent_wpInitData (somSelf);                        // Muá als erstes gerufen werden

    _ulFlag              =  ACCESSCONTROL_NECESSARY;    // Wird nicht gespeichert!
    _ulMisc              =  0;
    _ulAND_CTXT          = ~0;
    _ulOR_CTXT           =  0;
    _ulRestore_Style_AND = ~0;
    _ulRestore_Style_OR  =  0;

    return;
    }

/********************************************************************
 *  Behandlung einer Popup-Menu Selektion
 ********************************************************************/
SOM_Scope BOOL SOMLINK wpSac_wpMenuItemSelected (WPSac *somSelf,
                                                 HWND hwndFrame,
                                                 ULONG ulMenuId)
    {
//  WPSacData *somThis = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac", "wpSac_wpMenuItemSelected");

    switch (ulMenuId)
        {
        case IDM_LOGOUT:
            DebugE (D_SOM, "wpSac_wpMenuItemSelected", "Logout");

            if (pShareInitOS2 && pShareInitOS2->hwndSammy)
                WinPostMsg(pShareInitOS2->hwndSammy,
                           WM_LOGOFF,
                           0, 0);
            return TRUE;

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

/********************************************************************
 *  Wird gerufen, um einem Objekt die M”glichkeit zu geben, sein
 *  Popup-Men zu modifizieren; das Ergebnis wird durch die
 *  2 *_CTXT-Instanzvariable modifziert, die durch _AccessControl
 *  bestimmt wurden.
 ********************************************************************/
SOM_Scope ULONG SOMLINK wpSac_wpFilterPopupMenu (WPSac *somSelf,
                                                 ULONG ulFlags,
                                                 HWND hwndCnr,
                                                 BOOL fMultiSelect)
    {
    WPSacData *somThis = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac","wpSac_wpFilterPopupMenu");

    _AccessControl (somSelf);

    return ((parent_wpFilterPopupMenu (somSelf, ulFlags, hwndCnr, fMultiSelect) &
                                       _ulAND_CTXT) | _ulOR_CTXT);
    }

/********************************************************************
 *  Wird gerufen, um einem Objekt die M”glichkeit zu geben, seinem
 *  Popup-Men zus„tzliche Eintr„ge zu geben
 *  Wenn das Objekt der Desktop ist, wird hier das Logout-Men
 *  hinzugefgt. Alle anderen Objekte erhalten einen DEBUG-Punkt.
 ********************************************************************/
SOM_Scope BOOL SOMLINK wpSac_wpModifyPopupMenu (WPSac *somSelf,
                                                HWND hwndMenu,
                                                HWND hwndCnr,
                                                ULONG iPosition)
    {
    BOOL    (* ptrIsCurrentDesktop) (WPSac *somSelf);

    SOMClass  *MetaClass = SOM_GetClass (somSelf);
//  WPSacData *somThis   = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac", "wpSac_wpModifyPopupMenu");

    if (pShareInitOS2 && pShareInitOS2->hwndSammy)
        {
        _somFindMethod (MetaClass, somId_IsCurrentDesktop, (somMethodPtr *)&ptrIsCurrentDesktop);

        if (ptrIsCurrentDesktop != NULL)
            if (ptrIsCurrentDesktop (somSelf))
                {
                DebugE (D_SOM, "WPSac", "Insert menu item");
                _wpInsertPopupMenuItems (somSelf, hwndMenu, 0, hmod, ID_LOGOUT,   WPMENUID_PRIMARY);
                _wpInsertPopupMenuItems (somSelf, hwndMenu, 0, hmod, ID_ABOUTBOX, WPMENUID_PRIMARY);
                }
        }

    #ifdef DEBUG
    _wpInsertPopupMenuItems (somSelf, hwndMenu, 0, hmod, ID_OPENDEBUG, WPMENUID_PRIMARY);
    #endif

    return parent_wpModifyPopupMenu (somSelf, hwndMenu, hwndCnr, iPosition);
    }

/********************************************************************
 *  ™ffnen einer Objekt-View
 ********************************************************************/
SOM_Scope HWND SOMLINK wpSac_wpOpen (WPSac *somSelf, HWND hwndCnr,
                                     ULONG ulView, ULONG param)
    {
//  WPSacData *somThis = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac", "wpSac_wpOpen");

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
            return WPSac_parent_WPObject_wpOpen (somSelf, hwndCnr, ulView, param);
        }

    return NULLHANDLE;
    }

/********************************************************************
 *  Bestimmen der aktuellen Default-View des Objektes
 ********************************************************************/
SOM_Scope ULONG SOMLINK wpSac_wpQueryDefaultView (WPSac *somSelf)
    {
    WPSacData *somThis = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac", "wpSac_wpQueryDefaultView");

    _AccessControl (somSelf);

    if (_ulMisc & NO_DEFAULT_OPEN)
        return OPEN_UNKNOWN;

    return parent_wpQueryDefaultView (somSelf);
    }

/********************************************************************
 *  Die gespeicherten Instanzdaten werden geladen.
 *  Sollten noch keine Instanzdaten gespeichert gewesen sein, so
 *  werden Standartwerte verwendet, die wie veraltete Instanzdaten
 *  behandelt werden.
 ********************************************************************/
SOM_Scope BOOL SOMLINK wpSac_wpRestoreState (WPSac *somSelf,
                                             ULONG ulReserved)
    {
    BOOL  bRC;
    BOOL  bValidInfo       = TRUE;

    WPSacData *somThis = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac", "wpSac_wpRestoreState");

    bRC = parent_wpRestoreState (somSelf, ulReserved);

    bValidInfo &= _wpRestoreLong (somSelf, CLASSNAME, KEY_ULMISC,              &_ulMisc);
    bValidInfo &= _wpRestoreLong (somSelf, CLASSNAME, KEY_ULAND_CTXT,          &_ulAND_CTXT);
    bValidInfo &= _wpRestoreLong (somSelf, CLASSNAME, KEY_ULOR_CTXT,           &_ulOR_CTXT);
    bValidInfo &= _wpRestoreLong (somSelf, CLASSNAME, KEY_ULRESTORE_STYLE_AND, &_ulRestore_Style_AND);
    bValidInfo &= _wpRestoreLong (somSelf, CLASSNAME, KEY_ULRESTORE_STYLE_OR,  &_ulRestore_Style_OR);

    if (!bValidInfo)
        {
        _ulMisc              =  0;
        _ulAND_CTXT          = ~0;
        _ulOR_CTXT           =  0;
        _ulRestore_Style_AND = ~0;
        _ulRestore_Style_OR  =  0;
        }

    _ulFlag |= ACCESSCONTROL_NECESSARY;
    _AccessControl (somSelf);

    return bRC;
    }

/********************************************************************
 *  Speichern der Instanzdaten.
 ********************************************************************/
SOM_Scope BOOL SOMLINK wpSac_wpSaveState (WPSac *somSelf)
    {
    BOOL bRC;

    WPSacData *somThis = WPSacGetData (somSelf);
    WPSacMethodDebug ("WPSac", "wpSac_wpSaveState");

    if (_ulFlag & ACCESSCONTROL_NECESSARY)
        _AccessControl (somSelf);

    bRC = parent_wpSaveState (somSelf);

    DebugE (D_SOM, _wpQueryTitle (somSelf), "Save style info");

    bRC &= _wpSaveLong (somSelf, CLASSNAME, KEY_ULMISC,              _ulMisc);
    bRC &= _wpSaveLong (somSelf, CLASSNAME, KEY_ULAND_CTXT,          _ulAND_CTXT);
    bRC &= _wpSaveLong (somSelf, CLASSNAME, KEY_ULOR_CTXT,           _ulOR_CTXT);
    bRC &= _wpSaveLong (somSelf, CLASSNAME, KEY_ULRESTORE_STYLE_AND, _ulRestore_Style_AND);
    bRC &= _wpSaveLong (somSelf, CLASSNAME, KEY_ULRESTORE_STYLE_OR,  _ulRestore_Style_OR);

    return bRC;
    }


#undef SOM_CurrentClass
#define SOM_CurrentClass SOMMeta

/********************************************************************
 *  Initialisierung der Instanzdaten des Class-Object
 ********************************************************************/
SOM_Scope void SOMLINK wpSacM_wpclsInitData (M_WPSac *somSelf)
    {
    APIRET rc;

//  M_WPSacData *somThis = M_WPSacGetData (somSelf);
    M_WPSacMethodDebug ("M_WPSac", "wpSacM_wpclsInitData");

    parent_wpclsInitData (somSelf);                     // Muá als erstes gerufen werden

    /* Bestimmen der Einsprungpunkten in AUTHORIZ.DLL */
    if (hmodAuthoriz == NULLHANDLE)
        {
        rc = DosLoadModule (LoadErr, LDERRSIZE, AUTH_DLLNAME, &hmodAuthoriz);
        if (rc == 0)
            {
            DebugE (D_SOM, "M_WPSac: DLL geladen", AUTH_DLLNAME);
            rc = DosQueryProcAddr (hmodAuthoriz, 0, AUTH_DLLENTRTY, (PFN *)&pfnAuthorize);
            if (rc != 0)
                {
                DosFreeModule (hmodAuthoriz);
                hmodAuthoriz = NULLHANDLE;
                }
            else
                DebugE (D_SOM, "M_WPSac", "Authorize-Funktion linked");
            }
       }

    /* Bestimmen der eigenen Module-handle */
    if (hmod == NULLHANDLE)
        {
        zString zsPathName;
        zsPathName = _somLocateClassFile (SOMClassMgrObject, SOM_IdFromString (CLASSNAME),
                                          WPSac_MajorVersion, WPSac_MinorVersion);

        if (DosQueryModuleHandle (zsPathName, &hmod))
            {
            DebugE (D_SOM, "WPSamM_clsQueryModuleHandle", "Failed to load module");
            hmod = NULLHANDLE;
            }
        }

    /* SOM_Ids zu h„ufig gefragten Strings bestimmen */
    if (somId_QueryRealName == NULL)
        somId_QueryRealName = SOM_IdFromString ("wpQueryRealName");

    if (somId_QueryType == NULL)
        somId_QueryType = SOM_IdFromString ("wpQueryType");

    if (somId_IsCurrentDesktop == NULL)
        somId_IsCurrentDesktop = SOM_IdFromString ("wpIsCurrentDesktop");

    if (somId_QueryStyle == NULL)
        somId_QueryStyle = SOM_IdFromString ("wpQueryStyle");

    if (somId_SetStyle == NULL)
        somId_SetStyle = SOM_IdFromString ("wpSetStyle");

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

    return;
    }

