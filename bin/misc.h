#ifndef _MISC_H_
#define _MISC_H_

#include "colors.h"

/*
*
* Library with the miscellaneous of the program. 
* Includes menus, and non statistic functions
* Also includes the structure for stored data
*
*/

//Structure for list related to first word
typedef struct firstWordRecord
{
    int numElements;
    int totWords;
    int quartile;
    char *firstWord;
    int wordLength;
    int wordCount;
    struct firstWordRecord *next;
    struct firstWordRecord *previous;
} t_firstWord;

//Structure for list related to word type
typedef struct typeRecord
{
    int numElements;
    int totWords;
    char *type;
    int count;
    float probability;
    struct typeRecord *next;
    struct typeRecord *previous;
} t_wordType;

//struct used for temp sorting on circular linked lists related to word type
typedef struct
{
    t_wordType *first;
    t_wordType *last;
} ListWT;

//struct used for temp sorting on circular linked lists related to first word
typedef struct
{
    t_firstWord *first;
    t_firstWord *last;
} ListFW;

//tree structure
typedef struct _BTree
{
    int numElements;
    char *firstWord;
    int wordLength;
    char *wordType;
    int wordCount;
    float probability;
    struct _BTree *left, *right;
    struct _BTree *treeWT, *treeFW;
} BTree;

// holds the code until the number of seconds passes
void delay(int seconds);

// holds the screen, and makes a small "animation" before moving on
void waitToRedirect(int i);

//Wrong choice error display
void wrongOptionError();

//prototype of mainMenu procedure
int mainMenu(int num);

// Prints a header related to the option choosed by the user
void optionHeader(char *string);

//Yellow line to dashboards (header and footer)
void headerLine(int n);

//Yellow line to dashboards (header and footer)
void baseLine(int n);

//! Back to menu
void backToMenu();

//alternative to fflush
void clean_stdin(void);

//prints a message while loading data
void loadingHeader();

//Full screen mode
void fullscreen();

#endif