%{
/* Daniil Rolnik 334018009 | Elia Ben Anat 308048388 | Margarita Fridman 31267942
/* we wanted to deal with "--" using next regex [-][^-] but in your example you had 1900-1908 without spaces so it caused problems with year number recognition */
#include "recursive_descent.h"
#include <string.h>
extern int atoi (const char *);
extern void exit(int);

union _lexVal lexicalValue; // semantic value of current token

%}

%option noyywrap
/*  this will cause  flex to maintain the current input line number in 
    the global variable int yylineno.
*/	
%option yylineno

/* exclusive start condition -- deals with C++ style comments */ 

%x COMMENT

%%

"<sport>"  { return SPORT; }

"<years>" { return YEARS; }

\"([A-Z][a-z]+[ ]{0,1})*\" { strcpy(lexicalValue._counter.cname , yytext+1); 
lexicalValue._counter.cname[strlen(yytext)-2] = '\0';
return SPORT_NAME; }

([A-Z][a-z]+[ ]{0,1})* {  return TITLE; }

2020|20[0-1][0-9]|19[0-9][0-9]|189[6-9] {  lexicalValue._counter.year = atoi(yytext); return YEAR_NUM; }

"," { return COMMA; }

"-"|"through" {return THROUGH; }

"since" {return SINCE; }

"all" {return ALL; }


[\t ]+  /* skip white space */

\n { }

"//"       { BEGIN (COMMENT); }

<COMMENT>.+ /* skip comment */
<COMMENT>\n {  /* end of comment --> resume normal processing */
                BEGIN (0); }

.          { fprintf (stderr, "unrecognized token %c on line %d \n", yytext[0], yylineno); }

%%
