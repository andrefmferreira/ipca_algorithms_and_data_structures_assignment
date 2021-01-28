#ifndef _STATISTICFUN_
#define _STATISTICFUN_

#include "misc.h"
/*
*
* Library with the statistic functions for this practical task including auxiliar functions to calculate this functions. 
* Includes functions to calculate
*       > average
*       > count
*       > mode
*       > median
*       > mode
*       > histogram
*/

//Function to calculate the average of read words length from file
float avgWordLength(t_firstWord *headFirstWord, t_firstWord *stopFW);

//calculates the mode of the words recorded
int modeWordLenght(t_firstWord *headFirstWord, t_firstWord *stopFW);

//calculates the standard deviation
float stdDeviation(t_firstWord *headFirstWord, t_firstWord *stopFW, float avg);

//Function to print an absolutes frequency table related to the 3rd field (wordType)
void freqAbsTableWT(t_wordType *headWordType,t_wordType *stopWT);

//Function to print an absolutes frequency table related to the 1st field (firstWord)
void freqAbsTableFW(t_firstWord *headFirstWord, t_firstWord *stopFW);

//calculates the average and standard deviation per 3rd field vs probability
void morphAnalysisWT(t_wordType *headWordType, t_wordType *stopWT);

//calculates the median for wordType
int medianFW(t_firstWord *headFirstWord, t_firstWord *stopWF);

//calculates the quartiles attending to word count
void quartiles(t_firstWord *headFirstWord, t_firstWord *stopFW, t_firstWord *headFirstWordWL, t_firstWord *stopFWWL);

//calculates the quartiles attending to word length
void quartilesByLength(t_firstWord *headFirstWordWL, t_firstWord *stopFWWL, char *word);

//prints a histogram on the console
void histogramProbab(t_wordType *headWordType, t_wordType *stopWT);

#endif