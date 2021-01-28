#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "colors.h"
#include "misc.h"
#include "import.h"

#define indata "./indata/slate-tagged.txt"
#define parseWT "./parse/groupedWT.txt"
#define parseFW "./parse/groupedFW.txt"
#define parseFWLength "./parse/groupedFWLength.txt"
//#define indata "./indata/ficheiro.txt" // for test purpose

// reads all the lines to a binTree.
void readFromFile()
{
    BTree *treeWT=NULL, *treeFW = NULL;
    //BTree *tree =NULL;
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    char word1[100] = "\0", *word2 = NULL, *word3 = NULL;
    float probab = 0.0;
    FILE *fp = fopen(indata, "r");
    int count = 0;
    if (fp == NULL)
    {
        printf("Erro acesso a base de dados");
    }
    else
    {
        rewind(fp);       //makes sure the reading starts at the begining of the file.
        while (!feof(fp)) //runs while there are lines on the file (untill the end)
        {

            //allocates memmory to the temporary pointers
            word2 = (char *)malloc(100 * sizeof(char));
            word3 = (char *)malloc(100 * sizeof(char));

            fscanf(fp, "%s %s %s %f\n", word1, word2, word3, &probab); //it scans the file line for data feeding the list
            //Note-> As using scanf here, the lines that does not met the format specified will be ignored (empty lines)
            
            //checks if line has puntuation. If so, wont add it to the linked list.
            char y[1];
            strncpy(y, word3, 1);
            if (y[0] != 'F') //all puntuation on the 3th field has an F on the first letter.
            {
                treeWT = btreeAddGroupedWT(treeWT, word3, probab); // imports the line into a tree, grouped by word type (3rd column)
                strncpy(y, word1, 1);
                if (y[0] >= 65 && y[0] <= 90) //checks if first word has first letter as capital letter
                {
                    word1[0] += 32; // lowers case of first word
                }
                treeFW = btreeAddGroupedFW(treeFW, word1); // imports the line into a tree, grouped by first word (1st column)
            }
            //frees memmory for the temporary variables;
            free(word2);
            free(word3);
        }
    }
    fclose(fp); //closes the file

    //parses the trees back to file, but grouped
    fp=fopen(parseWT, "w");
    inOrderToFileWT(treeWT, fp);
    fclose(fp);
 
 
    fp=fopen(parseFW, "w");
    inOrderToFileFW(treeFW, fp);
    fclose(fp);

    deleteTree(treeWT);
    deleteTree(treeFW);
    parseFromFileWT();
    parseFromFileFW();  // mt lento aqui
}

// reads all the lines to a binTree.
void parseFromFileFW()
{
    BTree *treeFW = NULL, *treeFWLength=NULL;
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    char *wd=NULL;
    int wdc, wdl;
    FILE *fp = fopen(parseFW, "r");
    int count = 0;
    if (fp == NULL)
    {
        printf("Erro acesso a base de dados");
    }
    else
    {
        rewind(fp);       //makes sure the reading starts at the begining of the file.
        while (!feof(fp)) //runs while there are lines on the file (untill the end)
        {

            //allocates memmory to the temporary pointers
            wd = (char *)malloc(100 * sizeof(char));
            fscanf(fp, "%s %d %d\n", wd, &wdc, &wdl); //it scans the file line for data feeding the list
            //Note-> As using scanf here, the lines that does not met the format specified will be ignored (empty lines)
            
                treeFW = btreeSortFWByCount(treeFW, wd, wdc, wdl); // imports the line into a tree, sorted by word count
                treeFWLength=btreeSortFWByLength(treeFWLength, wd, wdc, wdl);// imports the line into a tree, sorted by word length
            free(wd);
        }
    }
    fclose(fp); //closes the file
 
    fp=fopen(parseFW, "w");
    inOrderToFileFW(treeFW, fp);
    fclose(fp);

    fp=fopen(parseFWLength, "w");
    inOrderToFileFW(treeFWLength, fp);
    fclose(fp);

    deleteTree(treeFW);
    deleteTree(treeFWLength);

}

// reads all the lines to a binTree.
void parseFromFileWT()
{
    BTree *treeWT = NULL;
    BTree *tree = (BTree *)malloc(sizeof(BTree));
    char *wd=NULL;
    int wdc=0;
    float probab =0.0;
    FILE *fp = fopen(parseWT, "r");
    int count = 0;
    if (fp == NULL)
    {
        printf("Erro acesso a base de dados");
    }
    else
    {
        rewind(fp);       //makes sure the reading starts at the begining of the file.
        while (!feof(fp)) //runs while there are lines on the file (untill the end)
        {

            //allocates memmory to the temporary pointers
            wd = (char *)malloc(10 * sizeof(char));
            fscanf(fp, "%s %d %f\n", wd, &wdc, &probab); //it scans the file line for data feeding the list
            //Note-> As using scanf here, the lines that does not met the format specified will be ignored (empty lines)
            
                treeWT = btreeSortWTByCount(treeWT, wd, wdc, probab); // imports the line into a tree, grouped by first word (1st column)
            free(wd);
        }
    }
    fclose(fp); //closes the file
 
    fp=fopen(parseWT, "w");
    inOrderToFileWT(treeWT, fp);
    fclose(fp);

    deleteTree(treeWT);

}

// procedure to group the elements by wordType
BTree *btreeAddGroupedWT(BTree *root, char *word3, float probab)
{

    if (root)
    {
        if (strcmp(word3, root->wordType) < 0)
        {
            root->left = btreeAddGroupedWT(root->left, word3, probab);
        }

        if (strcmp(word3, root->wordType) > 0)
        {
            root->right = btreeAddGroupedWT(root->right, word3, probab);
        }

        if (strcmp(word3, root->wordType) == 0)
        {
            root->probability+=probab;
            root->wordCount += 1;
            return root;
        }
    }
    else
    {
        root = (BTree *)malloc(sizeof(BTree));
        root->wordType = malloc(strlen(word3) * sizeof(char) + 1);
        strcpy(root->wordType, word3);
        strcat(root->wordType, "\0");
        root->probability = probab;
        root->wordCount = 1;
        root->left = root->right = NULL;
    }
    return root;
}

// procedure to group the elements by firstWord
BTree *btreeAddGroupedFW(BTree *root, char *word1)
{
    if (root)
    {
        if (strcmp(word1, root->firstWord) < 0)
        {
            root->left = btreeAddGroupedFW(root->left, word1);
        }

        if (strcmp(word1, root->firstWord) > 0)
        {
            root->right = btreeAddGroupedFW(root->right, word1);
        }

        if (strcmp(word1, root->firstWord) == 0)
        {
            root->wordCount += 1;
            return root;
        }
    }
    else
    {
        root = (BTree *)malloc(sizeof(BTree));
        root->firstWord = malloc(strlen(word1) * sizeof(char) + 1);
        strcpy(root->firstWord, word1);
        strcat(root->firstWord, "\0");
        root->wordLength = strlen(word1);
        root->wordCount = 1;
        root->left = root->right = NULL;
    }
    return root;
}

// procedure to sort the elements of FW by count
BTree *btreeSortFWByCount(BTree *root, char *wd, int wdc, int wdl)
{
    if (root)
    {
        if (wdc < root->wordCount)
        {
            root->left = btreeSortFWByCount(root->left, wd, wdc, wdl);
        }
        else if (wdc>root->wordCount)
        {
            root->right = btreeSortFWByCount(root->right, wd, wdc, wdl);
        }else
        {
            root->right = btreeSortFWByCount(root->right, wd, wdc, wdl);
        }
        
    }
    else
    {
        root = (BTree *)malloc(sizeof(BTree));
        root->firstWord = malloc(strlen(wd) * sizeof(char) + 1);
        strcpy(root->firstWord, wd);
        strcat(root->firstWord, "\0");
        root->wordLength = wdl;
        root->wordCount = wdc;
        root->left = root->right = NULL;
    }
    return root;
}

// procedure to sort the elements of FW by word length
BTree *btreeSortFWByLength(BTree *root, char *wd, int wdc, int wdl)
{
    if (root)
    {
        if (wdl < root->wordLength)
        {
            root->left = btreeSortFWByLength(root->left, wd, wdc, wdl);
        }else
        {
            root->right = btreeSortFWByLength(root->right, wd, wdc, wdl);
        }
    }
    else
    {
        root = (BTree *)malloc(sizeof(BTree));
        root->firstWord = malloc(strlen(wd) * sizeof(char) + 1);
        strcpy(root->firstWord, wd);
        strcat(root->firstWord, "\0");
        root->wordLength = wdl;
        root->wordCount = wdc;
        root->left = root->right = NULL;
    }
    return root;
}

// procedure to sort the elements of FW by count
BTree *btreeSortWTByCount(BTree *root, char *wd, int wdc, float probab)
{
    if (root)
    {
        if (wdc < root->wordCount)
        {
            root->left = btreeSortWTByCount(root->left, wd, wdc, probab);
        }

        if (wdc > root->wordCount)
        {
            root->right = btreeSortWTByCount(root->right, wd, wdc, probab);
        }
    }
    else
    {
        root = (BTree *)malloc(sizeof(BTree));
        root->wordType = malloc(strlen(wd) * sizeof(char) + 1);
        strcpy(root->wordType, wd);
        strcat(root->wordType, "\0");
        root->probability = probab;
        root->wordCount = wdc;
        root->left = root->right = NULL;
    }
    return root;
}

// writes a file with the grouped word type data
void inOrderToFileWT(BTree *root, FILE *fp)
{
    if (root)
    {
        inOrderToFileWT(root->left, fp);
            fprintf(fp,"%s %d %f\n", root->wordType, root->wordCount, root->probability);
        inOrderToFileWT(root->right, fp);
    }
    
}

// writes a file with the grouped first word data
void inOrderToFileFW(BTree *root, FILE *fp)
{
    if (root)
    {
        inOrderToFileFW(root->left, fp);
            fprintf(fp,"%s %d %d\n", root->firstWord, root->wordCount, root->wordLength);
        inOrderToFileFW(root->right, fp);
    }
    
}

//frees memory
void deleteTree(BTree* root)  
{ 
    if (!root) 
    {
        return; 
    }
  
    /* first delete both subtrees */
    deleteTree(root->left); 
    deleteTree(root->right); 
    
    free(root); 
}  

//imports all the records from the file into a list
t_wordType *wtListFromFile(t_wordType *headWordType)
{
    char *wd;
    int wdc=0, i=0, k=0;
    float probab=0.0;
    ListWT *sortedListWT=NULL;

    FILE *fp = fopen(parseWT, "r"); //path to the txt file
    //!checks if the file exists. If not, an error message is displayed
    if (fp == NULL)
    {
        printf("Erro acesso a base de dados");
    }
    else
    {
        rewind(fp);       //makes sure the reading starts at the begining of the file.
        while (!feof(fp)) //runs while there are lines on the file (untill the end)
        {
            wd=(char*)malloc(10*sizeof(char));
            fscanf(fp,"%s %d %f\n", wd, &wdc, &probab); //it scans the file line for data feeding the list
            //Note-> As using scanf here, the lines that does not met the format specified will be ignored (empty lines)
                i++;
                k+=wdc;
                sortedListWT = addSortedWT(headWordType, wd, wdc, probab, sortedListWT, i);

            //frees memmory for the temporary variables;
            free(wd);

        }
    }
    fclose(fp);      //closes the file
    sortedListWT->first->numElements=i;
    sortedListWT->first->totWords=k;
    headWordType=sortedListWT->first;

    return headWordType; //returns the lists pointer
}

//aux to sort the grouped list wt
ListWT *addSortedWT(t_wordType *headWordType, char*wd, int wdc, float probab, ListWT *sortedListWT, int i)
{
    t_wordType *node=(t_wordType*)malloc(sizeof(t_wordType));
    node->count = wdc;
    node->probability = probab;
    node->type=malloc(strlen(wd)*sizeof(char)+1);
    strcpy(node->type, wd);
    strcat(node->type, "\0");

    if (!sortedListWT) //if list is null, it will add as first node
    {
        sortedListWT = (ListWT *)malloc(sizeof(ListWT)); //creats an emptyList
        sortedListWT->first = node;
        sortedListWT->last = node;
        node->previous = node;
        node->next = node;
        return sortedListWT;
    }

    if (node->count < sortedListWT->first->count) //if is lesser to the last node, it will adds as first and the current first will became next to first
    {
        sortedListWT->first->previous = node;
        sortedListWT->last->next = node;
        node->previous = sortedListWT->last;
        node->next = sortedListWT->first;
        sortedListWT->first = node;
        return sortedListWT;
    }

    if (node->count >= sortedListWT->last->count) //if is greater or equal to the last node, it will adds as last and the last will became previous to last
    {
        sortedListWT->last->next = node;
        sortedListWT->first->previous = node;
        node->previous = sortedListWT->last;
        node->next = sortedListWT->first;
        sortedListWT->last = node;
        return sortedListWT;
    }

    t_wordType *new = sortedListWT->first; //it will add in the middle of the list, finding its sorted place
    for (int j = 0; j < i; j++)
    {
        if (new->count > node->count)
            break;
        new = new->next;
    }
    new->previous->next = node;
    node->previous = new->previous;
    node->next = new;
    new->previous = node;
    return sortedListWT;
}

//imports all the records from the file into a list
t_firstWord *fwListFromFile(t_firstWord *headFirstWord)
{
    char *wd;
    int wdc=0, wdl=0, i=0, k=0;

    ListFW *sortedListFW=NULL;

    FILE *fp = fopen(parseFW, "r"); //path to the txt file
    //!checks if the file exists. If not, an error message is displayed
    if (fp == NULL)
    {
        printf("Erro acesso a base de dados");
    }
    else
    {
        rewind(fp);       //makes sure the reading starts at the begining of the file.
        while (!feof(fp)) //runs while there are lines on the file (untill the end)
        {
            wd=(char*)malloc(100*sizeof(char));
            fscanf(fp,"%s %d %d\n", wd, &wdc, &wdl); //it scans the file line for data feeding the list
            //Note-> As using scanf here, the lines that does not met the format specified will be ignored (empty lines)
                i++;
                k+=wdc;
                sortedListFW = addSortedFW(headFirstWord, wd, wdc, wdl, sortedListFW, i);

            //frees memmory for the temporary variables;
            free(wd);

        }
    }
    fclose(fp);      //closes the file
    sortedListFW->first->numElements=i;
    sortedListFW->first->totWords=k;
    headFirstWord=sortedListFW->first;

    return headFirstWord; //returns the lists pointer
}

//aux to sort the grouped list wt
ListFW *addSortedFW(t_firstWord *headFirstWord, char*wd, int wdc, int wdl, ListFW *sortedListFW, int i)
{
    t_firstWord *node=(t_firstWord*)malloc(sizeof(t_firstWord));
    node->wordCount = wdc;
    node->wordLength = wdl;
    node->firstWord=malloc(strlen(wd)*sizeof(char)+1);
    strcpy(node->firstWord, wd);
    strcat(node->firstWord, "\0");

    if (!sortedListFW) //if list is null, it will add as first node
    {
        sortedListFW = (ListFW *)malloc(sizeof(ListFW)); //creats an emptyList
        sortedListFW->first = node;
        sortedListFW->last = node;
        node->previous = node;
        node->next = node;
        return sortedListFW;
    }

    if (node->wordCount < sortedListFW->first->wordCount) //if is lesser to the last node, it will adds as first and the current first will became next to first
    {
        sortedListFW->first->previous = node;
        sortedListFW->last->next = node;
        node->previous = sortedListFW->last;
        node->next = sortedListFW->first;
        sortedListFW->first = node;
        return sortedListFW;
    }

    if (node->wordCount >= sortedListFW->last->wordCount) //if is greater or equal to the last node, it will adds as last and the last will became previous to last
    {
        sortedListFW->last->next = node;
        sortedListFW->first->previous = node;
        node->previous = sortedListFW->last;
        node->next = sortedListFW->first;
        sortedListFW->last = node;
        return sortedListFW;
    }

    t_firstWord *new = sortedListFW->first; //it will add in the middle of the list, finding its sorted place
    for (int j = 0; j < i; j++)
    {
        if (new->wordCount > node->wordCount)
            break;
        new = new->next;
    }
    new->previous->next = node;
    node->previous = new->previous;
    node->next = new;
    new->previous = node;
    return sortedListFW;
}

//imports all the records from the file into a list
t_firstWord *fwListFromFileWL(t_firstWord *headFirstWordWL)
{
    char *wd;
    int wdc=0, wdl=0, i=0, k=0;

    ListFW *sortedListFW=NULL;

    FILE *fp = fopen(parseFWLength, "r"); //path to the txt file
    //!checks if the file exists. If not, an error message is displayed
    if (fp == NULL)
    {
        printf("Erro acesso a base de dados");
    }
    else
    {
        rewind(fp);       //makes sure the reading starts at the begining of the file.
        while (!feof(fp)) //runs while there are lines on the file (untill the end)
        {
            wd=(char*)malloc(100*sizeof(char));
            fscanf(fp,"%s %d %d\n", wd, &wdc, &wdl); //it scans the file line for data feeding the list
            //Note-> As using scanf here, the lines that does not met the format specified will be ignored (empty lines)
                i++;
                k+=wdc;
                sortedListFW = addSortedFWWordLength(headFirstWordWL, wd, wdc, wdl, sortedListFW, i);

            //frees memmory for the temporary variables;
            free(wd);

        }
    }
    fclose(fp);      //closes the file
    sortedListFW->first->numElements=i;
    sortedListFW->first->totWords=k;
    headFirstWordWL=sortedListFW->first;

    return headFirstWordWL; //returns the lists pointer
}


//aux to sort the grouped list wt
ListFW *addSortedFWWordLength(t_firstWord *headFirstWord, char*wd, int wdc, int wdl, ListFW *sortedListFW, int i)
{
    t_firstWord *node=(t_firstWord*)malloc(sizeof(t_firstWord));
    node->wordCount = wdc;
    node->wordLength = wdl;
    node->firstWord=malloc(strlen(wd)*sizeof(char)+1);
    strcpy(node->firstWord, wd);
    strcat(node->firstWord, "\0");

    if (!sortedListFW) //if list is null, it will add as first node
    {
        sortedListFW = (ListFW *)malloc(sizeof(ListFW)); //creats an emptyList
        sortedListFW->first = node;
        sortedListFW->last = node;
        node->previous = node;
        node->next = node;
        return sortedListFW;
    }

    if (node->wordLength < sortedListFW->first->wordLength) //if is lesser to the last node, it will adds as first and the current first will became next to first
    {
        sortedListFW->first->previous = node;
        sortedListFW->last->next = node;
        node->previous = sortedListFW->last;
        node->next = sortedListFW->first;
        sortedListFW->first = node;
        return sortedListFW;
    }

    if (node->wordLength >= sortedListFW->last->wordLength) //if is greater or equal to the last node, it will adds as last and the last will became previous to last
    {
        sortedListFW->last->next = node;
        sortedListFW->first->previous = node;
        node->previous = sortedListFW->last;
        node->next = sortedListFW->first;
        sortedListFW->last = node;
        return sortedListFW;
    }

    t_firstWord *new = sortedListFW->first; //it will add in the middle of the list, finding its sorted place
    for (int j = 0; j < i; j++)
    {
        if (new->wordLength > node->wordLength)
            break;
        new = new->next;
    }
    new->previous->next = node;
    node->previous = new->previous;
    node->next = new;
    new->previous = node;
    return sortedListFW;
}


//aux to sort the grouped list wt
ListWT *sortWTFromFlattenTree(BTree *flattenTree, ListWT *sortedListWT, int i)
{
    t_wordType *node = (t_wordType *)malloc(sizeof(t_wordType));
    node->count = flattenTree->wordCount;
    node->probability = flattenTree->probability;
    node->type=malloc(strlen(flattenTree->wordType)*sizeof(char)+1); //defines a space adjusted by the size of the flatten tree field
    strcpy(node->type,flattenTree->wordType);
    strcat(node->type, "\0");
    
    if (!sortedListWT) //if list is null, it will add as first node
    {
        sortedListWT = (ListWT *)malloc(sizeof(ListWT)); //creats an emptyList
        sortedListWT->first = node;
        sortedListWT->last = node;
        node->previous = node;
        node->next = node;
        i++;
        return sortedListWT;
    }

    if (node->count < sortedListWT->first->count) //if is lesser to the last node, it will adds as first and the current first will became next to first
    {
        sortedListWT->first->previous = node;
        sortedListWT->last->next = node;
        node->previous = sortedListWT->last;
        node->next = sortedListWT->first;
        sortedListWT->first = node;
        i++;
        return sortedListWT;
    }

    if (node->count >= sortedListWT->last->count) //if is greater or equal to the last node, it will adds as last and the last will became previous to last
    {
        sortedListWT->last->next = node;
        sortedListWT->first->previous = node;
        node->previous = sortedListWT->last;
        node->next = sortedListWT->first;
        sortedListWT->last = node;
        i++;
        return sortedListWT;
    }

    t_wordType *new = sortedListWT->first; //it will add in the middle of the list, finding its sorted place
    for (int j = 0; j < i; j++)
    {
        if (new->count > node->count)
            break;
        new = new->next;
    }
    new->previous->next = node;
    node->previous = new->previous;
    node->next = new;
    new->previous = node;
    i++;
    return sortedListWT;
}

//aux to sort the grouped list fw
ListFW *sortFWFromFlattenTree(BTree *flattenTree, ListFW *sortedListFW, int i)
{
    t_firstWord *node = (t_firstWord *)malloc(sizeof(t_firstWord));
    node->wordCount = flattenTree->wordCount;
    node->wordLength = flattenTree->wordLength;
    node->firstWord = flattenTree->firstWord;

    if (!sortedListFW) //if list is null, it will add as first node
    {
        sortedListFW = (ListFW *)malloc(sizeof(ListFW)); //creats an emptyList
        sortedListFW->first = node;
        sortedListFW->last = node;
        node->previous = node;
        node->next = node;
        i++;
        return sortedListFW;
    }
    if (node->wordLength < sortedListFW->first->wordLength) //if is lesser to the last node, it will adds as first and the current first will became next to first
    {
        sortedListFW->first->previous = node;
        sortedListFW->last->next = node;
        node->previous = sortedListFW->last;
        node->next = sortedListFW->first;
        sortedListFW->first = node;
        i++;
        return sortedListFW;
    }

    if (node->wordLength >= sortedListFW->last->wordLength) //if is greater or equal to the last node, it will adds as last and the last will became previous to last
    {
        sortedListFW->last->next = node;
        sortedListFW->first->previous = node;
        node->previous = sortedListFW->last;
        node->next = sortedListFW->first;
        sortedListFW->last = node;
        i++;
        return sortedListFW;
    }

    t_firstWord *new = sortedListFW->first; //it will add in the middle of the list, finding its sorted place
    for (int j = 0; j < i; j++)
    {
        if (new->wordLength > node->wordLength)
            break;
        new = new->next;
    }
    new->previous->next = node;
    node->previous = new->previous;
    node->next = new;
    new->previous = node;
    return sortedListFW;
}