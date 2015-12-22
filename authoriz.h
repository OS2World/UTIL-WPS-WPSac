/******************************************************************************
    Header-File fÅr WPSac.dll / Authoriz.dll
******************************************************************************/

/* Namen der DLL von BTB und der darin enthaltenen Funktionen */
#define AUTH_DLLNAME    "AUTHORIZ"      // DLL fÅr WPSac
#define AUTH_DLLENTRTY  "Authorize"     // Entry fÅr WPSac

/* Key-Value fÅr die WinSetObjectData Funktion */
#define SETUP_WPSBAC    "WPSBAC"

/* keyvalues fÅr die _ulMisc-Variable */
#define DONT_ASK_ANY_MORE  0x0001       // Das Objekt soll keine weiteren Aufrufe der
                                        // BTB_AUTHORIZE-Funktion mehr vornehmen, um
                                        // seine Attribute zu verÑndern.
#define NO_DEFAULT_OPEN    0x0002       // Au·erbetriebnahme des Doppelklicks

/* Struktur fÅr die ParameterÅbergabe zu und von der Authorisierungsfunktion */
#define OBJDESCR_VERSION   1

typedef struct _OBJDESCR
    {
    // Bidirektionale öbergabefelder
    ULONG   ulVersion;                  // Version dieser Programmschnittstelle (OBJDESCR_VERSION)
    ULONG   ulMisc;
    ULONG   ulAND_CTXT;
    ULONG   ulOR_CTXT;
    ULONG   ulAND_STYLE;
    ULONG   ulOR_STYLE;

    // öbergabe zur Authorize-Funktion
    PCHAR   szTitle;                    // Titel des Objects
    PCHAR   szType;                     // Type-Attribut des Objects z.B: "Plain Text\nC Code"
    PCHAR   szPhysName;                 // Physikalischer Name
    PCHAR   szDefaultTitle;             // Default Titel der Objektklasse, z.B.: "Datendatei"
    PCHAR   szObjectClass;              // z.B.: "WPFolder", "WPObject"
    } OBJDESCR;

