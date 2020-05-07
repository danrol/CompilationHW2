/* 
    This is an example of a recursive descent parser.
    
This  program reads a list  of songs  from its input.
It prints the length (in minutes and seconds) of the shortest song
   that satisfies the following conditions:
     (1)  Its length is at least 4:02  (4 minutes and 2 seconds)
     (2)  The artist is known by one name only (for example
          Madonna  but not Joe Cocker)
          
For an example of an input, see file madonna.txt 

To prepare the program, issue the following commands from
  The command line:
  
  flex madonna.lex    (This will generate a file called lex.yy.c)
    
  compile the program using a C compiler
  for example: 
       gcc lex.yy.c madonna.c -o madonna.exe
       
  (note that file madonna.h is part of the program (it's included in
     other files)
       
  The input file for the program should be supplied as a command line argument
  for example:
      madonna.exe  madonna.txt
      
      
  Here is a grammar describing the input
  (tokens are written using uppercase letters):
    start -> PLAYLIST songlist 
    songlist -> songlist song | epsilon
    song -> SEQ_NUM  SONG SONG_NAME  ARTIST artist_name LENGTH SONG_LENGTH
    artist_name -> NAME | NAME NAME
    
    Note that this grammar is not LL(1) because of the left recursion
    in the production for songlist and because of the productions
    for artist_name. It is not hard to find an equivalent LL(1) grammar
    but this was not necessary. See the functions songlist()
    and artist_name().

*/
#include <stdio.h>
#include <stdlib.h>  /* for exit() */
#include "recursive_descent.h"
  
extern enum token yylex (void);
int lookahead;

struct time
min_time (struct time t1, struct time t2);

// the recursive descent parser
void start();
struct time songlist();
struct time song(); 
int artist_name();

void match(int expectedToken)
{
    if (lookahead == expectedToken)
        lookahead = yylex();
    else {
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
    if (lookahead != 0) {  // 0 means EOF
        errorMsg("EOF expected");
        exit(1);
    }
}

void 
start()
{
    match(PLAYLIST);
    struct time t = songlist();
    if (t.minutes == -1) 
        printf ("no relevant song\n");
    else
        printf ("time for shortest relevant song: %d:%.2d\n",
                t.minutes, t.seconds);
}
    
struct time
songlist()
{
    struct time shortest = {-1,-1};
    while (lookahead == SEQ_NUM) {
        struct time t = song();
        shortest = min_time(shortest, t);
    }
    return shortest;
}

struct time
song() 
{
    match(SEQ_NUM);
    match(SONG);
    match(SONG_NAME);
    match(ARTIST);
    int n = artist_name();
    match(LENGTH);
    
    struct time t = lexicalValue._time;
    match(SONG_LENGTH);
    if (n == 1 && (t.minutes > 4 || t.minutes == 4 &&
                                    t.seconds >= 2))
         return t;
    else {
        struct time s = {-1, -1};
        return s; // this song will be ignored
    }
}

int artist_name() 
{
    match(NAME);
    if (lookahead == NAME) {
        match(NAME);
        return 2;  // artist has 2 names
    } else
        return 1; // artist has one name
}

int
main (int argc, char **argv)
{
    extern FILE *yyin;
    if (argc != 2) {
       fprintf (stderr, "Usage: %s <input-file-name>\n", argv[0]);
	   return 1;
    }
    yyin = fopen (argv [1], "r");
    if (yyin == NULL) {
       fprintf (stderr, "failed to open %s\n", argv[1]);
	   return 2;
    }
  
    parse();
  
    fclose (yyin);
    return 0;
}

void errorMsg(const char *s)
{
  extern int yylineno;
  fprintf (stderr, "line %d: %s\n", yylineno, s);
}

struct time
min_time (struct time t1, struct time t2)
{
     if (t1.minutes == -1)
         return t2;
     else if (t2.minutes == -1)
         return t1;
         
     if (t1.minutes < t2.minutes || (t1.minutes == t2.minutes &&
                                     t1.seconds < t2.seconds))
          return t1;
     return t2;
}




