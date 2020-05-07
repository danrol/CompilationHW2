#ifndef MADONNA

// yylex returns 0 when EOF is encountered
enum token {
     PLAYLIST =1, 
     SEQ_NUM,
     SONG,
     SONG_NAME,
     ARTIST,
     NAME,
     LENGTH, 
     SONG_LENGTH     
};

char *token_name(enum token token);

struct time {
         int minutes;  /* -1  means irrelevant */ 
         int seconds; 
};

// We don't really need a union here because only there is only one field.
union _lexVal{
   struct time _time;
};

extern union _lexVal lexicalValue;

void errorMsg(const char *s);

#endif
