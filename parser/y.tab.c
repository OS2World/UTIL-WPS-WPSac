#ifndef lint
static char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif
#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define yyclearin (yychar=(-1))
#define yyerrok (yyerrflag=0)
#define YYRECOVERING (yyerrflag!=0)
#define YYPREFIX "yy"
#define OBJ_BEGIN 257
#define ENDOBJECT 258
#define OBJ_CLASS 259
#define OBJ_TITLE 260
#define OBJ_TYPE 261
#define OBJ_DEFTITLE 262
#define OBJ_PHYSNAME 263
#define STRING 264
#define MOVE 265
#define NOMOVE 266
#define LINK 267
#define NOLINK 268
#define COPY 269
#define NOCOPY 270
#define DEFAULTICON 271
#define NODEFAULTICON 272
#define TEMPLATE 273
#define NOTEMPLATE 274
#define DELETE 275
#define NODELETE 276
#define PRINT 277
#define NOPRINT 278
#define DRAG 279
#define NODRAG 280
#define VISIBLE 281
#define NOTVISIBLE 282
#define SETTINGS 283
#define NOSETTINGS 284
#define RENAME 285
#define NORENAME 286
#define CREATEANOTHER 287
#define NOCREATEANOTHER 288
#define NEW 289
#define NONEW 290
#define OPEN 291
#define NOOPEN 292
#define WINDOW 293
#define NOWINDOW 294
#define CLOSE 295
#define NOCLOSE 296
#define HELP 297
#define NOHELP 298
#define PROGRAM 299
#define NOPROGRAM 300
#define ICONVIEW 301
#define NOICONVIEW 302
#define TREEVIEW 303
#define NOTREEVIEW 304
#define DETAILSVIEW 305
#define NODETAILSVIEW 306
#define FIND 307
#define NOFIND 308
#define SELECT 309
#define NOSELECT 310
#define ARRANGE 311
#define NOARRANGE 312
#define SORT 313
#define NOSORT 314
#define SHUTDOWN 315
#define NOSHUTDOWN 316
#define LOCKUP 317
#define NOLOCKUP 318
#define PALETTE 319
#define NOPALETTE 320
#define DBLKLICK 321
#define NODBLKLICK 322
#define ERROR 323
#define YYERRCODE 256
short yylhs[] = {                                        -1,
    0,    0,    2,    1,    3,    3,    5,    5,    5,    5,
    5,    6,    7,    8,    9,   10,   11,    4,    4,   12,
   12,   12,   12,   12,   12,   12,   12,   12,   12,   12,
   12,   12,   12,   12,   12,   12,   12,   12,   12,   12,
   12,   12,   12,   12,   12,   12,   12,   12,   13,   13,
   14,   14,   15,   15,   16,   16,   17,   17,   18,   18,
   19,   19,   20,   20,   21,   21,   22,   22,   23,   23,
   24,   24,   25,   25,   26,   26,   27,   27,   28,   28,
   29,   29,   30,   30,   31,   31,   32,   32,   33,   33,
   34,   34,   35,   35,   36,   36,   37,   37,   38,   38,
   39,   39,   40,   40,   41,   41,
};
short yylen[] = {                                         2,
    2,    1,    0,    7,    3,    1,    1,    1,    1,    1,
    1,    3,    3,    3,    3,    3,    1,    2,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    1,    1,
};
short yydefred[] = {                                      0,
    3,    0,    0,    0,    1,    0,    0,    0,    0,    0,
    0,    0,    0,    7,    8,    9,   10,   11,    0,    0,
    0,    0,    0,    0,    0,   17,   12,   13,   14,   15,
   16,   49,   50,   51,   52,   53,   54,   55,   56,   57,
   58,   59,   60,   61,   62,   63,   64,   65,   66,   67,
   68,   69,   70,   71,   72,   73,   74,   75,   76,   77,
   78,   79,   80,   81,   82,   83,   84,   85,   86,   87,
   88,   89,   90,   91,   92,   93,   94,   95,   96,   97,
   98,   99,  100,  101,  102,  103,  104,  105,  106,    0,
    0,   20,   21,   22,   23,   24,   25,   26,   27,   28,
   29,   30,   31,   32,   33,   34,   35,   36,   37,   38,
   39,   40,   41,   42,   43,   44,   45,   46,   47,   48,
    5,    4,   18,
};
short yydgoto[] = {                                       2,
    3,    4,   12,   90,   13,   14,   15,   16,   17,   18,
   27,   91,   92,   93,   94,   95,   96,   97,   98,   99,
  100,  101,  102,  103,  104,  105,  106,  107,  108,  109,
  110,  111,  112,  113,  114,  115,  116,  117,  118,  119,
  120,
};
short yysindex[] = {                                   -190,
    0,    0, -190,   28,    0, -201,    8,    9,   10,   11,
   12,   33,   31,    0,    0,    0,    0,    0, -188, -188,
 -188, -188, -188, -265, -201,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -181,
 -265,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
short yyrindex[] = {                                      0,
    0,    0,   78,    0,    0,    0,    0,    0,    0,    0,
    0,    0,   38,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
 -178,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,
};
short yygindex[] = {                                     79,
    0,    0,   56,   -8,    0,    0,    0,    0,    0,    0,
   43,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
#define YYTABLESIZE 83
short yytable[] = {                                      32,
   33,   34,   35,   36,   37,   38,   39,   40,   41,   42,
   43,   44,   45,   46,   47,   48,   49,   50,   51,   52,
   53,   54,   55,   56,   57,   58,   59,   60,   61,   62,
   63,   64,   65,   66,   67,   68,   69,   70,   71,   72,
   73,   74,   75,   76,   77,   78,   79,   80,   81,   82,
   83,   84,   85,   86,   87,   88,   89,    7,    8,    9,
   10,   11,   28,   29,   30,   31,    1,    6,   19,   20,
   21,   22,   23,   24,   25,   26,  122,    2,    6,   19,
  121,    5,  123,
};
short yycheck[] = {                                     265,
  266,  267,  268,  269,  270,  271,  272,  273,  274,  275,
  276,  277,  278,  279,  280,  281,  282,  283,  284,  285,
  286,  287,  288,  289,  290,  291,  292,  293,  294,  295,
  296,  297,  298,  299,  300,  301,  302,  303,  304,  305,
  306,  307,  308,  309,  310,  311,  312,  313,  314,  315,
  316,  317,  318,  319,  320,  321,  322,  259,  260,  261,
  262,  263,   20,   21,   22,   23,  257,   40,   61,   61,
   61,   61,   61,   41,   44,  264,  258,    0,   41,  258,
   25,    3,   91,
};
#define YYFINAL 2
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 323
#if YYDEBUG
char *yyname[] = {
"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,"'('","')'",0,0,"','",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"'='",0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"OBJ_BEGIN",
"ENDOBJECT","OBJ_CLASS","OBJ_TITLE","OBJ_TYPE","OBJ_DEFTITLE","OBJ_PHYSNAME",
"STRING","MOVE","NOMOVE","LINK","NOLINK","COPY","NOCOPY","DEFAULTICON",
"NODEFAULTICON","TEMPLATE","NOTEMPLATE","DELETE","NODELETE","PRINT","NOPRINT",
"DRAG","NODRAG","VISIBLE","NOTVISIBLE","SETTINGS","NOSETTINGS","RENAME",
"NORENAME","CREATEANOTHER","NOCREATEANOTHER","NEW","NONEW","OPEN","NOOPEN",
"WINDOW","NOWINDOW","CLOSE","NOCLOSE","HELP","NOHELP","PROGRAM","NOPROGRAM",
"ICONVIEW","NOICONVIEW","TREEVIEW","NOTREEVIEW","DETAILSVIEW","NODETAILSVIEW",
"FIND","NOFIND","SELECT","NOSELECT","ARRANGE","NOARRANGE","SORT","NOSORT",
"SHUTDOWN","NOSHUTDOWN","LOCKUP","NOLOCKUP","PALETTE","NOPALETTE","DBLKLICK",
"NODBLKLICK","ERROR",
};
char *yyrule[] = {
"$accept : OBJS",
"OBJS : ONE_OBJ OBJS",
"OBJS : ONE_OBJ",
"$$1 :",
"ONE_OBJ : OBJ_BEGIN $$1 '(' OBJ_HDRS ')' ANY_FLAGS ENDOBJECT",
"OBJ_HDRS : OBJ_HDR ',' OBJ_HDRS",
"OBJ_HDRS : OBJ_HDR",
"OBJ_HDR : HDR_CLASS",
"OBJ_HDR : HDR_TITLE",
"OBJ_HDR : HDR_TYPE",
"OBJ_HDR : HDR_DFTT",
"OBJ_HDR : HDR_PHNM",
"HDR_CLASS : OBJ_CLASS '=' MYSTRING",
"HDR_TITLE : OBJ_TITLE '=' MYSTRING",
"HDR_TYPE : OBJ_TYPE '=' MYSTRING",
"HDR_DFTT : OBJ_DEFTITLE '=' MYSTRING",
"HDR_PHNM : OBJ_PHYSNAME '=' MYSTRING",
"MYSTRING : STRING",
"ANY_FLAGS : ANY_FLAG ANY_FLAGS",
"ANY_FLAGS : ANY_FLAG",
"ANY_FLAG : OBJ_MOVE",
"ANY_FLAG : OBJ_LINK",
"ANY_FLAG : OBJ_COPY",
"ANY_FLAG : OBJ_ICON",
"ANY_FLAG : OBJ_TEMP",
"ANY_FLAG : OBJ_DEL",
"ANY_FLAG : OBJ_PRNT",
"ANY_FLAG : OBJ_DRAG",
"ANY_FLAG : OBJ_VIS",
"ANY_FLAG : OBJ_SET",
"ANY_FLAG : OBJ_REN",
"ANY_FLAG : OBJ_CREA",
"ANY_FLAG : OBJ_NEW",
"ANY_FLAG : OBJ_OPEN",
"ANY_FLAG : OBJ_WIN",
"ANY_FLAG : OBJ_CLOS",
"ANY_FLAG : OBJ_HELP",
"ANY_FLAG : OBJ_PROG",
"ANY_FLAG : OBJ_ICVW",
"ANY_FLAG : OBJ_TRVW",
"ANY_FLAG : OBJ_DTVW",
"ANY_FLAG : OBJ_FIND",
"ANY_FLAG : OBJ_SEL",
"ANY_FLAG : OBJ_ARRG",
"ANY_FLAG : OBJ_SORT",
"ANY_FLAG : OBJ_SHUT",
"ANY_FLAG : OBJ_LOCK",
"ANY_FLAG : OBJ_PAL",
"ANY_FLAG : OBJ_DBLK",
"OBJ_MOVE : MOVE",
"OBJ_MOVE : NOMOVE",
"OBJ_LINK : LINK",
"OBJ_LINK : NOLINK",
"OBJ_COPY : COPY",
"OBJ_COPY : NOCOPY",
"OBJ_ICON : DEFAULTICON",
"OBJ_ICON : NODEFAULTICON",
"OBJ_TEMP : TEMPLATE",
"OBJ_TEMP : NOTEMPLATE",
"OBJ_DEL : DELETE",
"OBJ_DEL : NODELETE",
"OBJ_PRNT : PRINT",
"OBJ_PRNT : NOPRINT",
"OBJ_DRAG : DRAG",
"OBJ_DRAG : NODRAG",
"OBJ_VIS : VISIBLE",
"OBJ_VIS : NOTVISIBLE",
"OBJ_SET : SETTINGS",
"OBJ_SET : NOSETTINGS",
"OBJ_REN : RENAME",
"OBJ_REN : NORENAME",
"OBJ_CREA : CREATEANOTHER",
"OBJ_CREA : NOCREATEANOTHER",
"OBJ_NEW : NEW",
"OBJ_NEW : NONEW",
"OBJ_OPEN : OPEN",
"OBJ_OPEN : NOOPEN",
"OBJ_WIN : WINDOW",
"OBJ_WIN : NOWINDOW",
"OBJ_CLOS : CLOSE",
"OBJ_CLOS : NOCLOSE",
"OBJ_HELP : HELP",
"OBJ_HELP : NOHELP",
"OBJ_PROG : PROGRAM",
"OBJ_PROG : NOPROGRAM",
"OBJ_ICVW : ICONVIEW",
"OBJ_ICVW : NOICONVIEW",
"OBJ_TRVW : TREEVIEW",
"OBJ_TRVW : NOTREEVIEW",
"OBJ_DTVW : DETAILSVIEW",
"OBJ_DTVW : NODETAILSVIEW",
"OBJ_FIND : FIND",
"OBJ_FIND : NOFIND",
"OBJ_SEL : SELECT",
"OBJ_SEL : NOSELECT",
"OBJ_ARRG : ARRANGE",
"OBJ_ARRG : NOARRANGE",
"OBJ_SORT : SORT",
"OBJ_SORT : NOSORT",
"OBJ_SHUT : SHUTDOWN",
"OBJ_SHUT : NOSHUTDOWN",
"OBJ_LOCK : LOCKUP",
"OBJ_LOCK : NOLOCKUP",
"OBJ_PAL : PALETTE",
"OBJ_PAL : NOPALETTE",
"OBJ_DBLK : DBLKLICK",
"OBJ_DBLK : NODBLKLICK",
};
#endif
#ifndef YYSTYPE
typedef int YYSTYPE;
#endif
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH 500
#endif
#endif
int yydebug;
int yynerrs;
int yyerrflag;
int yychar;
short *yyssp;
YYSTYPE *yyvsp;
YYSTYPE yyval;
YYSTYPE yylval;
short yyss[YYSTACKSIZE];
YYSTYPE yyvs[YYSTACKSIZE];
#define yystacksize YYSTACKSIZE
#line 172 "parser.y"
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
#line 508 "y.tab.c"
#define YYABORT goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR goto yyerrlab
int
yyparse()
{
    register int yym, yyn, yystate;
#if YYDEBUG
    register char *yys;
    extern char *getenv();

    if (yys = getenv("YYDEBUG"))
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = (-1);

    yyssp = yyss;
    yyvsp = yyvs;
    *yyssp = yystate = 0;

yyloop:
    if (yyn = yydefred[yystate]) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yyssp >= yyss + yystacksize - 1)
        {
            goto yyoverflow;
        }
        *++yyssp = yystate = yytable[yyn];
        *++yyvsp = yylval;
        yychar = (-1);
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;
#ifdef lint
    goto yynewerror;
#endif
yynewerror:
    yyerror("syntax error");
#ifdef lint
    goto yyerrlab;
#endif
yyerrlab:
    ++yynerrs;
yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yyssp]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yyssp, yytable[yyn]);
#endif
                if (yyssp >= yyss + yystacksize - 1)
                {
                    goto yyoverflow;
                }
                *++yyssp = yystate = yytable[yyn];
                *++yyvsp = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yyssp);
#endif
                if (yyssp <= yyss) goto yyabort;
                --yyssp;
                --yyvsp;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = (-1);
        goto yyloop;
    }
yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    yyval = yyvsp[1-yym];
    switch (yyn)
    {
case 3:
#line 40 "parser.y"
{InitClass(); }
break;
case 4:
#line 41 "parser.y"
{WriteClass();}
break;
case 12:
#line 48 "parser.y"
{ if (pchClass == NULL)
                                          pchClass = strdup((PCHAR)yyvsp[0]);
                                      else
                                          printf("CLASS double defined, second definition ignored\n"); }
break;
case 13:
#line 53 "parser.y"
{ if (pchTitle == NULL)
                                          pchTitle = strdup((PCHAR)yyvsp[0]);
                                      else
                                          printf("TITLE double defined, second definition ignored\n"); }
break;
case 14:
#line 58 "parser.y"
{ if (pchType == NULL)
                                          pchType = strdup((PCHAR)yyvsp[0]);
                                      else
                                          printf("TYPE double defined, second definition ignored\n"); }
break;
case 15:
#line 63 "parser.y"
{ if (pchDefTitle == NULL)
                                             pchDefTitle = strdup((PCHAR)yyvsp[0]);
                                         else
                                             printf("DEFTITLE double defined, second definition ignored\n"); }
break;
case 16:
#line 68 "parser.y"
{ if (pchPhysName == NULL)
                                             pchPhysName = strdup((PCHAR)yyvsp[0]);
                                         else
                                             printf("PHYSNAME double defined, second definition ignored\n"); }
break;
case 17:
#line 73 "parser.y"
{ yyval = (LONG) pchMyString; }
break;
case 49:
#line 84 "parser.y"
{ style_and &= (~OBJSTYLE_NOMOVE); context_or  |=   CTXT_MOVE ; }
break;
case 50:
#line 85 "parser.y"
{ style_or  |=   OBJSTYLE_NOMOVE ; context_and &= (~CTXT_MOVE); }
break;
case 51:
#line 87 "parser.y"
{ style_and &= (~OBJSTYLE_NOLINK); context_or  |=   CTXT_LINK ; }
break;
case 52:
#line 88 "parser.y"
{ style_or  |=   OBJSTYLE_NOLINK ; context_and &= (~CTXT_MOVE); }
break;
case 53:
#line 90 "parser.y"
{ style_and &= (~OBJSTYLE_NOCOPY); context_or  |=   CTXT_COPY ; }
break;
case 54:
#line 91 "parser.y"
{ style_or  |=   OBJSTYLE_NOCOPY ; context_and &= (~CTXT_COPY); }
break;
case 55:
#line 93 "parser.y"
{ style_and &= (~OBJSTYLE_NOTDEFAULTICON); }
break;
case 56:
#line 94 "parser.y"
{ style_or  |=   OBJSTYLE_NOTDEFAULTICON ; }
break;
case 57:
#line 96 "parser.y"
{ style_and &= (~OBJSTYLE_TEMPLATE); }
break;
case 58:
#line 97 "parser.y"
{ style_or  |=   OBJSTYLE_TEMPLATE ; }
break;
case 59:
#line 99 "parser.y"
{ style_and &= (~OBJSTYLE_NODELETE); context_and |=   CTXT_DELETE ; }
break;
case 60:
#line 100 "parser.y"
{ style_or  |=   OBJSTYLE_NODELETE ; context_and &= (~CTXT_DELETE); }
break;
case 61:
#line 102 "parser.y"
{ style_and &= (~OBJSTYLE_NOPRINT); context_and |=   CTXT_PRINT ; }
break;
case 62:
#line 103 "parser.y"
{ style_or  |=   OBJSTYLE_NOPRINT ; context_and &= (~CTXT_PRINT); }
break;
case 63:
#line 105 "parser.y"
{ style_and &= (~OBJSTYLE_NODRAG); }
break;
case 64:
#line 106 "parser.y"
{ style_or  |=   OBJSTYLE_NODRAG ; }
break;
case 65:
#line 108 "parser.y"
{ style_and &= (~OBJSTYLE_NOTVISIBLE); }
break;
case 66:
#line 109 "parser.y"
{ style_or  |=  OBJSTYLE_NOTVISIBLE  ; }
break;
case 67:
#line 111 "parser.y"
{ style_and &= (~OBJSTYLE_NOSETTINGS); context_and |=   CTXT_SETTINGS ; }
break;
case 68:
#line 112 "parser.y"
{ style_or  |=   OBJSTYLE_NOSETTINGS ; context_and &= (~CTXT_SETTINGS); }
break;
case 69:
#line 114 "parser.y"
{ style_and &= (~OBJSTYLE_NORENAME); }
break;
case 70:
#line 115 "parser.y"
{ style_or  |=   OBJSTYLE_NORENAME ; }
break;
case 71:
#line 117 "parser.y"
{ context_or  |=   CTXT_CRANOTHER ; }
break;
case 72:
#line 118 "parser.y"
{ context_and &= (~CTXT_CRANOTHER); }
break;
case 73:
#line 120 "parser.y"
{ context_or  |=   CTXT_NEW ; }
break;
case 74:
#line 121 "parser.y"
{ context_and &= (~CTXT_NEW); }
break;
case 75:
#line 123 "parser.y"
{ context_or  |=   CTXT_OPEN ; }
break;
case 76:
#line 124 "parser.y"
{ context_and &= (~CTXT_OPEN); }
break;
case 77:
#line 126 "parser.y"
{ context_or  |=   CTXT_WINDOW ; }
break;
case 78:
#line 127 "parser.y"
{ context_and &= (~CTXT_WINDOW); }
break;
case 79:
#line 129 "parser.y"
{ context_or  |=   CTXT_CLOSE ; }
break;
case 80:
#line 130 "parser.y"
{ context_and &= (~CTXT_CLOSE); }
break;
case 81:
#line 132 "parser.y"
{ context_or  |=   CTXT_HELP ; }
break;
case 82:
#line 133 "parser.y"
{ context_and &= (~CTXT_HELP); }
break;
case 83:
#line 135 "parser.y"
{ context_or  |=   CTXT_PROGRAM ; }
break;
case 84:
#line 136 "parser.y"
{ context_and &= (~CTXT_PROGRAM); }
break;
case 85:
#line 138 "parser.y"
{ context_or  |=   CTXT_ICON ; }
break;
case 86:
#line 139 "parser.y"
{ context_and &= (~CTXT_ICON); }
break;
case 87:
#line 141 "parser.y"
{ context_or  |=   CTXT_TREE ; }
break;
case 88:
#line 142 "parser.y"
{ context_and &= (~CTXT_TREE); }
break;
case 89:
#line 144 "parser.y"
{ context_or  |=   CTXT_DETAILS ; }
break;
case 90:
#line 145 "parser.y"
{ context_and &= (~CTXT_DETAILS); }
break;
case 91:
#line 147 "parser.y"
{ context_or  |=   CTXT_FIND ; }
break;
case 92:
#line 148 "parser.y"
{ context_and &= (~CTXT_FIND); }
break;
case 93:
#line 150 "parser.y"
{ context_or  |=   CTXT_SELECT ; }
break;
case 94:
#line 151 "parser.y"
{ context_and &= (~CTXT_SELECT); }
break;
case 95:
#line 153 "parser.y"
{ context_or  |=   CTXT_ARRANGE ; }
break;
case 96:
#line 154 "parser.y"
{ context_and &= (~CTXT_ARRANGE); }
break;
case 97:
#line 156 "parser.y"
{ context_or  |=   CTXT_SORT ; }
break;
case 98:
#line 157 "parser.y"
{ context_and &= (~CTXT_SORT); }
break;
case 99:
#line 159 "parser.y"
{ context_or  |=   CTXT_SHUTDOWN ; }
break;
case 100:
#line 160 "parser.y"
{ context_and &= (~CTXT_SHUTDOWN); }
break;
case 101:
#line 162 "parser.y"
{ context_or  |=   CTXT_LOCKUP ; }
break;
case 102:
#line 163 "parser.y"
{ context_and &= (~CTXT_LOCKUP); }
break;
case 103:
#line 165 "parser.y"
{ context_or  |=   CTXT_PALETTE ; }
break;
case 104:
#line 166 "parser.y"
{ context_and &= (~CTXT_PALETTE); }
break;
case 105:
#line 168 "parser.y"
{ misc_and &= (~NO_DEFAULT_OPEN); }
break;
case 106:
#line 169 "parser.y"
{ misc_or  |=   NO_DEFAULT_OPEN ; }
break;
#line 928 "y.tab.c"
    }
    yyssp -= yym;
    yystate = *yyssp;
    yyvsp -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yyssp = YYFINAL;
        *++yyvsp = yyval;
        if (yychar < 0)
        {
            if ((yychar = yylex()) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yyssp, yystate);
#endif
    if (yyssp >= yyss + yystacksize - 1)
    {
        goto yyoverflow;
    }
    *++yyssp = yystate;
    *++yyvsp = yyval;
    goto yyloop;
yyoverflow:
    yyerror("yacc stack overflow");
yyabort:
    return (1);
yyaccept:
    return (0);
}
