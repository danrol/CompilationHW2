#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_descent.h"

extern enum token yylex(void);
extern char *yytext;
void line();
struct counter game();
struct counter gamelist();
int yearExp(int currentToken, int *lookahead);
void match(int expectedToken);
void parse();
int lookahead;

void line()
{
	match(TITLE);
	printf("TITLE \n ");
	// match(NEWLINE);
	struct counter result = gamelist();
	printf("result.year = %d, result.c = %d \n", result.year, result.c);
	double avg = (double)result.year / (double)result.c;
	printf("\naverage number of games per sport:%7.2f\n", avg);
}

struct counter gamelist()
{
	struct counter gameResult;
	struct counter gamelistResult;
	int lineCounter = 2;
	gamelistResult.c = 0;
	gamelistResult.year = 0;
	printf("lookahead before yylex = %d\n", lookahead);
	while (lookahead == SPORT)
	{
		printf("\n\nSPORT ");
		lookahead = yylex();
		gameResult = game();
		gamelistResult.c += gameResult.c;
		printf("\n\nline #%d gameResult.c = %d, gameResult.year = %d\n", lineCounter, gameResult.year, gameResult.c);
		gamelistResult.year += gameResult.year;
		lineCounter++;
	}
	return gamelistResult;
}

struct counter game()
{
	char sportName[30];
	strcpy(sportName, yytext);
	match(SPORT_NAME);
	printf("SPORT_NAME ");
	match(YEARS);
	printf("YEARS ");
	struct counter game;
	game.c = 1;
	game.year = 0;

	while (lookahead != SPORT && lookahead != 0)
	{
		int yearResult = yearExp(lookahead, &lookahead);
		if (yearResult >= 7)
		{
			printf("%s\n", sportName);
		}
		game.year += yearResult;
		// printf("lookahead token at the end of while = %d", lookahead);
	}
	return game;
}

int yearExp(int currentToken, int *lookahead)
{
	char currentTokenValue[30];
	strcpy(currentTokenValue, yytext);
	char nextTokenValue[30];

	printf("beginning of yearExp with current token = %d\n", currentToken);
	int result;
	int updateLookahead = 1;
	int nextToken;

		if (currentToken == COMMA)
	{
		printf("COMMA ");
		result = 0;
	}
	else if (currentToken == ALL)
	{
		printf("ALL ");
		result = ((2016 - 1896) / 4) + 1;
		printf("yearExp result = %d ", result);
	}
	else
	{
		nextToken = yylex();
		printf(" next token = %d\n", nextToken);
		strcpy(nextTokenValue, yytext);
		if (currentToken == SINCE)
		{
			printf("SINCE ");
			result = ((2016 - atoi(nextTokenValue)) / 4) + 1;
			printf("yearExp result = %d ", result);
		}
		else if (nextToken == THROUGH)
		{
			printf("THROUGH ");
			nextToken = yylex();
			result = ((atoi(yytext) - atoi(currentTokenValue)) / 4) + 1;
			printf("yearExp result = %d ", result);
		}
		else if (currentToken == YEAR_NUM)
		{
			updateLookahead = 0;
			int year_num = atoi(currentTokenValue);
			printf("YEAR_NUM and year_num = %d ", year_num);
			if (year_num != 2020)
			{
				printf("year num returns 1 \n");
				result = 1;
			}
			else
			{
				printf("year num returns 0 \n");
				result = 0;
			}
		}
		else
		{
			printf("\n entered else in year_exp\n");
		}
	}
	if(updateLookahead == 1){
		*lookahead = yylex();
	}
	else{
		*lookahead = nextToken;
	}
	return result;
}

void match(int expectedToken)
{
	if (lookahead == expectedToken)
		lookahead = yylex();
	else
	{
		char e[100]; /* todo: error message should mention name of token
                   (not its number) */
		if (!lookahead)
		{
			printf("!!!!! inside else and lookahead is null ");
			printf("%d ", lookahead);
		}
		printf("expected token is %d ", expectedToken);
		printf("but received lookahead token = %d ", lookahead);
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

	parse();

	fclose(yyin);
	return 0;
}

void errorMsg(const char *s)
{
	extern int yylineno;
	fprintf(stderr, "line %d: %s\n", yylineno, s);
}