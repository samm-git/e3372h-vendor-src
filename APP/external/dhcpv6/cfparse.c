
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.1"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 31 "cfparse.y"

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/queue.h>
#include <sys/time.h>

#include <netinet/in.h>

#include <arpa/inet.h>

#include <stdlib.h>
#include <string.h>

#include "dhcp6.h"
#include "config.h"
#include "common.h"

extern int lineno;
extern int cfdebug;

extern void yywarn __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));
extern void yyerror __P((char *, ...))
	__attribute__((__format__(__printf__, 1, 2)));

#define MAKE_NAMELIST(l, n, p) do { \
	(l) = (struct cf_namelist *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (p) cleanup_cflist(p); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->name = (n); \
	l->params = (p); \
	} while (0)

#define MAKE_CFLIST(l, t, pp, pl) do { \
	(l) = (struct cf_list *)malloc(sizeof(*(l))); \
	if ((l) == NULL) { \
		yywarn("can't allocate memory"); \
		if (pp) free(pp); \
		if (pl) cleanup_cflist(pl); \
		return (-1); \
	} \
	memset((l), 0, sizeof(*(l))); \
	l->line = lineno; \
	l->type = (t); \
	l->ptr = (pp); \
	l->list = (pl); \
	l->tail = (l); \
	} while (0)

static struct cf_namelist *iflist_head, *hostlist_head, *iapdlist_head;
static struct cf_namelist *addrpoollist_head;
static struct cf_namelist *authinfolist_head, *keylist_head;
static struct cf_namelist *ianalist_head;
struct cf_list *cf_dns_list, *cf_dns_name_list, *cf_ntp_list;
struct cf_list *cf_sip_list, *cf_sip_name_list;
struct cf_list *cf_dslite_list, *cf_dslite_name_list;
struct cf_list *cf_nis_list, *cf_nis_name_list;
struct cf_list *cf_nisp_list, *cf_nisp_name_list;
struct cf_list *cf_bcmcs_list, *cf_bcmcs_name_list;
long long cf_refreshtime = -1;

extern int yylex __P((void));
extern int cfswitch_buffer __P((char *));
static int add_namelist __P((struct cf_namelist *, struct cf_namelist **));
static void cleanup __P((void));
static void cleanup_namelist __P((struct cf_namelist *));
static void cleanup_cflist __P((struct cf_list *));


/* Line 189 of yacc.c  */
#line 148 "y.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTERFACE = 258,
     IFNAME = 259,
     PREFIX_INTERFACE = 260,
     SLA_ID = 261,
     SLA_LEN = 262,
     DUID_ID = 263,
     ID_ASSOC = 264,
     IA_PD = 265,
     IAID = 266,
     IA_NA = 267,
     ADDRESS = 268,
     REQUEST = 269,
     SEND = 270,
     ALLOW = 271,
     PREFERENCE = 272,
     HOST = 273,
     HOSTNAME = 274,
     DUID = 275,
     OPTION = 276,
     RAPID_COMMIT = 277,
     DNS_SERVERS = 278,
     DNS_NAME = 279,
     NTP_SERVERS = 280,
     REFRESHTIME = 281,
     SIP_SERVERS = 282,
     SIP_NAME = 283,
     DSLITE_SERVERS = 284,
     DSLITE_NAME = 285,
     NIS_SERVERS = 286,
     NIS_NAME = 287,
     NISP_SERVERS = 288,
     NISP_NAME = 289,
     BCMCS_SERVERS = 290,
     BCMCS_NAME = 291,
     INFO_ONLY = 292,
     SCRIPT = 293,
     DELAYEDKEY = 294,
     AUTHENTICATION = 295,
     PROTOCOL = 296,
     ALGORITHM = 297,
     DELAYED = 298,
     RECONFIG = 299,
     HMACMD5 = 300,
     MONOCOUNTER = 301,
     AUTHNAME = 302,
     RDM = 303,
     KEY = 304,
     KEYINFO = 305,
     REALM = 306,
     KEYID = 307,
     SECRET = 308,
     KEYNAME = 309,
     EXPIRE = 310,
     ADDRPOOL = 311,
     POOLNAME = 312,
     RANGE = 313,
     TO = 314,
     ADDRESS_POOL = 315,
     INCLUDE = 316,
     NUMBER = 317,
     SLASH = 318,
     EOS = 319,
     BCL = 320,
     ECL = 321,
     STRING = 322,
     QSTRING = 323,
     PREFIX = 324,
     INFINITY = 325,
     COMMA = 326
   };
#endif
/* Tokens.  */
#define INTERFACE 258
#define IFNAME 259
#define PREFIX_INTERFACE 260
#define SLA_ID 261
#define SLA_LEN 262
#define DUID_ID 263
#define ID_ASSOC 264
#define IA_PD 265
#define IAID 266
#define IA_NA 267
#define ADDRESS 268
#define REQUEST 269
#define SEND 270
#define ALLOW 271
#define PREFERENCE 272
#define HOST 273
#define HOSTNAME 274
#define DUID 275
#define OPTION 276
#define RAPID_COMMIT 277
#define DNS_SERVERS 278
#define DNS_NAME 279
#define NTP_SERVERS 280
#define REFRESHTIME 281
#define SIP_SERVERS 282
#define SIP_NAME 283
#define DSLITE_SERVERS 284
#define DSLITE_NAME 285
#define NIS_SERVERS 286
#define NIS_NAME 287
#define NISP_SERVERS 288
#define NISP_NAME 289
#define BCMCS_SERVERS 290
#define BCMCS_NAME 291
#define INFO_ONLY 292
#define SCRIPT 293
#define DELAYEDKEY 294
#define AUTHENTICATION 295
#define PROTOCOL 296
#define ALGORITHM 297
#define DELAYED 298
#define RECONFIG 299
#define HMACMD5 300
#define MONOCOUNTER 301
#define AUTHNAME 302
#define RDM 303
#define KEY 304
#define KEYINFO 305
#define REALM 306
#define KEYID 307
#define SECRET 308
#define KEYNAME 309
#define EXPIRE 310
#define ADDRPOOL 311
#define POOLNAME 312
#define RANGE 313
#define TO 314
#define ADDRESS_POOL 315
#define INCLUDE 316
#define NUMBER 317
#define SLASH 318
#define EOS 319
#define BCL 320
#define ECL 321
#define STRING 322
#define QSTRING 323
#define PREFIX 324
#define INFINITY 325
#define COMMA 326




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 128 "cfparse.y"

	long long num;
	char* str;
	struct cf_list *list;
	struct dhcp6_prefix *prefix;
	struct dhcp6_range *range;
	struct dhcp6_poolspec *pool;



/* Line 214 of yacc.c  */
#line 337 "y.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 349 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   238

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  72
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  36
/* YYNRULES -- Number of rules.  */
#define YYNRULES  109
/* YYNRULES -- Number of states.  */
#define YYNSTATES  239

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   326

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    13,    15,    17,
      19,    21,    23,    30,    37,    42,    47,    52,    57,    62,
      67,    72,    77,    82,    87,    92,    97,   102,   107,   115,
     122,   130,   137,   144,   151,   155,   162,   163,   166,   168,
     169,   172,   176,   180,   183,   187,   191,   195,   199,   203,
     207,   211,   215,   219,   221,   225,   227,   230,   233,   236,
     238,   240,   242,   244,   246,   248,   250,   252,   254,   256,
     258,   260,   262,   264,   268,   271,   275,   280,   286,   289,
     293,   295,   297,   298,   301,   303,   307,   314,   315,   318,
     322,   326,   327,   330,   334,   335,   338,   342,   346,   350,
     354,   356,   358,   360,   362,   363,   366,   370,   374,   378
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      73,     0,    -1,    -1,    73,    74,    -1,    75,    -1,    76,
      -1,    77,    -1,    78,    -1,    79,    -1,    80,    -1,    82,
      -1,    81,    -1,     3,     4,    65,    85,    66,    64,    -1,
      18,    19,    65,    85,    66,    64,    -1,    21,    23,    83,
      64,    -1,    21,    24,    68,    64,    -1,    21,    25,    83,
      64,    -1,    21,    27,    83,    64,    -1,    21,    28,    68,
      64,    -1,    21,    29,    83,    64,    -1,    21,    30,    68,
      64,    -1,    21,    31,    83,    64,    -1,    21,    32,    68,
      64,    -1,    21,    33,    83,    64,    -1,    21,    34,    68,
      64,    -1,    21,    35,    83,    64,    -1,    21,    36,    68,
      64,    -1,    21,    26,    62,    64,    -1,     9,    10,    11,
      65,    94,    66,    64,    -1,     9,    10,    65,    94,    66,
      64,    -1,     9,    12,    11,    65,    99,    66,    64,    -1,
       9,    12,    65,    99,    66,    64,    -1,    40,    47,    65,
     101,    66,    64,    -1,    50,    54,    65,   106,    66,    64,
      -1,    61,    68,    64,    -1,    56,    57,    65,    85,    66,
      64,    -1,    -1,    83,    84,    -1,    67,    -1,    -1,    85,
      86,    -1,    15,    87,    64,    -1,    14,    87,    64,    -1,
      37,    64,    -1,    16,    88,    64,    -1,    20,     8,    64,
      -1,    13,    90,    64,    -1,    69,    91,    64,    -1,    17,
      62,    64,    -1,    38,    68,    64,    -1,    39,    67,    64,
      -1,    58,    89,    64,    -1,    60,    92,    64,    -1,    88,
      -1,    88,    71,    87,    -1,    22,    -1,    40,    47,    -1,
      10,    62,    -1,    12,    62,    -1,    27,    -1,    28,    -1,
      29,    -1,    30,    -1,    23,    -1,    24,    -1,    25,    -1,
      26,    -1,    31,    -1,    32,    -1,    33,    -1,    34,    -1,
      35,    -1,    36,    -1,    67,    59,    67,    -1,    67,    93,
      -1,    67,    93,    93,    -1,    67,    63,    62,    93,    -1,
      67,    63,    62,    93,    93,    -1,    67,    93,    -1,    67,
      93,    93,    -1,    70,    -1,    62,    -1,    -1,    94,    95,
      -1,    96,    -1,    69,    91,    64,    -1,     5,     4,    65,
      97,    66,    64,    -1,    -1,    97,    98,    -1,     6,    62,
      64,    -1,     7,    62,    64,    -1,    -1,    99,   100,    -1,
      13,    90,    64,    -1,    -1,   101,   102,    -1,    41,   103,
      64,    -1,    42,   104,    64,    -1,    48,   105,    64,    -1,
      49,    67,    64,    -1,    43,    -1,    44,    -1,    45,    -1,
      46,    -1,    -1,   106,   107,    -1,    51,    68,    64,    -1,
      52,    62,    64,    -1,    53,    68,    64,    -1,    55,    68,
      64,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   151,   151,   153,   157,   158,   159,   160,   161,   162,
     163,   164,   168,   180,   192,   201,   216,   225,   234,   249,
     258,   273,   282,   297,   306,   321,   330,   345,   374,   383,
     397,   406,   423,   435,   447,   458,   470,   471,   489,   512,
     513,   531,   539,   547,   555,   563,   571,   579,   587,   596,
     604,   612,   620,   631,   635,   645,   653,   661,   669,   677,
     685,   693,   701,   709,   717,   725,   733,   741,   749,   757,
     765,   773,   781,   792,   823,   850,   883,   910,   943,   968,
     999,  1003,  1010,  1011,  1029,  1030,  1041,  1051,  1052,  1070,
    1078,  1089,  1090,  1108,  1119,  1120,  1138,  1146,  1154,  1162,
    1173,  1174,  1178,  1182,  1186,  1187,  1205,  1213,  1221,  1229
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INTERFACE", "IFNAME",
  "PREFIX_INTERFACE", "SLA_ID", "SLA_LEN", "DUID_ID", "ID_ASSOC", "IA_PD",
  "IAID", "IA_NA", "ADDRESS", "REQUEST", "SEND", "ALLOW", "PREFERENCE",
  "HOST", "HOSTNAME", "DUID", "OPTION", "RAPID_COMMIT", "DNS_SERVERS",
  "DNS_NAME", "NTP_SERVERS", "REFRESHTIME", "SIP_SERVERS", "SIP_NAME",
  "DSLITE_SERVERS", "DSLITE_NAME", "NIS_SERVERS", "NIS_NAME",
  "NISP_SERVERS", "NISP_NAME", "BCMCS_SERVERS", "BCMCS_NAME", "INFO_ONLY",
  "SCRIPT", "DELAYEDKEY", "AUTHENTICATION", "PROTOCOL", "ALGORITHM",
  "DELAYED", "RECONFIG", "HMACMD5", "MONOCOUNTER", "AUTHNAME", "RDM",
  "KEY", "KEYINFO", "REALM", "KEYID", "SECRET", "KEYNAME", "EXPIRE",
  "ADDRPOOL", "POOLNAME", "RANGE", "TO", "ADDRESS_POOL", "INCLUDE",
  "NUMBER", "SLASH", "EOS", "BCL", "ECL", "STRING", "QSTRING", "PREFIX",
  "INFINITY", "COMMA", "$accept", "statements", "statement",
  "interface_statement", "host_statement", "option_statement",
  "ia_statement", "authentication_statement", "key_statement",
  "include_statement", "addrpool_statement", "address_list",
  "address_list_ent", "declarations", "declaration", "dhcpoption_list",
  "dhcpoption", "rangeparam", "addressparam", "prefixparam", "poolparam",
  "duration", "iapdconf_list", "iapdconf", "prefix_interface", "ifparams",
  "ifparam", "ianaconf_list", "ianaconf", "authparam_list", "authparam",
  "authproto", "authalg", "authrdm", "keyparam_list", "keyparam", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    72,    73,    73,    74,    74,    74,    74,    74,    74,
      74,    74,    75,    76,    77,    77,    77,    77,    77,    77,
      77,    77,    77,    77,    77,    77,    77,    77,    78,    78,
      78,    78,    79,    80,    81,    82,    83,    83,    84,    85,
      85,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    87,    87,    88,    88,    88,    88,    88,
      88,    88,    88,    88,    88,    88,    88,    88,    88,    88,
      88,    88,    88,    89,    90,    90,    91,    91,    92,    92,
      93,    93,    94,    94,    95,    95,    96,    97,    97,    98,
      98,    99,    99,   100,   101,   101,   102,   102,   102,   102,
     103,   103,   104,   105,   106,   106,   107,   107,   107,   107
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     6,     6,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     7,     6,
       7,     6,     6,     6,     3,     6,     0,     2,     1,     0,
       2,     3,     3,     2,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     1,     3,     1,     2,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     3,     2,     3,     4,     5,     2,     3,
       1,     1,     0,     2,     1,     3,     6,     0,     2,     3,
       3,     0,     2,     3,     0,     2,     3,     3,     3,     3,
       1,     1,     1,     1,     0,     2,     3,     3,     3,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     3,     4,     5,     6,     7,     8,     9,    11,    10,
       0,     0,     0,     0,    36,     0,    36,     0,    36,     0,
      36,     0,    36,     0,    36,     0,    36,     0,     0,     0,
       0,     0,    39,     0,    82,     0,    91,    39,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,   104,    39,    34,     0,    82,     0,    91,
       0,     0,    14,    38,    37,    15,    16,    27,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    40,     0,     0,     0,     0,    83,
      84,     0,     0,     0,    92,     0,     0,     0,     0,     0,
       0,    95,     0,     0,     0,     0,     0,   105,     0,     0,
       0,     0,     0,    55,    63,    64,    65,    66,    59,    60,
      61,    62,    67,    68,    69,    70,    71,    72,     0,     0,
      53,     0,     0,     0,     0,    43,     0,     0,     0,     0,
       0,     0,    12,     0,     0,     0,     0,    29,     0,     0,
       0,    31,    13,   100,   101,     0,   102,     0,   103,     0,
       0,    32,     0,     0,     0,     0,    33,    35,    81,    80,
      74,    46,    57,    58,    56,    42,     0,    41,    44,    48,
      45,    49,    50,     0,    51,    78,    52,     0,    47,    28,
      87,    85,    30,    93,    96,    97,    98,    99,   106,   107,
     108,   109,    75,    54,    73,    79,     0,     0,    76,     0,
       0,     0,    88,    77,     0,     0,    86,    89,    90
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    11,    12,    13,    14,    15,    16,    17,    18,
      19,    48,    74,    66,   104,   149,   150,   159,   130,   164,
     161,   190,    68,   109,   110,   227,   232,    70,   114,    88,
     121,   175,   177,   179,    89,   127
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -160
static const yytype_int16 yypact[] =
{
    -160,     3,  -160,     9,    37,    -5,   140,    -9,     1,     5,
     -12,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
      25,    -4,     7,    30,  -160,    29,  -160,    -2,  -160,    33,
    -160,    34,  -160,    36,  -160,    38,  -160,    43,    40,    48,
      50,    53,  -160,    58,  -160,    59,  -160,  -160,   -32,    62,
     -30,    64,   -25,    69,   -23,    72,   -19,    74,   -10,    75,
      16,    77,  -160,  -160,  -160,  -160,    13,  -160,    -3,  -160,
      -8,    61,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,   -26,    41,
      71,    68,   122,   122,   122,    63,   134,    79,    91,    93,
      94,   110,   114,   112,  -160,    -1,   176,   117,   112,  -160,
    -160,    23,    68,   118,  -160,   119,   -24,   139,   141,   121,
     125,  -160,   123,   124,   126,   127,   128,  -160,   129,   -53,
     132,   135,   136,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,   138,   137,
     131,   142,   143,   144,   145,  -160,   146,   147,   153,   149,
     -53,   150,  -160,   152,   154,   155,   151,  -160,   156,   157,
     158,  -160,  -160,  -160,  -160,   159,  -160,   160,  -160,   161,
     162,  -160,   163,   164,   165,   166,  -160,  -160,  -160,  -160,
     -53,  -160,  -160,  -160,  -160,  -160,   122,  -160,  -160,  -160,
    -160,  -160,  -160,   133,  -160,   -53,  -160,   169,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160,   -53,     4,   -53,   170,
     171,   172,  -160,  -160,   173,   174,  -160,  -160,  -160
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,  -160,
    -160,    86,  -160,   -39,  -160,   -93,    96,  -160,    87,    95,
    -160,  -159,   167,  -160,  -160,  -160,  -160,   148,  -160,  -160,
    -160,  -160,  -160,  -160,  -160,  -160
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
     151,   205,   106,     2,   106,   112,     3,    43,    71,   188,
     229,   230,     4,    20,    23,   116,   117,   189,    45,   173,
     174,     5,   118,   119,     6,    90,    91,    92,    93,    94,
      95,   222,    72,    96,    76,    73,   112,    73,    38,    78,
     120,    80,    73,     7,    73,    82,   225,    21,    73,    22,
      97,    98,    99,     8,    84,    39,    41,    73,   113,     9,
      51,    44,    40,   107,    10,   165,   108,   228,   108,   233,
     231,   100,    46,   101,    91,    92,    93,    94,    95,   102,
      86,    96,   103,    73,    91,    92,    93,    94,    95,   169,
      42,    96,   122,   123,   124,    47,   125,    49,    97,    98,
      99,    53,    55,   223,    57,    62,    59,   126,    97,    98,
      99,    61,    50,    63,    52,    64,    54,    65,    56,   100,
      58,   101,    60,    67,    69,   153,    75,   115,    77,   100,
     103,   101,   131,    79,   132,   129,    81,   128,    83,    85,
     103,    87,   154,   155,   133,   134,   135,   136,   137,   138,
     139,   140,   141,   142,   143,   144,   145,   146,   147,   156,
     157,   158,   148,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    36,    37,   160,   162,   163,
     166,   167,   171,   172,   176,   194,   183,   178,   180,   181,
     152,   182,   186,   187,   184,   185,   191,   192,   193,   170,
     224,   195,   196,   168,     0,     0,   197,   198,   199,   200,
     201,   202,   203,   204,   206,   207,   210,   111,   208,   209,
     211,   212,   213,   214,   215,   216,   217,   218,   219,   220,
     221,   226,   234,   235,   105,     0,   236,   237,   238
};

static const yytype_int16 yycheck[] =
{
      93,   160,     5,     0,     5,    13,     3,    11,    47,    62,
       6,     7,     9,     4,    19,    41,    42,    70,    11,    43,
      44,    18,    48,    49,    21,    64,    13,    14,    15,    16,
      17,   190,    64,    20,    64,    67,    13,    67,    47,    64,
      66,    64,    67,    40,    67,    64,   205,    10,    67,    12,
      37,    38,    39,    50,    64,    54,    68,    67,    66,    56,
      62,    65,    57,    66,    61,    66,    69,   226,    69,   228,
      66,    58,    65,    60,    13,    14,    15,    16,    17,    66,
      64,    20,    69,    67,    13,    14,    15,    16,    17,    66,
      65,    20,    51,    52,    53,    65,    55,    68,    37,    38,
      39,    68,    68,   196,    68,    65,    68,    66,    37,    38,
      39,    68,    26,    65,    28,    65,    30,    64,    32,    58,
      34,    60,    36,    65,    65,    62,    64,    66,    64,    58,
      69,    60,    10,    64,    12,    67,    64,    66,    64,    64,
      69,    64,     8,    64,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    68,
      67,    67,    40,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    36,    67,    64,    67,
       4,    64,    64,    64,    45,    47,    62,    46,    67,    64,
      94,    68,    64,    64,    68,    68,    64,    62,    62,   112,
      67,    64,    71,   108,    -1,    -1,    64,    64,    64,    64,
      64,    64,    59,    64,    64,    63,    65,    69,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    62,    62,    62,    67,    -1,    64,    64,    64
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    73,     0,     3,     9,    18,    21,    40,    50,    56,
      61,    74,    75,    76,    77,    78,    79,    80,    81,    82,
       4,    10,    12,    19,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    47,    54,
      57,    68,    65,    11,    65,    11,    65,    65,    83,    68,
      83,    62,    83,    68,    83,    68,    83,    68,    83,    68,
      83,    68,    65,    65,    65,    64,    85,    65,    94,    65,
      99,    85,    64,    67,    84,    64,    64,    64,    64,    64,
      64,    64,    64,    64,    64,    64,    64,    64,   101,   106,
      85,    13,    14,    15,    16,    17,    20,    37,    38,    39,
      58,    60,    66,    69,    86,    94,     5,    66,    69,    95,
      96,    99,    13,    66,   100,    66,    41,    42,    48,    49,
      66,   102,    51,    52,    53,    55,    66,   107,    66,    67,
      90,    10,    12,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    40,    87,
      88,    87,    88,    62,     8,    64,    68,    67,    67,    89,
      67,    92,    64,    67,    91,    66,     4,    64,    91,    66,
      90,    64,    64,    43,    44,   103,    45,   104,    46,   105,
      67,    64,    68,    62,    68,    68,    64,    64,    62,    70,
      93,    64,    62,    62,    47,    64,    71,    64,    64,    64,
      64,    64,    64,    59,    64,    93,    64,    63,    64,    64,
      65,    64,    64,    64,    64,    64,    64,    64,    64,    64,
      64,    64,    93,    87,    67,    93,    62,    97,    93,     6,
       7,    66,    98,    93,    62,    62,    64,    64,    64
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 12:

/* Line 1455 of yacc.c  */
#line 169 "cfparse.y"
    {
		struct cf_namelist *ifl;

		MAKE_NAMELIST(ifl, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(ifl, &iflist_head))
			return (-1);
	}
    break;

  case 13:

/* Line 1455 of yacc.c  */
#line 181 "cfparse.y"
    {
		struct cf_namelist *host;

		MAKE_NAMELIST(host, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(host, &hostlist_head))
			return (-1);
	}
    break;

  case 14:

/* Line 1455 of yacc.c  */
#line 193 "cfparse.y"
    {
			if (cf_dns_list == NULL)
				cf_dns_list = (yyvsp[(3) - (4)].list);
			else {
				cf_dns_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_dns_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 15:

/* Line 1455 of yacc.c  */
#line 202 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_dns_name_list == NULL) {
				cf_dns_name_list = l;
				cf_dns_name_list->tail = l;
				cf_dns_name_list->next = NULL;
			} else {
				cf_dns_name_list->tail->next = l;
				cf_dns_name_list->tail = l->tail;
			}
		}
    break;

  case 16:

/* Line 1455 of yacc.c  */
#line 217 "cfparse.y"
    {
			if (cf_ntp_list == NULL)
				cf_ntp_list = (yyvsp[(3) - (4)].list);
			else {
				cf_ntp_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_ntp_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 17:

/* Line 1455 of yacc.c  */
#line 226 "cfparse.y"
    {
			if (cf_sip_list == NULL)
				cf_sip_list = (yyvsp[(3) - (4)].list);
			else {
				cf_sip_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_sip_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 18:

/* Line 1455 of yacc.c  */
#line 235 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_sip_name_list == NULL) {
				cf_sip_name_list = l;
				cf_sip_name_list->tail = l;
				cf_sip_name_list->next = NULL;
			} else {
				cf_sip_name_list->tail->next = l;
				cf_sip_name_list->tail = l->tail;
			}
		}
    break;

  case 19:

/* Line 1455 of yacc.c  */
#line 250 "cfparse.y"
    {
			if (cf_dslite_list == NULL)
				cf_dslite_list = (yyvsp[(3) - (4)].list);
			else {
				cf_dslite_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_dslite_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 20:

/* Line 1455 of yacc.c  */
#line 259 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_dslite_name_list == NULL) {
				cf_dslite_name_list = l;
				cf_dslite_name_list->tail = l;
				cf_dslite_name_list->next = NULL;
			} else {
				cf_dslite_name_list->tail->next = l;
				cf_dslite_name_list->tail = l->tail;
			}
		}
    break;

  case 21:

/* Line 1455 of yacc.c  */
#line 274 "cfparse.y"
    {
			if (cf_nis_list == NULL)
				cf_nis_list = (yyvsp[(3) - (4)].list);
			else {
				cf_nis_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_nis_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 22:

/* Line 1455 of yacc.c  */
#line 283 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_nis_name_list == NULL) {
				cf_nis_name_list = l;
				cf_nis_name_list->tail = l;
				cf_nis_name_list->next = NULL;
			} else {
				cf_nis_name_list->tail->next = l;
				cf_nis_name_list->tail = l->tail;
			}
		}
    break;

  case 23:

/* Line 1455 of yacc.c  */
#line 298 "cfparse.y"
    {
			if (cf_nisp_list == NULL)
				cf_nisp_list = (yyvsp[(3) - (4)].list);
			else {
				cf_nisp_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_nisp_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 24:

/* Line 1455 of yacc.c  */
#line 307 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_nisp_name_list == NULL) {
				cf_nisp_name_list = l;
				cf_nisp_name_list->tail = l;
				cf_nisp_name_list->next = NULL;
			} else {
				cf_nisp_name_list->tail->next = l;
				cf_nisp_name_list->tail = l->tail;
			}
		}
    break;

  case 25:

/* Line 1455 of yacc.c  */
#line 322 "cfparse.y"
    {
			if (cf_bcmcs_list == NULL)
				cf_bcmcs_list = (yyvsp[(3) - (4)].list);
			else {
				cf_bcmcs_list->tail->next = (yyvsp[(3) - (4)].list);
				cf_bcmcs_list->tail = (yyvsp[(3) - (4)].list)->tail;
			}
		}
    break;

  case 26:

/* Line 1455 of yacc.c  */
#line 331 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, CFLISTENT_GENERIC, (yyvsp[(3) - (4)].str), NULL);

			if (cf_bcmcs_name_list == NULL) {
				cf_bcmcs_name_list = l;
				cf_bcmcs_name_list->tail = l;
				cf_bcmcs_name_list->next = NULL;
			} else {
				cf_bcmcs_name_list->tail->next = l;
				cf_bcmcs_name_list->tail = l->tail;
			}
		}
    break;

  case 27:

/* Line 1455 of yacc.c  */
#line 346 "cfparse.y"
    {
			if (cf_refreshtime == -1) {
				cf_refreshtime = (yyvsp[(3) - (4)].num);
				if (cf_refreshtime < -1 ||
				    cf_refreshtime > 0xffffffff) {
					/*
					 * refresh time should not be negative
					 * according to the lex definition,
					 * but check it for safety.
					 */
					yyerror("refresh time is out of range");
				}
				if (cf_refreshtime < DHCP6_IRT_MINIMUM) {
					/*
					 * the value MUST NOT be smaller than
					 * IRT_MINIMUM.
					 */
					yyerror("refresh time is too small "
					    "(must not be smaller than %d)",
					    DHCP6_IRT_MINIMUM);
				}
			} else {
				yywarn("multiple refresh times (ignored)");
			}
		}
    break;

  case 28:

/* Line 1455 of yacc.c  */
#line 375 "cfparse.y"
    {
			struct cf_namelist *iapd;

			MAKE_NAMELIST(iapd, (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
    break;

  case 29:

/* Line 1455 of yacc.c  */
#line 384 "cfparse.y"
    {
			struct cf_namelist *iapd;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iapd, zero, (yyvsp[(4) - (6)].list));

			if (add_namelist(iapd, &iapdlist_head))
				return (-1);
		}
    break;

  case 30:

/* Line 1455 of yacc.c  */
#line 398 "cfparse.y"
    {
			struct cf_namelist *iana;

			MAKE_NAMELIST(iana, (yyvsp[(3) - (7)].str), (yyvsp[(5) - (7)].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
    break;

  case 31:

/* Line 1455 of yacc.c  */
#line 407 "cfparse.y"
    {
			struct cf_namelist *iana;
			char *zero;

			if ((zero = strdup("0")) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			MAKE_NAMELIST(iana, zero, (yyvsp[(4) - (6)].list));

			if (add_namelist(iana, &ianalist_head))
				return (-1);
		}
    break;

  case 32:

/* Line 1455 of yacc.c  */
#line 424 "cfparse.y"
    {
		struct cf_namelist *authinfo;

		MAKE_NAMELIST(authinfo, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(authinfo, &authinfolist_head))
			return (-1);
	}
    break;

  case 33:

/* Line 1455 of yacc.c  */
#line 436 "cfparse.y"
    {
		struct cf_namelist *key;

		MAKE_NAMELIST(key, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(key, &keylist_head))
			return (-1);
	}
    break;

  case 34:

/* Line 1455 of yacc.c  */
#line 448 "cfparse.y"
    {
		if (cfswitch_buffer((yyvsp[(2) - (3)].str))) {
			free((yyvsp[(2) - (3)].str));
			return (-1);
		}
		free((yyvsp[(2) - (3)].str));
	}
    break;

  case 35:

/* Line 1455 of yacc.c  */
#line 459 "cfparse.y"
    {
		struct cf_namelist *pool;

		MAKE_NAMELIST(pool, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));

		if (add_namelist(pool, &addrpoollist_head))
			return (-1);
	}
    break;

  case 36:

/* Line 1455 of yacc.c  */
#line 470 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 37:

/* Line 1455 of yacc.c  */
#line 472 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 38:

/* Line 1455 of yacc.c  */
#line 490 "cfparse.y"
    {
		struct cf_list *l;
		struct in6_addr a0, *a;

		if (inet_pton(AF_INET6, (yyvsp[(1) - (1)].str), &a0) != 1) {
			yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (1)].str));
			free((yyvsp[(1) - (1)].str));
			return (-1);
		}
		if ((a = malloc(sizeof(*a))) == NULL) {
			yywarn("can't allocate memory");
			return (-1);
		}
		*a = a0;

		MAKE_CFLIST(l, CFLISTENT_GENERIC, a, NULL);

		(yyval.list) = l;
	}
    break;

  case 39:

/* Line 1455 of yacc.c  */
#line 512 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 40:

/* Line 1455 of yacc.c  */
#line 514 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 41:

/* Line 1455 of yacc.c  */
#line 532 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SEND, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 42:

/* Line 1455 of yacc.c  */
#line 540 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_REQUEST, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 43:

/* Line 1455 of yacc.c  */
#line 548 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_INFO_ONLY, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
    break;

  case 44:

/* Line 1455 of yacc.c  */
#line 556 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ALLOW, NULL, (yyvsp[(2) - (3)].list));

			(yyval.list) = l;
		}
    break;

  case 45:

/* Line 1455 of yacc.c  */
#line 564 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DUID, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 46:

/* Line 1455 of yacc.c  */
#line 572 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESS, (yyvsp[(2) - (3)].prefix),NULL);

			(yyval.list) = l;
		}
    break;

  case 47:

/* Line 1455 of yacc.c  */
#line 580 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFIX, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 48:

/* Line 1455 of yacc.c  */
#line 588 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_PREFERENCE, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);

			(yyval.list) = l;
		}
    break;

  case 49:

/* Line 1455 of yacc.c  */
#line 597 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_SCRIPT, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 50:

/* Line 1455 of yacc.c  */
#line 605 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_DELAYEDKEY, (yyvsp[(2) - (3)].str), NULL);

			(yyval.list) = l;
		}
    break;

  case 51:

/* Line 1455 of yacc.c  */
#line 613 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_RANGE, (yyvsp[(2) - (3)].range), NULL);

			(yyval.list) = l;
		}
    break;

  case 52:

/* Line 1455 of yacc.c  */
#line 621 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DECL_ADDRESSPOOL, (yyvsp[(2) - (3)].pool), NULL);

			(yyval.list) = l;
		}
    break;

  case 53:

/* Line 1455 of yacc.c  */
#line 632 "cfparse.y"
    {
			(yyval.list) = (yyvsp[(1) - (1)].list);
		}
    break;

  case 54:

/* Line 1455 of yacc.c  */
#line 636 "cfparse.y"
    {
			(yyvsp[(1) - (3)].list)->next = (yyvsp[(3) - (3)].list);
			(yyvsp[(1) - (3)].list)->tail = (yyvsp[(3) - (3)].list)->tail;

			(yyval.list) = (yyvsp[(1) - (3)].list);
		}
    break;

  case 55:

/* Line 1455 of yacc.c  */
#line 646 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_RAPID_COMMIT, NULL, NULL);
			/* no value */
			(yyval.list) = l;
		}
    break;

  case 56:

/* Line 1455 of yacc.c  */
#line 654 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_AUTHINFO, NULL, NULL);
			l->ptr = (yyvsp[(2) - (2)].str);
			(yyval.list) = l;
		}
    break;

  case 57:

/* Line 1455 of yacc.c  */
#line 662 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_PD, NULL, NULL);
			l->num = (yyvsp[(2) - (2)].num);
			(yyval.list) = l;
		}
    break;

  case 58:

/* Line 1455 of yacc.c  */
#line 670 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_IA_NA, NULL, NULL);
			l->num = (yyvsp[(2) - (2)].num);
			(yyval.list) = l;
		}
    break;

  case 59:

/* Line 1455 of yacc.c  */
#line 678 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 60:

/* Line 1455 of yacc.c  */
#line 686 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_SIPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 61:

/* Line 1455 of yacc.c  */
#line 694 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DSLITE, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 62:

/* Line 1455 of yacc.c  */
#line 702 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DSLITENAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 63:

/* Line 1455 of yacc.c  */
#line 710 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 64:

/* Line 1455 of yacc.c  */
#line 718 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_DNSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 65:

/* Line 1455 of yacc.c  */
#line 726 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NTP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 66:

/* Line 1455 of yacc.c  */
#line 734 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_REFRESHTIME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 67:

/* Line 1455 of yacc.c  */
#line 742 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NIS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 68:

/* Line 1455 of yacc.c  */
#line 750 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 69:

/* Line 1455 of yacc.c  */
#line 758 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISP, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 70:

/* Line 1455 of yacc.c  */
#line 766 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_NISPNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 71:

/* Line 1455 of yacc.c  */
#line 774 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCS, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 72:

/* Line 1455 of yacc.c  */
#line 782 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, DHCPOPT_BCMCSNAME, NULL, NULL);
			/* currently no value */
			(yyval.list) = l;
		}
    break;

  case 73:

/* Line 1455 of yacc.c  */
#line 793 "cfparse.y"
    {
			struct dhcp6_range range0, *range;		

			memset(&range0, 0, sizeof(range0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (3)].str), &range0.min) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				free((yyvsp[(3) - (3)].str));
				return (-1);
			}
			if (inet_pton(AF_INET6, (yyvsp[(3) - (3)].str), &range0.max) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(3) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				free((yyvsp[(3) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));
			free((yyvsp[(3) - (3)].str));

			if ((range = malloc(sizeof(*range))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*range = range0;

			(yyval.range) = range;
		}
    break;

  case 74:

/* Line 1455 of yacc.c  */
#line 824 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (2)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (2)].str));
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			free((yyvsp[(1) - (2)].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX this field is ignored */
			if ((yyvsp[(2) - (2)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(2) - (2)].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 75:

/* Line 1455 of yacc.c  */
#line 851 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (3)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (3)].str));
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));
			/* validate other parameters later */
			pconf0.plen = 128; /* XXX */
			if ((yyvsp[(2) - (3)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(2) - (3)].num);
			if ((yyvsp[(3) - (3)].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[(3) - (3)].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 76:

/* Line 1455 of yacc.c  */
#line 884 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (4)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (4)].str));
				free((yyvsp[(1) - (4)].str));
				return (-1);
			}
			free((yyvsp[(1) - (4)].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[(3) - (4)].num);
			if ((yyvsp[(4) - (4)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(4) - (4)].num);
			pconf0.vltime = pconf0.pltime;

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 77:

/* Line 1455 of yacc.c  */
#line 911 "cfparse.y"
    {
			struct dhcp6_prefix pconf0, *pconf;		

			memset(&pconf0, 0, sizeof(pconf0));
			if (inet_pton(AF_INET6, (yyvsp[(1) - (5)].str), &pconf0.addr) != 1) {
				yywarn("invalid IPv6 address: %s", (yyvsp[(1) - (5)].str));
				free((yyvsp[(1) - (5)].str));
				return (-1);
			}
			free((yyvsp[(1) - (5)].str));
			/* validate other parameters later */
			pconf0.plen = (yyvsp[(3) - (5)].num);
			if ((yyvsp[(4) - (5)].num) < 0)
				pconf0.pltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.pltime = (u_int32_t)(yyvsp[(4) - (5)].num);
			if ((yyvsp[(5) - (5)].num) < 0)
				pconf0.vltime = DHCP6_DURATION_INFINITE;
			else
				pconf0.vltime = (u_int32_t)(yyvsp[(5) - (5)].num);

			if ((pconf = malloc(sizeof(*pconf))) == NULL) {
				yywarn("can't allocate memory");
				return (-1);
			}
			*pconf = pconf0;

			(yyval.prefix) = pconf;
		}
    break;

  case 78:

/* Line 1455 of yacc.c  */
#line 944 "cfparse.y"
    {
			struct dhcp6_poolspec* pool;		

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[(1) - (2)].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (2)].str));
				return (-1);
			}
			free((yyvsp[(1) - (2)].str));

			/* validate other parameters later */
			if ((yyvsp[(2) - (2)].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[(2) - (2)].num);
			pool->vltime = pool->pltime;

			(yyval.pool) = pool;
		}
    break;

  case 79:

/* Line 1455 of yacc.c  */
#line 969 "cfparse.y"
    {
			struct dhcp6_poolspec* pool;		

			if ((pool = malloc(sizeof(*pool))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			if ((pool->name = strdup((yyvsp[(1) - (3)].str))) == NULL) {
				yywarn("can't allocate memory");
				free((yyvsp[(1) - (3)].str));
				return (-1);
			}
			free((yyvsp[(1) - (3)].str));

			/* validate other parameters later */
			if ((yyvsp[(2) - (3)].num) < 0)
				pool->pltime = DHCP6_DURATION_INFINITE;
			else
				pool->pltime = (u_int32_t)(yyvsp[(2) - (3)].num);
			if ((yyvsp[(3) - (3)].num) < 0)
				pool->vltime = DHCP6_DURATION_INFINITE;
			else
				pool->vltime = (u_int32_t)(yyvsp[(3) - (3)].num);

			(yyval.pool) = pool;
		}
    break;

  case 80:

/* Line 1455 of yacc.c  */
#line 1000 "cfparse.y"
    {
			(yyval.num) = -1;
		}
    break;

  case 81:

/* Line 1455 of yacc.c  */
#line 1004 "cfparse.y"
    {
			(yyval.num) = (yyvsp[(1) - (1)].num);
		}
    break;

  case 82:

/* Line 1455 of yacc.c  */
#line 1010 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 83:

/* Line 1455 of yacc.c  */
#line 1012 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 84:

/* Line 1455 of yacc.c  */
#line 1029 "cfparse.y"
    { (yyval.list) = (yyvsp[(1) - (1)].list); }
    break;

  case 85:

/* Line 1455 of yacc.c  */
#line 1031 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_PREFIX, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 86:

/* Line 1455 of yacc.c  */
#line 1042 "cfparse.y"
    {
		struct cf_list *ifl;

		MAKE_CFLIST(ifl, IACONF_PIF, (yyvsp[(2) - (6)].str), (yyvsp[(4) - (6)].list));
		(yyval.list) = ifl;
	}
    break;

  case 87:

/* Line 1455 of yacc.c  */
#line 1051 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 88:

/* Line 1455 of yacc.c  */
#line 1053 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 89:

/* Line 1455 of yacc.c  */
#line 1071 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_ID, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 90:

/* Line 1455 of yacc.c  */
#line 1079 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IFPARAM_SLA_LEN, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 91:

/* Line 1455 of yacc.c  */
#line 1089 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 92:

/* Line 1455 of yacc.c  */
#line 1091 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 93:

/* Line 1455 of yacc.c  */
#line 1109 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, IACONF_ADDR, (yyvsp[(2) - (3)].prefix), NULL);

			(yyval.list) = l;
		}
    break;

  case 94:

/* Line 1455 of yacc.c  */
#line 1119 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 95:

/* Line 1455 of yacc.c  */
#line 1121 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 96:

/* Line 1455 of yacc.c  */
#line 1139 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_PROTO, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 97:

/* Line 1455 of yacc.c  */
#line 1147 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_ALG, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 98:

/* Line 1455 of yacc.c  */
#line 1155 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_RDM, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 99:

/* Line 1455 of yacc.c  */
#line 1163 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, AUTHPARAM_KEY, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 100:

/* Line 1455 of yacc.c  */
#line 1173 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHPROTO_DELAYED; }
    break;

  case 101:

/* Line 1455 of yacc.c  */
#line 1174 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHPROTO_RECONFIG; }
    break;

  case 102:

/* Line 1455 of yacc.c  */
#line 1178 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHALG_HMACMD5; }
    break;

  case 103:

/* Line 1455 of yacc.c  */
#line 1182 "cfparse.y"
    { (yyval.num) = DHCP6_AUTHRDM_MONOCOUNTER; }
    break;

  case 104:

/* Line 1455 of yacc.c  */
#line 1186 "cfparse.y"
    { (yyval.list) = NULL; }
    break;

  case 105:

/* Line 1455 of yacc.c  */
#line 1188 "cfparse.y"
    {
			struct cf_list *head;

			if ((head = (yyvsp[(1) - (2)].list)) == NULL) {
				(yyvsp[(2) - (2)].list)->next = NULL;
				(yyvsp[(2) - (2)].list)->tail = (yyvsp[(2) - (2)].list);
				head = (yyvsp[(2) - (2)].list);
			} else {
				head->tail->next = (yyvsp[(2) - (2)].list);
				head->tail = (yyvsp[(2) - (2)].list)->tail;
			}

			(yyval.list) = head;
		}
    break;

  case 106:

/* Line 1455 of yacc.c  */
#line 1206 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_REALM, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 107:

/* Line 1455 of yacc.c  */
#line 1214 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_KEYID, NULL, NULL);
			l->num = (yyvsp[(2) - (3)].num);
			(yyval.list) = l;
		}
    break;

  case 108:

/* Line 1455 of yacc.c  */
#line 1222 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_SECRET, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;

  case 109:

/* Line 1455 of yacc.c  */
#line 1230 "cfparse.y"
    {
			struct cf_list *l;

			MAKE_CFLIST(l, KEYPARAM_EXPIRE, NULL, NULL);
			l->ptr = (yyvsp[(2) - (3)].str);
			(yyval.list) = l;
		}
    break;



/* Line 1455 of yacc.c  */
#line 3233 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1675 of yacc.c  */
#line 1239 "cfparse.y"

/* supplement routines for configuration */
static int
add_namelist(new, headp)
	struct cf_namelist *new, **headp;
{
	struct cf_namelist *n;
	
	/* check for duplicated configuration */
	for (n = *headp; n; n = n->next) {
		if (strcmp(n->name, new->name) == 0) {
			yywarn("duplicated name: %s (ignored)",
			       new->name);
			cleanup_namelist(new);
			return (0);
		}
	}

	new->next = *headp;
	*headp = new;

	return (0);
}

/* free temporary resources */
static void
cleanup()
{
	cleanup_namelist(iflist_head);
	iflist_head = NULL;
	cleanup_namelist(hostlist_head);
	hostlist_head = NULL;
	cleanup_namelist(iapdlist_head);
	iapdlist_head = NULL;
	cleanup_namelist(ianalist_head);
	ianalist_head = NULL;
	cleanup_namelist(authinfolist_head);
	authinfolist_head = NULL;
	cleanup_namelist(keylist_head);
	keylist_head = NULL;
	cleanup_namelist(addrpoollist_head);
	addrpoollist_head = NULL;

	cleanup_cflist(cf_sip_list);
	cf_sip_list = NULL;
	cleanup_cflist(cf_sip_name_list);
	cf_sip_name_list = NULL;
	cleanup_cflist(cf_dslite_list);
	cf_dslite_list = NULL;
	cleanup_cflist(cf_dslite_name_list);
	cf_dslite_name_list = NULL;
	cleanup_cflist(cf_dns_list);
	cf_dns_list = NULL;
	cleanup_cflist(cf_dns_name_list);
	cf_dns_name_list = NULL;
	cleanup_cflist(cf_ntp_list);
	cf_ntp_list = NULL;
	cleanup_cflist(cf_nis_list);
	cf_nis_list = NULL;
	cleanup_cflist(cf_nis_name_list);
	cf_nis_name_list = NULL;
	cleanup_cflist(cf_nisp_list);
	cf_nisp_list = NULL;
	cleanup_cflist(cf_nisp_name_list);
	cf_nisp_name_list = NULL;
	cleanup_cflist(cf_bcmcs_list);
	cf_bcmcs_list = NULL;
	cleanup_cflist(cf_bcmcs_name_list);
	cf_bcmcs_name_list = NULL;
}

static void
cleanup_namelist(head)
	struct cf_namelist *head;
{
	struct cf_namelist *ifp, *ifp_next;

	for (ifp = head; ifp; ifp = ifp_next) {
		ifp_next = ifp->next;
		cleanup_cflist(ifp->params);
		free(ifp->name);
		free(ifp);
	}
}

static void
cleanup_cflist(p)
	struct cf_list *p;
{
	struct cf_list *n;

	if (p == NULL)
		return;

	n = p->next;
	if (p->type == DECL_ADDRESSPOOL) {
		free(((struct dhcp6_poolspec *)p->ptr)->name);
	}
	if (p->ptr)
		free(p->ptr);
	if (p->list)
		cleanup_cflist(p->list);
	free(p);

	cleanup_cflist(n);
}

#define config_fail() \
	do { cleanup(); configure_cleanup(); return (-1); } while(0)

int
cf_post_config()
{
	if (configure_keys(keylist_head))
		config_fail();

	if (configure_authinfo(authinfolist_head))
		config_fail();

	if (configure_ia(iapdlist_head, IATYPE_PD))
		config_fail();

	if (configure_ia(ianalist_head, IATYPE_NA))
		config_fail();

	if (configure_pool(addrpoollist_head))
		config_fail();

	if (configure_interface(iflist_head))
		config_fail();

	if (configure_host(hostlist_head))
		config_fail();

	if (configure_global_option())
		config_fail();

	configure_commit();
	cleanup();
	return (0);
}
#undef config_fail

void
cf_init()
{
	iflist_head = NULL;
}

