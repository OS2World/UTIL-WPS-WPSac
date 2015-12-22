#include <stdio.h>
#include <string.h>

extern FILE *yyin;
extern FILE *yyout;
FILE *parsed;

main(int argc, char *argv[], char *envp[])
{
   char *szDest;
   long ulSize;

   if (argc < 2)
   {
      printf("Usage:\n");
      return 1;
   } /* endif */

   szDest = strrchr(argv[1], '.');
   ulSize = (szDest) ? (szDest - argv[1]) : strlen (argv[1]);
   szDest = (char *) malloc (ulSize+4);
   strncpy(szDest, argv[1], ulSize);
   szDest[ulSize] = '\0';
   strcat(szDest, ".SAC");

   yyin = fopen(argv[1], "r");
   yyout = stdout;
   parsed = fopen(szDest, "wb");

   yyparse();
   fclose (yyin);
   fclose (parsed);
}
