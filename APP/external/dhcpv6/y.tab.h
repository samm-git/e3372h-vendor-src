
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1676 of yacc.c  */
#line 128 "cfparse.y"

	long long num;
	char* str;
	struct cf_list *list;
	struct dhcp6_prefix *prefix;
	struct dhcp6_range *range;
	struct dhcp6_poolspec *pool;



/* Line 1676 of yacc.c  */
#line 205 "y.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


