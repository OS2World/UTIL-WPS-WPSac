#define IDD_LOGIN                   0x0100
#define IDD_EDIT                    0x0200
#define IDD_MESSAGEBOX              0x0300
#define IDD_ADMINMSG                0x0400
#define IDD_TIMEDMSG                0x0500
#define IDD_RXOUT                   0x0600
#define IDD_VIEW                    0x0700
#define IDD_ABOUTBOX                0x0800

/* allgemeine IDs */
#define DID_ICON                    0x0010
#define DID_HELP                    0x0011

/* IDD_LOGIN */
#define DID_LOGIN_TEXT_USER         0x0102
#define DID_LOGIN_USER              0x0103
#define DID_LOGIN_TEXT_PASSWORD     0x0104
#define DID_LOGIN_PASSWORD          0x0105
#define DID_LOGIN_TEXTIDCARD        0x0106
#define DID_SHUTDOWN                0x0107
#define DID_OPTION                  0x0108

/* IDD_TIMEDMSG */
#define DID_TEXT                    0x0501

/* IDD_EDIT */
#define DID_EDIT_USER               0x0201
#define DID_EDIT_OLDPWD             0x0202
#define DID_EDIT_PASSWORD           0x0203
#define DID_EDIT_VERIFICATION       0x0204
#define DID_TXT_PWDEXPIRED          0x0205

/* IDD_VIEW */
#define DID_VALUE_COLOR             0x0701
#define DID_ENTER_FILE              0x0702
#define DID_SEARCH_FILE             0x0703

/* IDD_LOGINMENU */
#define IDM_LOGIN                   0x2000
#define IDM_SHUTDOWN                0x2001
#define IDM_CHGPWD                  0x2002
#define IDM_VIEW                    0x2003
#define IDM_HELP                    0x2004
#define IDM_ABOUT                   0x2005

/* String-IDs fÅr Fehlermeldungen */
#define IDS_NOUSER                  0x1000  /* Benutzername ungÅltig */
#define IDS_DESKTPNOTFOUND          0x1001  /* Benutzer nicht gefunden */
#define IDS_USERDESTROYED           0x1002  /* Benutzerdaten inkonsistent */
#define IDS_SHELLNOTFOUND           0x1003  /* Angeforderte Shell nicht gefunden */
#define IDS_REGISTER                0x1004  /* Programm nicht registriert */
#define IDS_NOMAINWINDOW            0x1005  /* Main-Window konnte nicht erstellt werden */
#define IDS_USERINCOMPL             0x1006
#define IDS_PATHTOOLONG             0x1007
#define IDS_ERRCREATE               0x1008
#define IDS_USERWITHSYSTEMINI       0x1009
#define IDS_HELPWINDOWTITLE         0x100A
#define IDS_HELPLIBRARYNAME         0x100B
#define IDS_NOTMARRIED              0x100C
#define IDS_NODESKINRC              0x100D
#define IDS_WRONGPASSWORD           0x100E  /* falsches oder kein Kennwort angegeben */
#define IDS_USERNOTFOUND            0x100F  /* Benutzer oder INI-Datei nicht gefunden */
#define IDS_DIFFERENTPWDS           0x1010  /* Kennwîrter stimmen nicht Åberein */

#define IDS_USERPREPARE             0x1011
#define IDS_USERLOGON               0x1012
#define IDS_USERLOGOFF              0x1013
#define IDS_PRELOGON                0x1014

#define IDS_NOINIDATABASE           0x1015

#define IDS_ERRUNCPATH              0x1016
#define IDS_ERRUNCDRIVEWRITE        0x1017
#define IDS_ERRUNCDRIVEREAD         0x1018
#define IDS_ERRWRITELANPROFILE      0x1019

#define IDS_INVDOMAIN               0x101A
#define IDS_INVPATH                 0x101B
#define IDS_NOPRELOGUSER            0x101C
#define IDS_LOGON                   0x101D
#define IDS_LOGOFF                  0x101E
#define IDS_NODLL                   0x101F

#define IDS_LOGONTITLE              0x1020

#define IDS_DTPATHCHANGED           0x1021

#define IDS_DEFAULTUSER             0x1022
#define IDS_DEFAULTPASSWORD         0x1023
