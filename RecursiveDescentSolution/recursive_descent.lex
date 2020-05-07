%{
#include "recursive_descent.h"
extern int atoi (const char *);
extern void exit(int);

union _lexVal lexicalValue; // semantic value of current token
%}

%option noyywrap
%option yylineno

%%
"[playlist]"  { return PLAYLIST; }
"[song]"      { return SONG; }
"[artist]"    { return ARTIST; }
"[length]"    { return LENGTH; }

[0-9]+     { return SEQ_NUM; }

\"[A-Za-z][A-Za-z ]*\"   { return SONG_NAME; }

[A-Za-z]+      { return NAME; }

[0-9]":"[0-9][0-9]  { lexicalValue._time.minutes = yytext[0] - '0';
                      lexicalValue._time.seconds = atoi(yytext+2);
                      return SONG_LENGTH; }
                      
[\t\n ]+   /* skip white space */

.          { char e[100];
            sprintf(e, "unrecognized token %c", yytext[0]);  
            errorMsg(e);  
             exit(1);
           }

%%
/* useful for error messages */
char *token_name(enum token token)
{
    static char *names[] = {
         "EOF", "PLAYLIST", "SEQ_NUM", "SONG", "SONG_NAME",
         "ARTIST", "NAME", "LENGTH", "SONG_LENGTH" };
    
    return token <= SONG_LENGTH ? names[token] : "unknown token";
}
