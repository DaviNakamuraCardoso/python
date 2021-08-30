#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <types.h>
#include <defrag.h>


TOKEN* new_wordt(DICTIONARY d, char* word)
{    
    TOKEN* t = malloc(sizeof(TOKEN));
    WORD* w = search_word(d, word);
    if (w == NULL || w->class == INEXISTENT) 
    {
        t->type = __UNKNOWN; 
        t->content = strdup(word);
        return t; 
       
    }

    t->type = __WORD;
    t->word = (WORD*) w; 

    return t; 
}

TOKEN* new_numeralt(DICTIONARY d, char* word)
{
    TOKEN* t = malloc(sizeof(TOKEN));
    t->type = __NUMERAL;
    t->number = atof(word);

    return t; 
}

TOKEN* new_symbolt(DICTIONARY d, char* word)
{
    TOKEN* t = malloc(sizeof(TOKEN));
    t->type = __SYMBOL;
    t->symbol = d.symbols[symboltype(*word)]; 
    return t; 
}

TOKEN* new_chaptert(DICTIONARY d, char* word)
{
    TOKEN* t = malloc(sizeof(TOKEN));
    t->type = __CHAPTER;
    t->content = strdup(word);
    return t; 
}


TOKEN* new_unknownt(DICTIONARY d, char* word)
{
    TOKEN* t = malloc(sizeof (TOKEN)); 
    t->type = __UNKNOWN; 
    t->content = strdup(word); 

    return t; 
}


static unsigned int isnewline(char c)
{
    return c == '\n'; 
}


int getword(char* phrase, int index, char* buff)
{
    int i = index;
    int j; 
    for (j = 0; isalnum(phrase[i]) && phrase[i] != '\0'; i++, j++)
    {
        buff[j] = phrase[i]; 
    }
    buff[j] = '\0'; 

    for (; isblank(phrase[i]); i++) {}

    if (i == index)
    {
        if (issym(phrase[i])) { buff[0] = phrase[i++]; buff[1] = '\0'; }
        for (; !isalnum(phrase[i]) && 
                !issym(phrase[i]) && phrase[i] != '\0'; i++){}
    }
    return i-1;

}


List* tokenize(DICTIONARY dictionary, FILE* stream)
{
    List* l = new_list(); 
    char buffer[400];
    unsigned long counter = 1; 

    inline enum tokentype wordtype(char* word)
    {
        if (isnumeral(word)) return __NUMERAL;
        if (issym(*word)) return __SYMBOL;
        if (ischapter(word)) return __CHAPTER;

        return __WORD; 
    }
        
    inline int generatetoken(char* word)
    {
        TOKEN* (*constructors[]) (DICTIONARY, char*) = {
            new_wordt, 
            new_symbolt, 
            new_numeralt, 
            new_chaptert,
            new_unknownt
        };

        addt(dictionary, l, constructors[wordtype(word)](dictionary, word)); 

        return 0;
    }

    inline int searchwords(char* buff)
    {
        for (int i = 0; buff[i] != '\0'; i++)
        {
            char word[200]; 
            i = getword(buff, i, word);
            generatetoken(word);     
        }
        return 0; 
    }

    while (get_line(stream, buffer) != NULL)
    {
        searchwords(buffer); 
        buffer[0] = '\0';
    }
    return l;

}

// Get the next line 
char* get_line(FILE* f, char* buff)
{
    int i = 0; 
    char c; 
    for (; (c = fgetc(f)) != EOF; i++)
    {
        if (isnewline(c))
        {
            buff[i] = '\0'; 
            return buff; 
        }
        buff[i] = c; 
    }

    buff[i] = '\0'; 
    return NULL; 
} 




