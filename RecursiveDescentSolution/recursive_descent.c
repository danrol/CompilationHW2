#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "recursive_descent.h"

extern enum token yylex(void);
extern char *yytext;
void line();
struct counter game();
struct counter gamelist();
int yearExp(int currentToken);
void match(int expectedToken);
void parse();
int lookahead;

void line()
{
	match(TITLE);
	struct counter result = gamelist();
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
	while (lookahead == SPORT)
	{
		lookahead = yylex();
		gameResult = game();
		gamelistResult.c += gameResult.c;
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
	match(YEARS);
	struct counter game;
	game.c = 1;
	game.year = 0;

	while (lookahead != SPORT && lookahead != 0)
	{
		int yearResult = yearExp(lookahead);
		if (yearResult >= 7)
		{
			printf("%s\n", sportName);
		}
		game.year += yearResult;
	}
	return game;
}

int yearExp(int currentToken)
{
	char currentTokenValue[30];
	strcpy(currentTokenValue, yytext);
	char nextTokenValue[30];

	int result;
	int updateLookahead = 1;
	int nextToken;

	if (currentToken == COMMA)
	{
		result = 0;
	}
	else if (currentToken == ALL)
	{
		result = ((2016 - 1896) / 4) + 1;
	}
	else
	{
		nextToken = yylex();
		strcpy(nextTokenValue, yytext);
		if (currentToken == SINCE)
		{
			result = ((2016 - atoi(nextTokenValue)) / 4) + 1;
		}
		else if (nextToken == THROUGH)
		{
			nextToken = yylex();
			result = ((atoi(yytext) - atoi(currentTokenValue)) / 4) + 1;
		}
		else if (currentToken == YEAR_NUM)
		{
			updateLookahead = 0;
			int year_num = atoi(currentTokenValue);
			if (year_num != 2020)
			{
				result = 1;
			}
			else
			{
				result = 0;
			}
		}
		else
		{
			printf("\n entered else in year_exp\n");
		}
	}
	if (updateLookahead == 1)
	{
		lookahead = yylex();
	}
	else
	{
		lookahead = nextToken;
	}
	return result;
}

void match(int expectedToken)
{
	if (lookahead == expectedToken)
		lookahead = yylex();
	else
	{
		char e[100]; 
		
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
	{
		// 0 means EOF
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

	printf("Sports which appeared in at least 7 olympic games:\n");
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