%{ 
	#define _CRT_SECURE_NO_DEPRECATE
    #include <stdio.h> 
    #include <iostream> 
	#include <string.h>
	using namespace std;

	#define SIZE 30

  // stuff from flex that bison needs to know about: 
  extern int yylex(); 
  extern int yyparse(); 
  extern FILE *yyin;

  void yyerror(const char *s); 
%}

/* note: no semicolon after the union */ 



%code requires {
	

	

	struct counter{
		int c;
		int year;
		char cname[30];
		
		
	};
}

%union { 
	int year;
	int number;
	int avr;
	char oname[30];
	struct counter count;
}


// define the "terminal symbol" token types I'm going to use (in CAPS 
// by convention), and associate each with a field of the union: 
%token SPORT  TITLE YEARS COMMA THROUGH SINCE ALL NEWLINE 
%token <year> YEAR_NUM  
%type <year> year_exp
%token <oname> SPORT_NAME
%type <count> gamelist game 




%% 
line: TITLE NEWLINE gamelist
{
printf("year = %d, c = %d", $3.year, $3.c);
double avg=(double)$3.year/(double)$3.c;
//printf("the sum is %d\n",$3.year);
printf("\naverage number of games per sport:%7.2f\n",avg);
}

gamelist: /* empty */{};

gamelist: gamelist game NEWLINE 
{

	//TODO deal with negative memory
	if($$.c<0)
	{
		$$.c=1;

	}else
	{
		$$.c=$1.c+1;
	}
	if($$.year<0)
	{
		$$.year=$2.year;

	}else
	{
		$$.year = $1.year+$2.year;
	}

//printf("%d\n" , $$.c);
//printf("%d\n" , $$.year);

} 

game: SPORT SPORT_NAME YEARS year_exp
{ if($4 >= 7)
	{
	printf("%s\n" , $2);
	}
 $$.c=1;
 $$.year=$4;

 }

year_exp: YEAR_NUM {if($$ != 2020)$$= 1; else $$ = 0; }
year_exp:year_exp COMMA year_exp {$$ = $1 + $3;}
year_exp:SINCE YEAR_NUM { $$ = ((2016-$2)/4)+1;}
year_exp:ALL { $$ = ((2016-1896)/4)+1;  } 
year_exp:YEAR_NUM THROUGH YEAR_NUM {$$ = (($3-$1)/4)+1;}


%%
int main (int argc, char **argv)
{
   /*
   if (argc != 2) {
      fprintf(stderr, "Usage: mylex <input file name>\n", argv [0]);
      exit (1);
   }
   */

   yyin = fopen ("input.txt", "r");
   printf("sports which appeared in at least 7 olympic games:\n");

  // Parse through the input:
  yyparse();
 
   fclose (yyin);
 
}

void yyerror(const char *s) {
  printf ("EEK, parse error!  Message");
  // might as well halt now:
  exit(-1);
}