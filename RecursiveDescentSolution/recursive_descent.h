//#ifndef MADONNA

// yylex returns 0 when EOF is encountered
enum token {
     TITLE =1, 
     SPORT,
     YEARS,
     COMMA,
     THROUGH,
     SINCE,
     ALL, 
     NEWLINE,
     YEAR_NUM,
     SPORT_NAME    
};

char *token_name(enum token token);

struct counter
{
   int c;
   int year;
   char cname[30];
};

// We don't really need a union here because only there is only one field.
union _lexVal{
   struct counter _counter;
};

extern union _lexVal lexicalValue;

void errorMsg(const char *s);

//#endif
