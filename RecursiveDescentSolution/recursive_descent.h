//#ifndef MADONNA

// yylex returns 0 when EOF is encountered
enum token
{
   TITLE = 1,
   SPORT,       //2
   YEARS,       //3
   COMMA,       //4
   THROUGH,     //5
   SINCE,       //6
   ALL,         //7
   YEAR_NUM,    //8
   SPORT_NAME   //9
   //NEWLINE
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
