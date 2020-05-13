#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_descent.h"

extern enum token yylex(void);
extern char* yytext;
struct counter game();
struct counter gamelist();
int yearExp(int currentToken);
void start();
void match();
int lookahead;


void line(){
	match(TITLE);
	match(NEWLINE);
	struct counter result = gamelist();
	double avg = (double)result.year/(double)result.c;
	printf("\naverage number of games per sport:%7.2f\n", avg);
}

struct counter gamelist(){
	struct counter gameResult;
	struct counter gamelistResult;
	gamelistResult.c = 0;
	gamelistResult.year = 0;
	while (yylex() == NEWLINE){
		gameResult = game();
		gamelistResult.c += gameResult.c;
		gamelistResult.year += gameResult.year;
	}
}

struct counter game(){
	match(SPORT);
	match(SPORT_NAME);
	char sportName[30];
	strcpy(sportName, yytext);
	match(YEARS);
	printf("%s", yylex());
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
	if(yylex() == COMMA ){
		return yearExp(currentToken) + yearExp(yylex());
	}
	else if(currentToken == SINCE){
		return ((2016 - yylex())/4)+1;
	}
	else if (currentToken == ALL){
		return ((2016-1896)/4)+1;
	}
	else if (yylex() == THROUGH){
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
		sprintf(e, "error: expected token %s, found token %s",
				token_name(expectedToken), token_name(lookahead));
		errorMsg(e);
		exit(1);
	}
}

void parse()
{
	lookahead = yylex();
	start();
	if (lookahead != 0)
	{ // 0 means EOF
		errorMsg("EOF expected");
		exit(1);
	}
}

int main(int argc, char **argv)
{
	extern FILE *yyin;
	// if (argc != 2)
	// {
	// 	fprintf(stderr, "Usage: %s <input-file-name>\n", argv[0]);
	// 	return 1;
	// }

	// yyin = fopen(argv[1], "r");
	// if (yyin == NULL)
	// {
	// 	fprintf(stderr, "failed to open %s\n", argv[1]);
	// 	return 2;
	// }

	yyin = fopen("input.txt", "r");
	printf("sports which appeared in at least 7 olympic games:\n");

	parse();

	fclose(yyin);
	return 0;
}

void errorMsg(const char *s)
{
	extern int yylineno;
	fprintf(stderr, "line %d: %s\n", yylineno, s);
}