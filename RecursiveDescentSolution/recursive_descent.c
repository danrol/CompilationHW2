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
	printf("\naverage number of games per sport:%7.2f\n", avg);
}

struct counter gamelist(){
	struct counter gameResult;
	struct counter gamelistResult;
	int lineCounter = 2;
	gamelistResult.c = 0;
	gamelistResult.year = 0;
	printf("lookahead before yylex = %d\n", lookahead);
	while (lookahead == SPORT){
		printf("\n\nSPORT ");
		lookahead = yylex();
		gameResult = game();
		gamelistResult.c += gameResult.c;
		printf("line #%d gameResult.c = %d, gameResult.year = %d\n", lineCounter, gameResult.year, gameResult.c);
		gamelistResult.year += gameResult.year;
		lineCounter++;
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
	struct counter game;
	game.c = 1;
	game.year = 0;

		// printf("!!!!!!!!!!!!!!!!!!!!!!! %s", yylex());
		while (lookahead != SPORT && lookahead != 0)
	{
		int yearResult = yearExp(lookahead);
		if (yearResult >= 7)
		{
			printf("%s\n", sportName);
		}
		game.year += yearResult;
		lookahead = yylex();
	}
	return game;
}

int yearExp(int currentToken)
{
	char currentTokenValue[30];
	strcpy(currentTokenValue, yytext);
	char nextTokenValue[30];
	
	printf("beginning of yearExp with current token = %d\n", currentToken);
	int result;

	if (currentToken == COMMA)
	{
		printf("COMMA ");
		// result = yearExp(currentToken) + yearExp(yylex());
		result = 0;
		// printf("yearExp result = %d", result);
		return result;
	}
	else if (currentToken == ALL)
	{
		printf("ALL ");
		result = ((2016 - 1896) / 4) + 1;
		printf("yearExp result = %d ", result);
		return result;
	}
	int nextToken = yylex();
	printf(" next token = %d\n", nextToken);
	strcpy(nextTokenValue, yytext);
	if(currentToken == SINCE){
		printf("SINCE ");
		result = ((2016 - atoi(nextTokenValue)) / 4) + 1;
		printf("yearExp result = %d ", result);
		return result;
	}
	else if (nextToken == THROUGH)
	{
		printf("THROUGH ");
		yylex();
		result = ((atoi(yytext) - atoi(currentTokenValue))/4)+1;
		printf("yearExp result = %d ", result);
		return result;
	}
	else if (currentToken == YEAR_NUM)
	{
		int year_num = atoi(yytext);
		printf("YEAR_NUM ");
		if (year_num != 2020)
		{
			printf("year num returns 1 \n");
			return 1;
		}
		else
		{
			printf("year num returns 0 \n");
			return 0;
		}
	}
	else{
		//TODO deal with other
		printf("\n entered else in year_exp\n");
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