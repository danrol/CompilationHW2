#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_descent.h"

extern enum token yylex(void);
extern char* yytext;
void line();
struct counter game();
struct counter gamelist();
int yearExp(int currentToken);
void match(int expectedToken);
void parse();
// void start();
int lookahead;

// void start()
// {
// 	line();
// 		match() struct time t = songlist();
// 	if (t.minutes == -1)
// 		printf("no relevant song\n");
// 	else
// 		printf("time for shortest relevant song: %d:%.2d\n",
// 			   t.minutes, t.seconds);
// }

void line(){
	match(TITLE);
	printf("TITLE \n ");
	// match(NEWLINE);
	struct counter result = gamelist();
	printf("result.year = %d, result.c = %d \n", result.year, result.c);
	double avg = (double)result.year/(double)result.c;
	printf("\n average number of games per sport:%7.2f\n", avg);
}

struct counter gamelist(){
	struct counter gameResult;
	struct counter gamelistResult;
	gamelistResult.c = 0;
	gamelistResult.year = 0;
	printf("lookahead before yylex = %d\n", lookahead);
	while (lookahead == SPORT){
		printf("SPORT ");
		lookahead = yylex();
		gameResult = game();
		gamelistResult.c += gameResult.c;
		gamelistResult.year += gameResult.year;
		// lookahead = yylex();
	}
}

struct counter game(){
	//match(SPORT);
	match(SPORT_NAME);
	printf("SPORT_NAME ");
	char sportName[30];
	strcpy(sportName, yytext);
	match(YEARS);
	printf("YEARS ");

	// printf("!!!!!!!!!!!!!!!!!!!!!!! %s", yylex());
	int yearResult = yearExp(yylex());
	if(yearResult >= 7){
		printf("%s\n", sportName);
	}
	struct counter game;
	game.c = 1;
	game.year = yearResult;
	return game;
}

int yearExp(int currentToken)
{
	printf("beginning of yearExp\n");
	if(yylex() == COMMA ){
		printf("COMMA ");
		return yearExp(currentToken) + yearExp(yylex());
	}
	else if(currentToken == SINCE){
		printf("SINCE ");
		return ((2016 - yylex())/4)+1;
	}
	else if (currentToken == ALL){
		printf("SPORT_NAME");
		return ((2016-1896)/4)+1;
	}
	else if (yylex() == THROUGH){
		printf("THROUGH");
		return ((yylex() - currentToken)/4)+1;
	}
	else{
		//TODO deal with other
	}
	if (currentToken == YEAR_NUM){
		int year_num = atoi(yytext);
		if (year_num != 2020){
			return 1;
		}
		else{
			return 0;
		}
	}
}

void match(int expectedToken)
{
	if (lookahead == expectedToken)
		lookahead = yylex();
	else
	{
		char e[100]; /* todo: error message should mention name of token
                   (not its number) */
		if (!lookahead){
			printf("!!!!! inside else and lookahead is null ");
			printf("%d ", lookahead);
		}
		printf("expected token is %d ", expectedToken);
		printf("but received lookahead token = %d ", lookahead);
		// sprintf(e, "error: expected token %s, found token %s",
		// 		token_name(expectedToken), token_name(lookahead));
		printf("problem with tokens");
		// errorMsg(e);
		exit(1);
	}
}

void parse()
{
	lookahead = yylex();
	line();
	if (lookahead != 0)
	{ // 0 means EOF
		printf("error lookahead = %d \n", lookahead);
		errorMsg("EOF expected");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	extern FILE *yyin;
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <input-file-name>\n", argv[0]);
		return 1;
	}

	yyin = fopen(argv[1], "r");
	if (yyin == NULL)
	{
		fprintf(stderr, "failed to open %s\n", argv[1]);
		return 2;
	}

	// yyin = fopen("input.txt", "r");
	// printf("sports which appeared in at least 7 olympic games:\n");

	parse();

	fclose(yyin);
	return 0;
}

void errorMsg(const char *s)
{
	extern int yylineno;
	fprintf(stderr, "line %d: %s\n", yylineno, s);
}