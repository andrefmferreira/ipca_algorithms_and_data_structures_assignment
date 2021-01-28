#ifndef _IMPORT_H_
#define _IMPORT_H_

#include "misc.h"


// reads all the lines to a binTree.
void readFromFile();

// procedure to group the elements by wordType
BTree *btreeAddGroupedWT(BTree *root, char *word3, float probab);

// procedure to group the elements by firstWord
BTree *btreeAddGroupedFW(BTree *root, char *word1);

// writes a file with the grouped word type data
void inOrderToFileWT(BTree *root, FILE *fp);

// writes a file with the grouped first word data
void inOrderToFileFW(BTree *root, FILE *fp);

//frees memory deleting the tree no longer used
void deleteTree(BTree* root);

//sorts the wordType list
t_wordType *sortedGroupListWT(t_wordType *headWordType, char*wd, int wdc, float probab);

//aux to sort the grouped list wt
ListWT *addSortedWT(t_wordType *headWordType, char*wd, int wdc, float probab, ListWT *sortedListWT, int i);

//aux to sort the grouped list wt
ListFW *addSortedFW(t_firstWord *headFirstWord, char*wd, int wdc, int wdl, ListFW *sortedListFW, int i);

//aux to sort the grouped list wt
ListFW *addSortedFWWordLength(t_firstWord *headFirstWord, char*wd, int wdc, int wdl, ListFW *sortedListFW, int i);

//imports all the records from the file into a list
t_wordType *wtListFromFile(t_wordType *headWordType);

//imports all the records from the file into a list
t_firstWord *fwListFromFile(t_firstWord *headFirstWord);

//imports all the records from the file into a list
t_firstWord *fwListFromFileWL(t_firstWord *headFirstWordWL);

// reads all the lines from temp file to a binTree.
void parseFromFileFW();

// reads all the lines from temp file to a binTree.
void parseFromFileWT();

// procedure to sort the elements of FW by count
BTree *btreeSortFWByCount(BTree *root, char *wd, int wdc, int wdl);

// procedure to sort the elements of FW by word length
BTree *btreeSortFWByLength(BTree *root, char *wd, int wdc, int wdl);

// procedure to sort the elements of FW by count
BTree *btreeSortWTByCount(BTree *root, char *wd, int wdc, float probab);

//aux to sort the grouped list wt
ListWT *sortWTFromFlattenTree(BTree *flattenTree, ListWT *sortedListWT, int i);

//aux to sort the grouped list fw
ListFW *sortFWFromFlattenTree(BTree *flattenTree, ListFW *sortedListFW, int i);

#endif