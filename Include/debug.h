#ifndef _DEBUG_
#define _DEBUG_

/* Private Debug-Hilfroutinen fr PMPRINTF */
#ifdef DEBUG
  extern ULONG ulDebugMask;
  extern CHAR  szDebug[128];

  /* DEBUG-Maske */
  #define D_AUTH    0x00000001      // Kommunikation mit AUTHORIZ.DLL
  #define D_SOM     0x00000002      // SOM-Methoden
  #define D_SAM     0x00000004      // Schalter fr Sammy

  /* Hilfe-Text fr DEBUG-Maske */
  #define D_TEXT_MASK   "\
D_AUTH    Kommunikation mit Authoriz   0x00000001\n\
D_SOM     SOM-Methoden                 0x00000002\n\
D_SAM     Schalter fr Sammy           0x00000004\n"

#ifdef LOGFILE
  #define DEBPRINT              wrtprintf
  int   wrtprintf (char *f, ...);
#else
  #define DEBPRINT              pmprintf
  int   pmprintf (char *f, ...);
#endif

  #define DebugS(m,title)         DEBPRINT (m&ulDebugMask?"%s| %s\n":"", CLASSNAME, title)
  #define DebugE(m,title,text)    DEBPRINT (m&ulDebugMask?"%s| %s => %s\n":"", CLASSNAME, title, text)
  #define DebugULd(m,title,s,ul)  DEBPRINT (m&ulDebugMask?"%s| %s => %s: %d\n":"", CLASSNAME, title, s, ul)
  #define DebugULx(m,title,s,ul)  DEBPRINT (m&ulDebugMask?"%s| %s => %s: 0x%X\n":"", CLASSNAME, title, s, ul)
#else
  #define DebugS(m,title)         {}
  #define DebugE(m,title,text)    {}
  #define DebugULd(m,title,s,ul)  {}
  #define DebugULx(m,title,s,ul)  {}
#endif /* DEBUG */

#endif /* _DEBUG_ */
