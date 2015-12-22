#include <os2.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

static BOOL bHasSeed = FALSE;

ULONG get1stPos (PUCHAR szBuffer, ULONG ulLen)
    {
    int i;

    for (i=0; i<ulLen; i++)
        if (szBuffer[i] == '\0')
            return i;

    return 0;
    }

PCHAR createPwd (PCHAR szBuffer, ULONG ulAlphaNumeric, ULONG ulAlpha, ULONG ulNumeric)
    {
    ULONG ulc;
    ULONG i, pos;
    CHAR  c;

    if (bHasSeed == FALSE)
        {
        srand (clock ());
        bHasSeed = TRUE;
        }

    /* Anzahl der Zeichen; Puffer l”schen */
    ulc = ulAlphaNumeric + ulAlpha + ulNumeric;
    memset (szBuffer, '\0', ulc+1);

    /* Alhpanumerische Zeichen setzen */
    for (i = 0; i < ulAlphaNumeric; i++)
        {
        pos = rand () * (ulc-1) / RAND_MAX;
        /* Um die Rechenzeit kurz zu halten, wird im Konfliktfall  */
        /* (Zeichen bereits belegt) das erste leere Zeichen belegt */
        if (szBuffer[pos])
            pos = get1stPos (szBuffer, ulc);
        c = rand () * 35 /RAND_MAX + '0';
        if (c > '9')
            c += 'A' - '9' - 1;
        szBuffer[pos] = c;
        }

    /* Alhpa-Zeichen setzen */
    for (i = 0; i < ulAlpha; i++)
        {
        pos = rand () * (ulc-1) / RAND_MAX;
        /* Um die Rechenzeit kurz zu halten, wird im Konfliktfall  */
        /* (Zeichen bereits belegt) das erste leere Zeichen belegt */
        if (szBuffer[pos])
            pos = get1stPos (szBuffer, ulc);
        szBuffer[pos] = rand () * 25 /RAND_MAX + 'A';
        }

    /* Numerische Zeichen setzen */
    for (i = 0; i < ulc; i++)
        {
        if (szBuffer[i])
            continue;
        szBuffer[i] = rand () * 9 /RAND_MAX + '0';
        }

    szBuffer[i] = '\0';

    return szBuffer;
    }
