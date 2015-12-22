#ifndef MAGPRO2_H
#define MAGPRO2_H

/*** Application & Key in OS2SYS.INI ***/
#define OS2SYS_APP_NAME         "WPSac"
#define OS2SYS_KEY_INI_PATH     "UprofileINIPath"   // Pfad der INI-Datei fÅr UPRPOFILE
#define OS2SYS_KEY_INI_FILE     "UprofileINIFile"   // Dateiname der INI-Datei fÅr UPRPOFILE
#define OS2SYS_KEY_CLNT_LOCAL   "ClientPathLocal"   // lokaler Pfad fÅr INI-Datei, *sac/*drc-Dateien
#define OS2SYS_KEY_CLNT_LAN     "ClientPathLAN"     // LAN-Pfad fÅr INI-Datei, *sac/*drc-Dateien
#define OS2SYS_KEY_LANTIMEOUT   "LANTimeout"        // Wartezeit auf LAN-Laufwerk bei Programmstart

/*** Applications in MAGPRO2.INI ***/
#define MAGPRO_FILE_APP         "_Files"
#define MAGPRO_DESKTOP_APP      "_Desktops"
#define MAGPRO_SYSTEM_APP       "_System"
#define MAGPRO_SCRIPT_APP       "_Scripts"
#define MAGPRO_ENVIRONM_APP     "_Env"
#define MAGPRO_PRELOGON_APP     "_Prelogon"

/*** Keys in MAGPRO2.INI ***/
// App: MAGPRO_FILE_APP
#define MAGPRO_LANG_KEY         "Language"          // Sprach-DLL fÅr UPROFILE
#define MAGPRO_BASERESOURCE_KEY "BaseResource"      // Basis-Resource-Datei (Default: ini.rc)
#define MAGPRO_ICONSRC_KEY      "IconSource"        // Original-Icon-Pfad von Deskman/2

// App: MAGPRO_SYSTEM_APP
#define MAGPRO_KEY_DT_PATH      "DesktopPath"       // Pfad fÅr ArbeitsoberflÑchen
#define MAGPRO_KEY_SAMMY_BGCLR  "SammyBgColor"      // Hintergrundfarbe fÅr Sammy
#define MAGPRO_KEY_SAMMY_BGBMP  "SammyBgBitmap"     // Hintergrundbild fÅr Sammy
#define MAGPRO_KEY_LASTUSER     "SammyLastuser"     // Letzter eingeloggter Benutzer
#define MAGPRO_CO_FLAG_KEY      "CO_FLAG"
#define MAGPRO_PWD_INTVL_KEY    "PWD_VALID"         // Pa·wort-GÅltigkeitszeitraum in Tagen
#define MAGPRO_TIME_KEY         "TIMESTAMP"         // Uprofile-interner Timestamp (nicht fÅr WPSAC)
#define MAGPRO_WIN_X            "WIN_X"
#define MAGPRO_WIN_Y            "WIN_Y"
#define MAGPRO_WIN_WIDTH        "WIN_WIDTH"
#define MAGPRO_WIN_HEIGHT       "WIN_HEIGHT"
#define MAGPRO_FLAG_KEY         "FLAGS"


    // zulÑssige User+Passwd-Modi abhÑngig von den FLG_SYS_*-Flags:
    //
    // MC  CO  PM      User   Passwd ID-Card        MC: MagCalls installiert
    // 0   X   0        ja     opt    NA            CO: FLG_SYS_CARD_ONLY
    // 0   X   1        ja     ja     NA            PM: FLG_SYS_PWD_MANDATORY
    // 1   0   0        ja     opt    ja
    // 1   0   1        ja     ja     ja
    // 1   1   0        ja     opt    nein
    //            oder  nein   nein   ja
    // 1   1   1        ja     ja     nein
    //            oder  nein   nein   ja
#define PWD_CARD_ONLY_BIT       0
#define FLG_SYS_CARD_ONLY       0x0001          // Verwendung von Karte+User[+Passwd] ist obligatorisch
#define PWD_MANDATORY_BIT       1
#define FLG_SYS_PWD_MANDATORY   0x0002          // Passwd ist obligatorisch
#define ADMIN_LOCK_BIT          2
#define FLG_SYS_ADMIN_LOCK      0x0004          // Mindestens ein User ist Administrator

// App: MAGPRO_SCRIPT_APP       "_Scripts"
// Keys are script names

// App: MAGPRO_ENVIRONM_APP     "_Env"
// Keys are environment names

// App: MAGPRO_PRELOGON_APP     "_Prelogon"
#define MAGPRO_KEY_PRLOG_USER   "PrelogonUser"      // Prelogon User (max. CCHMAXNAMELEN)
#define MAGPRO_KEY_PRLOG_PWD    "PrelogonPasswd"    // Prelogon Password (max. CCHMAXNAMELEN)
#define MAGPRO_KEY_PRLOG_DOMAIN "PrelogonDomain"    // Prelogon Domain; mu· fÅr IBM LS mit "LS:" beginnen
                                                    //    (max. CCHMAXDOMAINNAME)

// App: MAGPRO_DESKTOP_APP
// Keys are desktop names

// All other App names are User names (max. CCHMAXUSERNAME)
#define MAGPRO_CARDCODE_KEY     "CardCode"          // (max. CCHMAXTRACKDATA)
#define MAGPRO_ADMINFLAG_KEY    "AdminFlag"         // (crypt)
#define MAGPRO_USERDESK_KEY     "DeskTop"
#define MAGPRO_SAC_USER_KEY     "SAC_User"
#define MAGPRO_SAC_GROUP_KEY    "SAC_Group"
#define MAGPRO_ICON_POS_X       "Icon_X"
#define MAGPRO_ICON_POS_Y       "Icon_Y"
#define MAGPRO_LASTMOD_KEY      "LastModTime"       // Timestamp fÅr énderungen an (*)
#define MAGPRO_TOCOPY_KEY       "T_Copy"            // (*)
#define MAGPRO_PASSWORD_KEY     "PassWord"          // (crypt)(*) (max. CCHMAXPASSWORD)
#define MAGPRO_PASSWORDAGE_KEY  "PassWordAge"       // (*)
#define MAGPRO_ENVLIST_KEY      "EnvList"
#define MAGPRO_SERVICE_KEY      "L_"                // (crypt)(*)
    // Inhalt des ServiceData-Strings (L_*); dieser hat die Form eines
    // Stringarrays: "<string1>\0<string2>\0<string3>\0...<stringN>\0"
    // Der enum-Wert entspricht der Position innerhalb des Arrays
    enum _string
        {
        USERNAME = 0,
        PASSWORD,
        DOMAIN,
        LOGIN,
        LOGOUT,
        CHANGEPWD,
        PWDAGE,
        NEWPASSWORD,
        NEWPWDAGE,
        MAXSTRING
        };

#endif /* MAGPRO2_H */
