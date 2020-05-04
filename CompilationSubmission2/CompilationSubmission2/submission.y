%{ 
	#define _CRT_SECURE_NO_DEPRECATE
    #include <stdio.h> 
	#include "stdafx.h"
    #include <iostream> using namespace std;

  // stuff from flex that bison needs to know about: 
  extern int yylex(); 
  extern int yyparse(); 
  extern FILE *yyin;

  void yyerror(const char *s); 
%}

/* note: no semicolon after the union */ 

%union { 
int ival; 
}

// define the "terminal symbol" token types I'm going to use (in CAPS 
// by convention), and associate each with a field of the union: 
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! 
%token SPORT SPORT_NAME SEMICOLON TITLE YEARS YEAR_NUM COMMA THROUGH SINCE ALL

%% 
line: TITLE { printf("title"); } ;

| SPORT SPORT_NAME YEARS year_exp ;

year_exp: YEAR_NUM { printf("title"); } |  year_exp SEMICOLON | SINCE YEAR_NUM { 
printf("title"); } | ALL { printf("title"); } |  YEAR_NUM THROUGH YEAR_NUM { 
printf("title"); } ;


%%

int main (int argc, char **argv)
{
   int token;

   if (argc != 2) {
      fprintf(stderr, "Usage: mylex <input file name>\n", argv [0]);
      exit (1);
   }

   yyin = fopen (argv[1], "r");
   printf ("TOKEN\t\t\tLEXEME\t\t\tSEMANTIC VALUE\n");
   printf ("-----------------------------------------------------------------------\n");

  // Parse through the input:
  yyparse();
   fclose (yyin);
}

void yyerror(const char *s) {
  cout << "EEK, parse error!  Message: " << s << endl;
  // might as well halt now:
  exit(-1);
}