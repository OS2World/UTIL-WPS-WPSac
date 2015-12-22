%token OBJ_BEGIN ENDOBJECT
%token OBJ_CLASS OBJ_TITLE OBJ_TYPE OBJ_DEFTITLE OBJ_PHYSNAME
%token STRING
%token MOVE NOMOVE
%token LINK NOLINK
%token COPY NOCOPY
%token DEFAULTICON NODEFAULTICON
%token TEMPLATE NOTEMPLATE
%token DELETE NODELETE
%token PRINT NOPRINT
%token DRAG NODRAG
%token VISIBLE NOTVISIBLE
%token SETTINGS NOSETTINGS
%token RENAME NORENAME
%token CREATEANOTHER NOCREATEANOTHER
%token NEW NONEW
%token OPEN NOOPEN
%token WINDOW NOWINDOW
%token CLOSE NOCLOSE
%token HELP NOHELP
%token PROGRAM NOPROGRAM
%token ICONVIEW NOICONVIEW
%token TREEVIEW NOTREEVIEW
%token DETAILSVIEW NODETAILSVIEW
%token FIND NOFIND
%token SELECT NOSELECT
%token ARRANGE NOARRANGE
%token SORT NOSORT
%token SHUTDOWN NOSHUTDOWN
%token LOCKUP NOLOCKUP
%token PALETTE NOPALETTE
%token DBLKLICK NODBLKLICK
%token '(' ')' '='
%token ERROR
%%

OBJS     : ONE_OBJ OBJS
         | ONE_OBJ;

ONE_OBJ    : OBJ_BEGIN                                {InitClass(); }
             '(' OBJ_HDRS ')' ANY_FLAGS ENDOBJECT     {WriteClass();} ;

OBJ_HDRS   : OBJ_HDR ',' OBJ_HDRS
           | OBJ_HDR ;

OBJ_HDR    : HDR_CLASS | HDR_TITLE | HDR_TYPE | HDR_DFTT | HDR_PHNM ;

HDR_CLASS  : OBJ_CLASS '=' MYSTRING { if (pchClass == NULL)
                                          pchClass = strdup((PCHAR)$3);
                                      else
                                          printf("CLASS double defined, second definition ignored\n"); };

HDR_TITLE  : OBJ_TITLE '=' MYSTRING { if (pchTitle == NULL)
                                          pchTitle = strdup((PCHAR)$3);
                                      else
                                          printf("TITLE double defined, second definition ignored\n"); };

HDR_TYPE   : OBJ_TYPE  '=' MYSTRING { if (pchType == NULL)
                                          pchType = strdup((PCHAR)$3);
                                      else
                                          printf("TYPE double defined, second definition ignored\n"); };

HDR_DFTT   : OBJ_DEFTITLE '=' MYSTRING { if (pchDefTitle == NULL)
                                             pchDefTitle = strdup((PCHAR)$3);
                                         else
                                             printf("DEFTITLE double defined, second definition ignored\n"); };

HDR_PHNM   : OBJ_PHYSNAME '=' MYSTRING { if (pchPhysName == NULL)
                                             pchPhysName = strdup((PCHAR)$3);
                                         else
                                             printf("PHYSNAME double defined, second definition ignored\n"); };

MYSTRING   : STRING { $$ = (LONG) pchMyString; };

ANY_FLAGS  : ANY_FLAG ANY_FLAGS
           | ANY_FLAG;

ANY_FLAG   : OBJ_MOVE | OBJ_LINK | OBJ_COPY | OBJ_ICON | OBJ_TEMP | OBJ_DEL  | OBJ_PRNT
           | OBJ_DRAG | OBJ_VIS  | OBJ_SET  | OBJ_REN  | OBJ_CREA | OBJ_NEW  | OBJ_OPEN
           | OBJ_WIN  | OBJ_CLOS | OBJ_HELP | OBJ_PROG | OBJ_ICVW | OBJ_TRVW | OBJ_DTVW
           | OBJ_FIND | OBJ_SEL  | OBJ_ARRG | OBJ_SORT | OBJ_SHUT | OBJ_LOCK | OBJ_PAL
           | OBJ_DBLK ;

OBJ_MOVE : MOVE   { style_and &= (~OBJSTYLE_NOMOVE); context_or  |=   CTXT_MOVE ; }
         | NOMOVE { style_or  |=   OBJSTYLE_NOMOVE ; context_and &= (~CTXT_MOVE); } ;

OBJ_LINK : LINK   { style_and &= (~OBJSTYLE_NOLINK); context_or  |=   CTXT_LINK ; }
         | NOLINK { style_or  |=   OBJSTYLE_NOLINK ; context_and &= (~CTXT_MOVE); } ;

OBJ_COPY : COPY   { style_and &= (~OBJSTYLE_NOCOPY); context_or  |=   CTXT_COPY ; }
         | NOCOPY { style_or  |=   OBJSTYLE_NOCOPY ; context_and &= (~CTXT_COPY); } ;

OBJ_ICON : DEFAULTICON   { style_and &= (~OBJSTYLE_NOTDEFAULTICON); }
         | NODEFAULTICON { style_or  |=   OBJSTYLE_NOTDEFAULTICON ; } ;

OBJ_TEMP : TEMPLATE   { style_and &= (~OBJSTYLE_TEMPLATE); }
         | NOTEMPLATE { style_or  |=   OBJSTYLE_TEMPLATE ; } ;

OBJ_DEL  : DELETE   { style_and &= (~OBJSTYLE_NODELETE); context_and |=   CTXT_DELETE ; }
         | NODELETE { style_or  |=   OBJSTYLE_NODELETE ; context_and &= (~CTXT_DELETE); } ;

OBJ_PRNT : PRINT   { style_and &= (~OBJSTYLE_NOPRINT); context_and |=   CTXT_PRINT ; }
         | NOPRINT { style_or  |=   OBJSTYLE_NOPRINT ; context_and &= (~CTXT_PRINT); } ;

OBJ_DRAG : DRAG   { style_and &= (~OBJSTYLE_NODRAG); }
         | NODRAG { style_or  |=   OBJSTYLE_NODRAG ; } ;

OBJ_VIS  : VISIBLE   { style_and &= (~OBJSTYLE_NOTVISIBLE); }
         | NOTVISIBLE { style_or  |=  OBJSTYLE_NOTVISIBLE  ; } ;

OBJ_SET  : SETTINGS   { style_and &= (~OBJSTYLE_NOSETTINGS); context_and |=   CTXT_SETTINGS ; }
         | NOSETTINGS { style_or  |=   OBJSTYLE_NOSETTINGS ; context_and &= (~CTXT_SETTINGS); } ;

OBJ_REN  : RENAME   { style_and &= (~OBJSTYLE_NORENAME); }
         | NORENAME { style_or  |=   OBJSTYLE_NORENAME ; } ;

OBJ_CREA : CREATEANOTHER   { context_or  |=   CTXT_CRANOTHER ; }
         | NOCREATEANOTHER { context_and &= (~CTXT_CRANOTHER); } ;

OBJ_NEW  : NEW   { context_or  |=   CTXT_NEW ; }
         | NONEW { context_and &= (~CTXT_NEW); } ;

OBJ_OPEN : OPEN   { context_or  |=   CTXT_OPEN ; }
         | NOOPEN { context_and &= (~CTXT_OPEN); } ;

OBJ_WIN  : WINDOW   { context_or  |=   CTXT_WINDOW ; }
         | NOWINDOW { context_and &= (~CTXT_WINDOW); } ;

OBJ_CLOS : CLOSE   { context_or  |=   CTXT_CLOSE ; }
         | NOCLOSE { context_and &= (~CTXT_CLOSE); } ;

OBJ_HELP : HELP   { context_or  |=   CTXT_HELP ; }
         | NOHELP { context_and &= (~CTXT_HELP); } ;

OBJ_PROG : PROGRAM   { context_or  |=   CTXT_PROGRAM ; }
         | NOPROGRAM { context_and &= (~CTXT_PROGRAM); } ;

OBJ_ICVW : ICONVIEW   { context_or  |=   CTXT_ICON ; }
         | NOICONVIEW { context_and &= (~CTXT_ICON); } ;

OBJ_TRVW : TREEVIEW   { context_or  |=   CTXT_TREE ; }
         | NOTREEVIEW { context_and &= (~CTXT_TREE); } ;

OBJ_DTVW : DETAILSVIEW   { context_or  |=   CTXT_DETAILS ; }
         | NODETAILSVIEW { context_and &= (~CTXT_DETAILS); } ;

OBJ_FIND : FIND   { context_or  |=   CTXT_FIND ; }
         | NOFIND { context_and &= (~CTXT_FIND); } ;

OBJ_SEL  : SELECT   { context_or  |=   CTXT_SELECT ; }
         | NOSELECT { context_and &= (~CTXT_SELECT); } ;

OBJ_ARRG : ARRANGE   { context_or  |=   CTXT_ARRANGE ; }
         | NOARRANGE { context_and &= (~CTXT_ARRANGE); } ;

OBJ_SORT : SORT   { context_or  |=   CTXT_SORT ; }
         | NOSORT { context_and &= (~CTXT_SORT); } ;

OBJ_SHUT : SHUTDOWN   { context_or  |=   CTXT_SHUTDOWN ; }
         | NOSHUTDOWN { context_and &= (~CTXT_SHUTDOWN); } ;

OBJ_LOCK : LOCKUP   { context_or  |=   CTXT_LOCKUP ; }
         | NOLOCKUP { context_and &= (~CTXT_LOCKUP); } ;

OBJ_PAL  : PALETTE   { context_or  |=   CTXT_PALETTE ; }
         | NOPALETTE { context_and &= (~CTXT_PALETTE); } ;
    
OBJ_DBLK : DBLKLICK   { misc_and &= (~NO_DEFAULT_OPEN); }
         | NODBLKLICK { misc_or  |=   NO_DEFAULT_OPEN ; } ;

%%
#include <OS2.H>
#include <SOM.H>
#include <WPOBJECT.H>
#include <string.h>
#include <stdio.h>
#include <wp_parse.h>

#define NO_DEFAULT_OPEN    0x0002               // Auáerbetriebnahme des Doppelklicks

ULONG style_or, style_and, context_or, context_and, misc_or, misc_and;
PCHAR pchClass, pchTitle, pchType, pchDefTitle, pchPhysName, pchMyString = NULL;
extern int   yylineno;
extern char *yytext;
extern FILE *parsed;
AccessObject *pao2Write;

void yyerror()
{
    printf("Syntax error in line #%d\n", yylineno);
    return;
}

char *MyString(char *pchIn)
{
    if (pchMyString != NULL)
    {
        free(pchMyString);
        pchMyString = NULL;
    }
    pchMyString = strdup(pchIn + 1);
    pchMyString[strlen(pchMyString)-1] = '\0';
    return pchMyString;
}

void InitClass()
{
    free (pchClass);
    free (pchTitle);
    free (pchType);
    free (pchDefTitle); 
    free (pchPhysName); 
    pchClass    = NULL;
    pchTitle    = NULL;
    pchType     = NULL;
    pchDefTitle = NULL;
    pchPhysName = NULL;

    style_or  = context_or  = misc_or  = 0;
    style_and = context_and = misc_and = 0xFFFFFFFF;

    pao2Write = (AccessObject *) malloc (2000);
}

#if 0
void PrintClass()
{
    if (pchClass)
        printf("1: %s, ", pchClass);
    if (pchTitle)
        printf("2: %s, ", pchTitle);
    if (pchType)
        printf("3: %s, ", pchType);
    if (pchDefTitle)
        printf("4: %s, ", pchDefTitle);
    if (pchPhysName)
        printf("5: %s\n", pchPhysName);

    printf("-> S|: %08x, S&: %08x, C|: %08x, C&: %08x, M|: %08x, M&: %08x \n",
            style_or, style_and, context_or, context_and, misc_or, misc_and);
}
#endif

void WriteClass()
{
    PCHAR pchString = &(pao2Write->Strings);
    USHORT usLength = sizeof(AccessObject);

    pao2Write->usType = 1;

    if (pchClass)
    {
        strcpy(pchString, pchClass);
        pao2Write->usClassLen = strlen(pchClass);
        pao2Write->usClassOffset = pchString - (PCHAR)pao2Write;
        usLength  += pao2Write->usClassLen + 1;
        pchString += pao2Write->usClassLen + 1;
    }
    else
    {
        pao2Write->usClassLen = 0;
        pao2Write->usClassOffset = 0;
    }

    if (pchTitle)
    {
        strcpy(pchString, pchTitle);
        pao2Write->usTitleLen = strlen(pchTitle);
        pao2Write->usTitleOffset = pchString - (PCHAR)pao2Write;
        usLength  += pao2Write->usTitleLen + 1;
        pchString += pao2Write->usTitleLen + 1;
    }
    else
    {
        pao2Write->usTitleLen = 0;
        pao2Write->usTitleOffset = 0;
    }

    if (pchType)
    {
        strcpy(pchString, pchType);
        pao2Write->usTypeLen = strlen(pchType);
        pao2Write->usTypeOffset = pchString - (PCHAR)pao2Write;
        usLength  += pao2Write->usTypeLen + 1;
        pchString += pao2Write->usTypeLen + 1;
    }
    else
    {
        pao2Write->usTypeLen = 0;
        pao2Write->usTypeOffset = 0;
    }

    if (pchDefTitle)
    {
        strcpy(pchString, pchDefTitle);
        pao2Write->usDefTitleLen = strlen(pchDefTitle);
        pao2Write->usDefTitleOffset = pchString - (PCHAR)pao2Write;
        usLength  += pao2Write->usDefTitleLen + 1;
        pchString += pao2Write->usDefTitleLen + 1;
    }
    else
    {
        pao2Write->usDefTitleLen = 0;
        pao2Write->usDefTitleOffset = 0;
    }

    if (pchPhysName)
    {
        strcpy(pchString, pchPhysName);
        pao2Write->usPhysNameLen = strlen(pchPhysName);
        pao2Write->usPhysNameOffset = pchString - (PCHAR)pao2Write;
        usLength  += pao2Write->usPhysNameLen + 1;
        pchString += pao2Write->usPhysNameLen + 1;
    }
    else
    {
        pao2Write->usPhysNameLen = 0;
        pao2Write->usPhysNameOffset = 0;
    }

    pao2Write->ulContext_and = context_and;
    pao2Write->ulContext_or  = context_or;
    pao2Write->ulStyle_and   = style_and;
    pao2Write->ulStyle_or    = style_or;
    pao2Write->ulMisc_and    = misc_and;
    pao2Write->ulMisc_or     = misc_or;
    pao2Write->usLen         = usLength;

    fwrite(pao2Write, (size_t) usLength, 1, parsed);
}
