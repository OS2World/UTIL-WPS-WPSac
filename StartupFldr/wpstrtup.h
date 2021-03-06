
/*
 * This file was generated by the SOM Compiler.
 * FileName: .\wpstrtup.h.
 * Generated using:
 *     SOM Precompiler somipc: 2.29.1.7
 *     SOM Emitter emith: 2.42
 */

/*
 * 
 * 
 *    Module Name: WPStartup2
 * 
 * 
 */


#ifndef SOM_WPStartup2_h
#define SOM_WPStartup2_h


#include <som.h>

/*
 * -- This emitter treats Method Tokens as Thunks by default.
 * -- Use the sc modifier "nothunks" to change this default
 */
#undef somresolve_
#define somresolve_(obj,mToken) ((somMethodProc*)((void)obj, mToken))

/*
 * Define the class name as an object type
 */
#ifndef WPStartup2
#define WPStartup2 SOMObject
#endif
#include <wpfolder.h>

/*
 * Start of user-defined types:
 */
#ifndef SOMClass
    #define SOMClass SOMObject
#endif /* SOMClass */
#ifndef WPFolder
    #define WPFolder SOMObject
#endif /* WPFolder */
#ifndef M_WPObject
    #define M_WPObject SOMObject
#endif /* M_WPObject */
#ifndef WPObject
    #define WPObject SOMObject
#endif /* WPObject */
#ifndef WPImageFile
    #define WPImageFile SOMObject
#endif /* WPImageFile */
#ifndef M_WPFileSystem
    #define M_WPFileSystem SOMObject
#endif /* M_WPFileSystem */
#ifndef M_WPFolder
    #define M_WPFolder SOMObject
#endif /* M_WPFolder */
#ifndef M_WPStartup2
    #define M_WPStartup2 SOMObject
#endif /* M_WPStartup2 */

/*
 * End of user-defined types.
 */

/*
 * Passthru lines: File: "C.h", "after"
 */



#define WPStartup2_MajorVersion 1
#define WPStartup2_MinorVersion 1

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define WPStartup2NewClass wpStartup2c
#pragma linkage(wpStartup2c, system)
#define WPStartup2ClassData wpStartup2d
#define WPStartup2CClassData wpStartup2x
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */

/*
 * Include the meta class file
 */
#include <wpstrtup.h>
/*--------------Migration------------*/
#define WPStartup2_classObj WPStartup2ClassData.classObject
#define _WPStartup2 WPStartup2_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK WPStartup2NewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(WPStartup2NewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct WPStartup2ClassDataStructure {
	SOMClass *classObject;
	somMToken isSet;
	somMToken markSet;
} SOMDLINK WPStartup2ClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct WPStartup2CClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK WPStartup2CClassData;

/*
 * New and Renew macros for WPStartup2
 */
#define WPStartup2New() \
   (_somNew(_WPStartup2 ? _WPStartup2 : WPStartup2NewClass(WPStartup2_MajorVersion, WPStartup2_MinorVersion)))
#define WPStartup2Renew(buf) \
   (_somRenew(_WPStartup2 ? _WPStartup2 : WPStartup2NewClass(WPStartup2_MajorVersion, WPStartup2_MinorVersion), buf))

/*
 * Override method: wpMenuItemSelected
 */
#define WPStartup2_wpMenuItemSelected(somSelf,hwndFrame,ulMenuId) \
	WPObject_wpMenuItemSelected(somSelf,hwndFrame,ulMenuId)

/*
 * Override method: wpModifyPopupMenu
 */
#define WPStartup2_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition) \
	WPObject_wpModifyPopupMenu(somSelf,hwndMenu,hwndCnr,iPosition)

/*
 * Override method: wpOpen
 */
#define WPStartup2_wpOpen(somSelf,hwndCnr,ulView,param) \
	WPObject_wpOpen(somSelf,hwndCnr,ulView,param)

/*
 * Override method: wpObjectReady
 */
#define WPStartup2_wpObjectReady(somSelf,ulCode,refObject) \
	WPObject_wpObjectReady(somSelf,ulCode,refObject)

/*
 * New Method: isSet
 */
typedef BOOL   SOMLINK somTP_WPStartup2_isSet(WPStartup2 *somSelf);
#pragma linkage(somTP_WPStartup2_isSet, system)
typedef somTP_WPStartup2_isSet *somTD_WPStartup2_isSet;
#define somMD_WPStartup2_isSet "::WPStartup2::isSet"
#define WPStartup2_isSet(somSelf) \
    (SOM_Resolve(somSelf, WPStartup2, isSet) \
	(somSelf))
#ifndef SOMGD_isSet
    #if (defined(_isSet) || defined(__isSet))
        #undef _isSet
        #undef __isSet
        #define SOMGD_isSet 1
    #else
        #define _isSet WPStartup2_isSet
    #endif /* _isSet */
#endif /* SOMGD_isSet */

/*
 * New Method: markSet
 */
typedef BOOL   SOMLINK somTP_WPStartup2_markSet(WPStartup2 *somSelf,
		HOBJECT hObj);
#pragma linkage(somTP_WPStartup2_markSet, system)
typedef somTP_WPStartup2_markSet *somTD_WPStartup2_markSet;
#define somMD_WPStartup2_markSet "::WPStartup2::markSet"
#define WPStartup2_markSet(somSelf,hObj) \
    (SOM_Resolve(somSelf, WPStartup2, markSet) \
	(somSelf,hObj))
#ifndef SOMGD_markSet
    #if (defined(_markSet) || defined(__markSet))
        #undef _markSet
        #undef __markSet
        #define SOMGD_markSet 1
    #else
        #define _markSet WPStartup2_markSet
    #endif /* _markSet */
#endif /* SOMGD_markSet */
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define WPStartup2_wpSetFldrFlags WPFolder_wpSetFldrFlags
#define WPStartup2_wpQueryFldrFlags WPFolder_wpQueryFldrFlags
#define WPStartup2_wpSetFldrFont WPFolder_wpSetFldrFont
#define WPStartup2_wpQueryFldrFont WPFolder_wpQueryFldrFont
#define WPStartup2_wpSetFldrAttr WPFolder_wpSetFldrAttr
#define WPStartup2_wpQueryFldrAttr WPFolder_wpQueryFldrAttr
#define WPStartup2_wpSetNextIconPos WPFolder_wpSetNextIconPos
#define WPStartup2_wpQueryNextIconPos WPFolder_wpQueryNextIconPos
#define WPStartup2_wpPopulate WPFolder_wpPopulate
#define WPStartup2_wpQueryContent WPFolder_wpQueryContent
#define WPStartup2_wpAddFolderView1Page WPFolder_wpAddFolderView1Page
#define WPStartup2_wpAddFolderView2Page WPFolder_wpAddFolderView2Page
#define WPStartup2_wpAddFolderView3Page WPFolder_wpAddFolderView3Page
#define WPStartup2_wpAddFolderIncludePage WPFolder_wpAddFolderIncludePage
#define WPStartup2_wpAddFolderSortPage WPFolder_wpAddFolderSortPage
#define WPStartup2_wpAddFolderBackgroundPage WPFolder_wpAddFolderBackgroundPage
#define WPStartup2_wpAddFolderMenu2Page WPFolder_wpAddFolderMenu2Page
#define WPStartup2_wpAddFolderSelfClosePage WPFolder_wpAddFolderSelfClosePage
#define WPStartup2_wpInitIconPosData WPFolder_wpInitIconPosData
#define WPStartup2_wpFreeIconPosData WPFolder_wpFreeIconPosData
#define WPStartup2_wpStoreIconPosData WPFolder_wpStoreIconPosData
#define WPStartup2_wpQueryIconPosition WPFolder_wpQueryIconPosition
#define WPStartup2_wpSetFldrSort WPFolder_wpSetFldrSort
#define WPStartup2_wpQueryFldrSort WPFolder_wpQueryFldrSort
#define WPStartup2_wpRestoreFldrRunObjs WPFolder_wpRestoreFldrRunObjs
#define WPStartup2_wpStoreFldrRunObjs WPFolder_wpStoreFldrRunObjs
#define WPStartup2_wpHideFldrRunObjs WPFolder_wpHideFldrRunObjs
#define WPStartup2_wpDeleteContents WPFolder_wpDeleteContents
#define WPStartup2_wpSetFldrDetailsClass WPFolder_wpSetFldrDetailsClass
#define WPStartup2_wpQueryFldrDetailsClass WPFolder_wpQueryFldrDetailsClass
#define WPStartup2_wpSearchFolder WPFolder_wpSearchFolder
#define WPStartup2_wpContainsFolders WPFolder_wpContainsFolders
#define WPStartup2_wpQueryOpenFolders WPFolder_wpQueryOpenFolders
#define WPStartup2_wpModifyFldrFlags WPFolder_wpModifyFldrFlags
#define WPStartup2_wpAddToContent WPFolder_wpAddToContent
#define WPStartup2_wpDeleteFromContent WPFolder_wpDeleteFromContent
#define WPStartup2_wpSetDetailsColumnVisibility WPFolder_wpSetDetailsColumnVisibility
#define WPStartup2_wpIsDetailsColumnVisible WPFolder_wpIsDetailsColumnVisible
#define WPStartup2_wpSetFldrSortClass WPFolder_wpSetFldrSortClass
#define WPStartup2_wpQueryFldrSortClass WPFolder_wpQueryFldrSortClass
#define WPStartup2_wpSetSortAttribAvailable WPFolder_wpSetSortAttribAvailable
#define WPStartup2_wpIsSortAttribAvailable WPFolder_wpIsSortAttribAvailable
#define WPStartup2_wpQueryIconViewPos WPFolder_wpQueryIconViewPos
#define WPStartup2_wpAddFirstChild WPFolder_wpAddFirstChild
#define WPStartup2_wpFlushNotifications WPFolder_wpFlushNotifications
#define WPStartup2_wpSetMenuBarVisibility WPFolder_wpSetMenuBarVisibility
#define WPStartup2_wpQueryMenuBarVisibility WPFolder_wpQueryMenuBarVisibility
#define WPStartup2_wpQueryIconTextBackgroundColor WPFolder_wpQueryIconTextBackgroundColor
#define WPStartup2_wpSetIconTextBackgroundColor WPFolder_wpSetIconTextBackgroundColor
#define WPStartup2_wpQueryIconTextColor WPFolder_wpQueryIconTextColor
#define WPStartup2_wpSetIconTextColor WPFolder_wpSetIconTextColor
#define WPStartup2_wpQueryShadowTextColor WPFolder_wpQueryShadowTextColor
#define WPStartup2_wpSetShadowTextColor WPFolder_wpSetShadowTextColor
#define WPStartup2_wpQueryIconTextVisibility WPFolder_wpQueryIconTextVisibility
#define WPStartup2_wpSetIconTextVisibility WPFolder_wpSetIconTextVisibility
#define WPStartup2_wpQueryFldrBackground WPFolder_wpQueryFldrBackground
#define WPStartup2_wpSetFldrBackground WPFolder_wpSetFldrBackground
#define WPStartup2_wpSetRealName WPFileSystem_wpSetRealName
#define WPStartup2_wpSetType WPFileSystem_wpSetType
#define WPStartup2_wpQueryType WPFileSystem_wpQueryType
#define WPStartup2_wpSetAttr WPFileSystem_wpSetAttr
#define WPStartup2_wpQueryAttr WPFileSystem_wpQueryAttr
#define WPStartup2_wpAddFile1Page WPFileSystem_wpAddFile1Page
#define WPStartup2_wpAddFile2Page WPFileSystem_wpAddFile2Page
#define WPStartup2_wpAddFile3Page WPFileSystem_wpAddFile3Page
#define WPStartup2_wpAddFileMenuPage WPFileSystem_wpAddFileMenuPage
#define WPStartup2_wpQueryCreation WPFileSystem_wpQueryCreation
#define WPStartup2_wpQueryLastAccess WPFileSystem_wpQueryLastAccess
#define WPStartup2_wpQueryLastWrite WPFileSystem_wpQueryLastWrite
#define WPStartup2_wpQueryFileSize WPFileSystem_wpQueryFileSize
#define WPStartup2_wpQueryEASize WPFileSystem_wpQueryEASize
#define WPStartup2_wpSetDateInfo WPFileSystem_wpSetDateInfo
#define WPStartup2_wpSetFileSizeInfo WPFileSystem_wpSetFileSizeInfo
#define WPStartup2_wpRefresh WPFileSystem_wpRefresh
#define WPStartup2_wpQueryRefreshFlags WPFileSystem_wpQueryRefreshFlags
#define WPStartup2_wpSetRefreshFlags WPFileSystem_wpSetRefreshFlags
#define WPStartup2_wpPrintPlainTextFile WPFileSystem_wpPrintPlainTextFile
#define WPStartup2_wpSetTitleAndRenameFile WPFileSystem_wpSetTitleAndRenameFile
#define WPStartup2_wpConfirmRenameFileWithExt WPFileSystem_wpConfirmRenameFileWithExt
#define WPStartup2_wpVerifyUpdateAccess WPFileSystem_wpVerifyUpdateAccess
#define WPStartup2_wpQueryEASupport WPFileSystem_wpQueryEASupport
#define WPStartup2_wpAddUserItemsToPopupMenu WPFileSystem_wpAddUserItemsToPopupMenu
#define WPStartup2_wpIsDiskSwapped WPFileSystem_wpIsDiskSwapped
#define WPStartup2_wpQueryRealName WPFileSystem_wpQueryRealName
#define WPStartup2_wpQueryFilename WPFileSystem_wpQueryFilename
#define WPStartup2_wpQueryDisk WPFileSystem_wpQueryDisk
#define WPStartup2_wpQueryDateInfo WPFileSystem_wpQueryDateInfo
#define WPStartup2_wpConfirmKeepAssoc WPFileSystem_wpConfirmKeepAssoc
#define WPStartup2_wpAddObjectGeneralPage WPObject_wpAddObjectGeneralPage
#define WPStartup2_wpAddObjectGeneralPage2 WPObject_wpAddObjectGeneralPage2
#define WPStartup2_wpAddObjectWindowPage WPObject_wpAddObjectWindowPage
#define WPStartup2_wpAddSettingsPages WPObject_wpAddSettingsPages
#define WPStartup2_wpAddToObjUseList WPObject_wpAddToObjUseList
#define WPStartup2_wpAllocMem WPObject_wpAllocMem
#define WPStartup2_wpAppendObject WPObject_wpAppendObject
#define WPStartup2_wpAssertObjectMutexSem WPObject_wpAssertObjectMutexSem
#define WPStartup2_wpClose WPObject_wpClose
#define WPStartup2_wpCnrInsertObject WPObject_wpCnrInsertObject
#define WPStartup2_wpCnrRemoveObject WPObject_wpCnrRemoveObject
#define WPStartup2_wpCnrSetEmphasis WPObject_wpCnrSetEmphasis
#define WPStartup2_wpConfirmDelete WPObject_wpConfirmDelete
#define WPStartup2_wpConfirmObjectTitle WPObject_wpConfirmObjectTitle
#define WPStartup2_wpCopiedFromTemplate WPObject_wpCopiedFromTemplate
#define WPStartup2_wpCopyObject WPObject_wpCopyObject
#define WPStartup2_wpCreateAnother WPObject_wpCreateAnother
#define WPStartup2_wpCreateFromTemplate WPObject_wpCreateFromTemplate
#define WPStartup2_wpCreateShadowObject WPObject_wpCreateShadowObject
#define WPStartup2_wpDelete WPObject_wpDelete
#define WPStartup2_wpDeleteFromObjUseList WPObject_wpDeleteFromObjUseList
#define WPStartup2_wpDisplayHelp WPObject_wpDisplayHelp
#define WPStartup2_wpDisplayMenu WPObject_wpDisplayMenu
#define WPStartup2_wpDoesObjectMatch WPObject_wpDoesObjectMatch
#define WPStartup2_wpDraggedOverObject WPObject_wpDraggedOverObject
#define WPStartup2_wpDragOver WPObject_wpDragOver
#define WPStartup2_wpDrop WPObject_wpDrop
#define WPStartup2_wpDroppedOnObject WPObject_wpDroppedOnObject
#define WPStartup2_wpEndConversation WPObject_wpEndConversation
#define WPStartup2_wpFilterPopupMenu WPObject_wpFilterPopupMenu
#define WPStartup2_wpFilterMenu WPObject_wpFilterMenu
#define WPStartup2_wpFindUseItem WPObject_wpFindUseItem
#define WPStartup2_wpFormatDragItem WPObject_wpFormatDragItem
#define WPStartup2_wpFree WPObject_wpFree
#define WPStartup2_wpFreeMem WPObject_wpFreeMem
#define WPStartup2_wpHide WPObject_wpHide
#define WPStartup2_wpInitData WPObject_wpInitData
#define WPStartup2_wpInsertPopupMenuItems WPObject_wpInsertPopupMenuItems
#define WPStartup2_wpInsertMenuItems WPObject_wpInsertMenuItems
#define WPStartup2_wpInsertSettingsPage WPObject_wpInsertSettingsPage
#define WPStartup2_wpMenuItemHelpSelected WPObject_wpMenuItemHelpSelected
#define WPStartup2_wpModifyMenu WPObject_wpModifyMenu
#define WPStartup2_wpMoveObject WPObject_wpMoveObject
#define WPStartup2_wpPrintObject WPObject_wpPrintObject
#define WPStartup2_wpQueryConcurrentView WPObject_wpQueryConcurrentView
#define WPStartup2_wpQueryButtonAppearance WPObject_wpQueryButtonAppearance
#define WPStartup2_wpQueryConfirmations WPObject_wpQueryConfirmations
#define WPStartup2_wpQueryDefaultHelp WPObject_wpQueryDefaultHelp
#define WPStartup2_wpQueryDefaultView WPObject_wpQueryDefaultView
#define WPStartup2_wpQueryDetailsData WPObject_wpQueryDetailsData
#define WPStartup2_wpQueryError WPObject_wpQueryError
#define WPStartup2_wpSetFolder WPObject_wpSetFolder
#define WPStartup2_wpQueryFolder WPObject_wpQueryFolder
#define WPStartup2_wpQueryIcon WPObject_wpQueryIcon
#define WPStartup2_wpQueryIconData WPObject_wpQueryIconData
#define WPStartup2_wpQueryMinWindow WPObject_wpQueryMinWindow
#define WPStartup2_wpQueryNameClashOptions WPObject_wpQueryNameClashOptions
#define WPStartup2_wpQueryStyle WPObject_wpQueryStyle
#define WPStartup2_wpSetTaskRec WPObject_wpSetTaskRec
#define WPStartup2_wpFindTaskRec WPObject_wpFindTaskRec
#define WPStartup2_wpQueryTitle WPObject_wpQueryTitle
#define WPStartup2_wpRegisterView WPObject_wpRegisterView
#define WPStartup2_wpReleaseObjectMutexSem WPObject_wpReleaseObjectMutexSem
#define WPStartup2_wpRender WPObject_wpRender
#define WPStartup2_wpRenderComplete WPObject_wpRenderComplete
#define WPStartup2_wpReplaceObject WPObject_wpReplaceObject
#define WPStartup2_wpRequestObjectMutexSem WPObject_wpRequestObjectMutexSem
#define WPStartup2_wpRestore WPObject_wpRestore
#define WPStartup2_wpRestoreData WPObject_wpRestoreData
#define WPStartup2_wpRestoreLong WPObject_wpRestoreLong
#define WPStartup2_wpRestoreState WPObject_wpRestoreState
#define WPStartup2_wpRestoreString WPObject_wpRestoreString
#define WPStartup2_wpSaveData WPObject_wpSaveData
#define WPStartup2_wpSaveImmediate WPObject_wpSaveImmediate
#define WPStartup2_wpSaveDeferred WPObject_wpSaveDeferred
#define WPStartup2_wpSaveLong WPObject_wpSaveLong
#define WPStartup2_wpSaveState WPObject_wpSaveState
#define WPStartup2_wpSaveString WPObject_wpSaveString
#define WPStartup2_wpScanSetupString WPObject_wpScanSetupString
#define WPStartup2_wpSetConcurrentView WPObject_wpSetConcurrentView
#define WPStartup2_wpSetButtonAppearance WPObject_wpSetButtonAppearance
#define WPStartup2_wpSetDefaultHelp WPObject_wpSetDefaultHelp
#define WPStartup2_wpSetDefaultView WPObject_wpSetDefaultView
#define WPStartup2_wpSetError WPObject_wpSetError
#define WPStartup2_wpQueryScreenGroupID WPObject_wpQueryScreenGroupID
#define WPStartup2_wpSetupOnce WPObject_wpSetupOnce
#define WPStartup2_wpSetIcon WPObject_wpSetIcon
#define WPStartup2_wpSetIconData WPObject_wpSetIconData
#define WPStartup2_wpSetMinWindow WPObject_wpSetMinWindow
#define WPStartup2_wpModifyStyle WPObject_wpModifyStyle
#define WPStartup2_wpSetTitle WPObject_wpSetTitle
#define WPStartup2_wpSetup WPObject_wpSetup
#define WPStartup2_wpSwitchTo WPObject_wpSwitchTo
#define WPStartup2_wpUnInitData WPObject_wpUnInitData
#define WPStartup2_wpViewObject WPObject_wpViewObject
#define WPStartup2_wpQueryTrueStyle WPObject_wpQueryTrueStyle
#define WPStartup2_wpQueryHandle WPObject_wpQueryHandle
#define WPStartup2_wpUnlockObject WPObject_wpUnlockObject
#define WPStartup2_wpIsObjectInitialized WPObject_wpIsObjectInitialized
#define WPStartup2_wpCreateShadowObjectExt WPObject_wpCreateShadowObjectExt
#define WPStartup2_wpCnrDeleteUseItem WPObject_wpCnrDeleteUseItem
#define WPStartup2_wpIsDeleteable WPObject_wpIsDeleteable
#define WPStartup2_wpQueryCoreRecord WPObject_wpQueryCoreRecord
#define WPStartup2_wpSetObjectID WPObject_wpSetObjectID
#define WPStartup2_wpQueryObjectID WPObject_wpQueryObjectID
#define WPStartup2_wpSetDefaultIconPos WPObject_wpSetDefaultIconPos
#define WPStartup2_wpQueryDefaultIconPos WPObject_wpQueryDefaultIconPos
#define WPStartup2_wpCnrRefreshDetails WPObject_wpCnrRefreshDetails
#define WPStartup2_wpFindViewItem WPObject_wpFindViewItem
#define WPStartup2_wpLockObject WPObject_wpLockObject
#define WPStartup2_wpIsLocked WPObject_wpIsLocked
#define WPStartup2_wpQueryContainerFlagPtr WPObject_wpQueryContainerFlagPtr
#define WPStartup2_wpWaitForClose WPObject_wpWaitForClose
#define WPStartup2_wpSetMenuStyle WPObject_wpSetMenuStyle
#define WPStartup2_wpQueryMenuStyle WPObject_wpQueryMenuStyle
#define WPStartup2_wpIdentify WPObject_wpIdentify
#define WPStartup2_wpSetStyle WPObject_wpSetStyle
#define WPStartup2_somDefaultInit SOMObject_somDefaultInit
#define WPStartup2_somDestruct SOMObject_somDestruct
#define WPStartup2_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define WPStartup2_somDefaultAssign SOMObject_somDefaultAssign
#define WPStartup2_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define WPStartup2_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define WPStartup2_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define WPStartup2_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define WPStartup2_somDefaultVAssign SOMObject_somDefaultVAssign
#define WPStartup2_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define WPStartup2_somInit SOMObject_somInit
#define WPStartup2_somFree SOMObject_somFree
#define WPStartup2_somUninit SOMObject_somUninit
#define WPStartup2_somGetClass SOMObject_somGetClass
#define WPStartup2_somGetClassName SOMObject_somGetClassName
#define WPStartup2_somGetSize SOMObject_somGetSize
#define WPStartup2_somIsA SOMObject_somIsA
#define WPStartup2_somIsInstanceOf SOMObject_somIsInstanceOf
#define WPStartup2_somRespondsTo SOMObject_somRespondsTo
#define WPStartup2_somDispatch SOMObject_somDispatch
#define WPStartup2_somClassDispatch SOMObject_somClassDispatch
#define WPStartup2_somCastObj SOMObject_somCastObj
#define WPStartup2_somResetObj SOMObject_somResetObj
#define WPStartup2_somDispatchV SOMObject_somDispatchV
#define WPStartup2_somDispatchL SOMObject_somDispatchL
#define WPStartup2_somDispatchA SOMObject_somDispatchA
#define WPStartup2_somDispatchD SOMObject_somDispatchD
#define WPStartup2_somPrintSelf SOMObject_somPrintSelf
#define WPStartup2_somDumpSelf SOMObject_somDumpSelf
#define WPStartup2_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_WPStartup2_h */

#ifndef SOM_M_WPStartup2_h
#define SOM_M_WPStartup2_h


#include <som.h>

/*
 * -- This emitter treats Method Tokens as Thunks by default.
 * -- Use the sc modifier "nothunks" to change this default
 */
#undef somresolve_
#define somresolve_(obj,mToken) ((somMethodProc*)((void)obj, mToken))

/*
 * Define the class name as an object type
 */
#ifndef M_WPStartup2
#define M_WPStartup2 SOMObject
#endif
#include <wpfolder.h>

/*
 * Start of user-defined types:
 */

/*
 * End of user-defined types.
 */

#define M_WPStartup2_MajorVersion 1
#define M_WPStartup2_MinorVersion 1

/*
 * External name shortening if required
 */
#ifdef SOM_USE_SHORT_EXTERNAL_NAMES
#define M_WPStartup2NewClass c
#pragma linkage(c, system)
#define M_WPStartup2ClassData d
#define M_WPStartup2CClassData x
#endif /* SOM_USE_SHORT_EXTERNAL_NAMES */
/*--------------Migration------------*/
#define M_WPStartup2_classObj M_WPStartup2ClassData.classObject
#define _M_WPStartup2 M_WPStartup2_classObj

/*
 * Declare the class creation procedure
 */
SOMEXTERN SOMClass * SOMLINK M_WPStartup2NewClass(integer4 somtmajorVersion,
		integer4 somtminorVersion);
#pragma linkage(M_WPStartup2NewClass, system)

/*
 * Declare the class data structure
 */
SOMEXTERN struct M_WPStartup2ClassDataStructure {
	SOMClass *classObject;
	somMToken clsQueryModuleHandle;
} SOMDLINK M_WPStartup2ClassData;

/*
 * Declare the C specific class data structure
 */
SOMEXTERN struct M_WPStartup2CClassDataStructure {
	somMethodTabs parentMtab;
	somDToken instanceDataToken;
} SOMDLINK M_WPStartup2CClassData;

/*
 * New and Renew macros for M_WPStartup2
 */
#define M_WPStartup2New() \
   (_somNew(_M_WPStartup2 ? _M_WPStartup2 : M_WPStartup2NewClass(M_WPStartup2_MajorVersion, M_WPStartup2_MinorVersion)))
#define M_WPStartup2Renew(buf) \
   (_somRenew(_M_WPStartup2 ? _M_WPStartup2 : M_WPStartup2NewClass(M_WPStartup2_MajorVersion, M_WPStartup2_MinorVersion), buf))

/*
 * Override method: wpclsInitData
 */
#define M_WPStartup2_wpclsInitData(somSelf) \
	M_WPObject_wpclsInitData(somSelf)

/*
 * Override method: wpclsQueryTitle
 */
#define M_WPStartup2_wpclsQueryTitle(somSelf) \
	M_WPObject_wpclsQueryTitle(somSelf)

/*
 * Override method: wpclsQueryIconData
 */
#define M_WPStartup2_wpclsQueryIconData(somSelf,pIconInfo) \
	M_WPObject_wpclsQueryIconData(somSelf,pIconInfo)

/*
 * Override method: wpclsQueryIconDataN
 */
#define M_WPStartup2_wpclsQueryIconDataN(somSelf,pIconInfo,ulIconIndex) \
	M_WPFolder_wpclsQueryIconDataN(somSelf,pIconInfo,ulIconIndex)

/*
 * New Method: clsQueryModuleHandle
 */
typedef HMODULE   SOMLINK somTP_M_WPStartup2_clsQueryModuleHandle(M_WPStartup2 *somSelf);
#pragma linkage(somTP_M_WPStartup2_clsQueryModuleHandle, system)
typedef somTP_M_WPStartup2_clsQueryModuleHandle *somTD_M_WPStartup2_clsQueryModuleHandle;
#define somMD_M_WPStartup2_clsQueryModuleHandle "::M_WPStartup2::clsQueryModuleHandle"
#define M_WPStartup2_clsQueryModuleHandle(somSelf) \
    (SOM_Resolve(somSelf, M_WPStartup2, clsQueryModuleHandle) \
	(somSelf))
#ifndef SOMGD_clsQueryModuleHandle
    #if (defined(_clsQueryModuleHandle) || defined(__clsQueryModuleHandle))
        #undef _clsQueryModuleHandle
        #undef __clsQueryModuleHandle
        #define SOMGD_clsQueryModuleHandle 1
    #else
        #define _clsQueryModuleHandle M_WPStartup2_clsQueryModuleHandle
    #endif /* _clsQueryModuleHandle */
#endif /* SOMGD_clsQueryModuleHandle */
#ifndef SOM_DONT_USE_INHERITED_MACROS
#define M_WPStartup2_wpclsQueryIconN M_WPFolder_wpclsQueryIconN
#define M_WPStartup2_wpclsQueryOpenFolders M_WPFolder_wpclsQueryOpenFolders
#define M_WPStartup2_wpclsQueryAwakeObject M_WPFileSystem_wpclsQueryAwakeObject
#define M_WPStartup2_wpclsFileSysExists M_WPFileSystem_wpclsFileSysExists
#define M_WPStartup2_wpclsQueryObjectFromPath M_WPFileSystem_wpclsQueryObjectFromPath
#define M_WPStartup2_wpclsQueryInstanceType M_WPFileSystem_wpclsQueryInstanceType
#define M_WPStartup2_wpclsQueryInstanceFilter M_WPFileSystem_wpclsQueryInstanceFilter
#define M_WPStartup2_wpclsQueryInstanceClass M_WPFileSystem_wpclsQueryInstanceClass
#define M_WPStartup2_wpclsCreateDefaultTemplates M_WPObject_wpclsCreateDefaultTemplates
#define M_WPStartup2_wpclsMakeAwake M_WPObject_wpclsMakeAwake
#define M_WPStartup2_wpclsNew M_WPObject_wpclsNew
#define M_WPStartup2_wpclsQueryDefaultHelp M_WPObject_wpclsQueryDefaultHelp
#define M_WPStartup2_wpclsQueryDefaultView M_WPObject_wpclsQueryDefaultView
#define M_WPStartup2_wpclsQueryDetails M_WPObject_wpclsQueryDetails
#define M_WPStartup2_wpclsQueryDetailsInfo M_WPObject_wpclsQueryDetailsInfo
#define M_WPStartup2_wpclsQueryExtendedCriteria M_WPObject_wpclsQueryExtendedCriteria
#define M_WPStartup2_wpclsQueryFolder M_WPObject_wpclsQueryFolder
#define M_WPStartup2_wpclsQueryIcon M_WPObject_wpclsQueryIcon
#define M_WPStartup2_wpclsQuerySearchInfo M_WPObject_wpclsQuerySearchInfo
#define M_WPStartup2_wpclsQueryStyle M_WPObject_wpclsQueryStyle
#define M_WPStartup2_wpclsUnInitData M_WPObject_wpclsUnInitData
#define M_WPStartup2_wpclsFindObjectFirst M_WPObject_wpclsFindObjectFirst
#define M_WPStartup2_wpclsFindObjectNext M_WPObject_wpclsFindObjectNext
#define M_WPStartup2_wpclsFindObjectEnd M_WPObject_wpclsFindObjectEnd
#define M_WPStartup2_wpclsSetError M_WPObject_wpclsSetError
#define M_WPStartup2_wpclsQueryError M_WPObject_wpclsQueryError
#define M_WPStartup2_wpclsQuerySettingsPageSize M_WPObject_wpclsQuerySettingsPageSize
#define M_WPStartup2_wpclsQueryObject M_WPObject_wpclsQueryObject
#define M_WPStartup2_wpclsQueryButtonAppearance M_WPObject_wpclsQueryButtonAppearance
#define M_WPStartup2_wpclsDecUsage M_WPObject_wpclsDecUsage
#define M_WPStartup2_wpclsIncUsage M_WPObject_wpclsIncUsage
#define M_WPStartup2_wpclsInsertMultipleObjects M_WPObject_wpclsInsertMultipleObjects
#define M_WPStartup2_wpclsFindOneObject M_WPObject_wpclsFindOneObject
#define M_WPStartup2_wpclsSetSettingsPageSize M_WPObject_wpclsSetSettingsPageSize
#define M_WPStartup2_wpclsSetIconData M_WPObject_wpclsSetIconData
#define M_WPStartup2_wpclsSetIcon M_WPObject_wpclsSetIcon
#define M_WPStartup2_wpclsObjectFromHandle M_WPObject_wpclsObjectFromHandle
#define M_WPStartup2_wpclsRemoveObjects M_WPObject_wpclsRemoveObjects
#define M_WPStartup2__get_somDataAlignment SOMClass__get_somDataAlignment
#define M_WPStartup2__get_somInstanceDataOffsets SOMClass__get_somInstanceDataOffsets
#define M_WPStartup2__get_somDirectInitClasses SOMClass__get_somDirectInitClasses
#define M_WPStartup2_somNew SOMClass_somNew
#define M_WPStartup2_somNewNoInit SOMClass_somNewNoInit
#define M_WPStartup2_somRenew SOMClass_somRenew
#define M_WPStartup2_somRenewNoInit SOMClass_somRenewNoInit
#define M_WPStartup2_somRenewNoZero SOMClass_somRenewNoZero
#define M_WPStartup2_somRenewNoInitNoZero SOMClass_somRenewNoInitNoZero
#define M_WPStartup2_somAllocate SOMClass_somAllocate
#define M_WPStartup2_somDeallocate SOMClass_somDeallocate
#define M_WPStartup2_somInitClass SOMClass_somInitClass
#define M_WPStartup2_somInitMIClass SOMClass_somInitMIClass
#define M_WPStartup2_somAddStaticMethod SOMClass_somAddStaticMethod
#define M_WPStartup2_somAddDynamicMethod SOMClass_somAddDynamicMethod
#define M_WPStartup2_somOverrideSMethod SOMClass_somOverrideSMethod
#define M_WPStartup2_somClassReady SOMClass_somClassReady
#define M_WPStartup2_somGetApplyStub SOMClass_somGetApplyStub
#define M_WPStartup2_somGetClassData SOMClass_somGetClassData
#define M_WPStartup2_somSetClassData SOMClass_somSetClassData
#define M_WPStartup2_somGetClassMtab SOMClass_somGetClassMtab
#define M_WPStartup2_somGetInstanceOffset SOMClass_somGetInstanceOffset
#define M_WPStartup2_somGetInstancePartSize SOMClass_somGetInstancePartSize
#define M_WPStartup2_somGetInstanceSize SOMClass_somGetInstanceSize
#define M_WPStartup2_somGetInstanceToken SOMClass_somGetInstanceToken
#define M_WPStartup2_somGetMemberToken SOMClass_somGetMemberToken
#define M_WPStartup2_somGetMethodData SOMClass_somGetMethodData
#define M_WPStartup2_somGetRdStub SOMClass_somGetRdStub
#define M_WPStartup2_somGetMethodDescriptor SOMClass_somGetMethodDescriptor
#define M_WPStartup2_somGetMethodIndex SOMClass_somGetMethodIndex
#define M_WPStartup2_somGetMethodToken SOMClass_somGetMethodToken
#define M_WPStartup2_somGetName SOMClass_somGetName
#define M_WPStartup2_somGetNthMethodData SOMClass_somGetNthMethodData
#define M_WPStartup2_somGetNthMethodInfo SOMClass_somGetNthMethodInfo
#define M_WPStartup2_somGetNumMethods SOMClass_somGetNumMethods
#define M_WPStartup2_somGetNumStaticMethods SOMClass_somGetNumStaticMethods
#define M_WPStartup2_somGetParent SOMClass_somGetParent
#define M_WPStartup2_somGetParents SOMClass_somGetParents
#define M_WPStartup2_somGetVersionNumbers SOMClass_somGetVersionNumbers
#define M_WPStartup2_somFindMethod SOMClass_somFindMethod
#define M_WPStartup2_somFindMethodOk SOMClass_somFindMethodOk
#define M_WPStartup2_somFindSMethod SOMClass_somFindSMethod
#define M_WPStartup2_somFindSMethodOk SOMClass_somFindSMethodOk
#define M_WPStartup2_somLookupMethod SOMClass_somLookupMethod
#define M_WPStartup2_somCheckVersion SOMClass_somCheckVersion
#define M_WPStartup2_somDescendedFrom SOMClass_somDescendedFrom
#define M_WPStartup2_somSupportsMethod SOMClass_somSupportsMethod
#define M_WPStartup2_somDefinedMethod SOMClass_somDefinedMethod
#define M_WPStartup2_somOverrideMtab SOMClass_somOverrideMtab
#define M_WPStartup2_somDefaultInit SOMObject_somDefaultInit
#define M_WPStartup2_somDestruct SOMObject_somDestruct
#define M_WPStartup2_somDefaultCopyInit SOMObject_somDefaultCopyInit
#define M_WPStartup2_somDefaultAssign SOMObject_somDefaultAssign
#define M_WPStartup2_somDefaultConstCopyInit SOMObject_somDefaultConstCopyInit
#define M_WPStartup2_somDefaultVCopyInit SOMObject_somDefaultVCopyInit
#define M_WPStartup2_somDefaultConstVCopyInit SOMObject_somDefaultConstVCopyInit
#define M_WPStartup2_somDefaultConstAssign SOMObject_somDefaultConstAssign
#define M_WPStartup2_somDefaultVAssign SOMObject_somDefaultVAssign
#define M_WPStartup2_somDefaultConstVAssign SOMObject_somDefaultConstVAssign
#define M_WPStartup2_somInit SOMObject_somInit
#define M_WPStartup2_somFree SOMObject_somFree
#define M_WPStartup2_somUninit SOMObject_somUninit
#define M_WPStartup2_somGetClass SOMObject_somGetClass
#define M_WPStartup2_somGetClassName SOMObject_somGetClassName
#define M_WPStartup2_somGetSize SOMObject_somGetSize
#define M_WPStartup2_somIsA SOMObject_somIsA
#define M_WPStartup2_somIsInstanceOf SOMObject_somIsInstanceOf
#define M_WPStartup2_somRespondsTo SOMObject_somRespondsTo
#define M_WPStartup2_somDispatch SOMObject_somDispatch
#define M_WPStartup2_somClassDispatch SOMObject_somClassDispatch
#define M_WPStartup2_somCastObj SOMObject_somCastObj
#define M_WPStartup2_somResetObj SOMObject_somResetObj
#define M_WPStartup2_somDispatchV SOMObject_somDispatchV
#define M_WPStartup2_somDispatchL SOMObject_somDispatchL
#define M_WPStartup2_somDispatchA SOMObject_somDispatchA
#define M_WPStartup2_somDispatchD SOMObject_somDispatchD
#define M_WPStartup2_somPrintSelf SOMObject_somPrintSelf
#define M_WPStartup2_somDumpSelf SOMObject_somDumpSelf
#define M_WPStartup2_somDumpSelfInt SOMObject_somDumpSelfInt
#endif /* SOM_DONT_USE_INHERITED_MACROS */

#endif       /* SOM_M_WPStartup2_h */
