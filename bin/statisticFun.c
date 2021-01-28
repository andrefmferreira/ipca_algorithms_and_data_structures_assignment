#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "statisticFun.h"
#include "misc.h"

/* Code responsible to deal with:
*   mean calculations
*   median calculations
*   mode calculations
*   standard deviation calculations
*   chart print
*   search words on database and quartile calculation
*/

//Function to calculate the average of read words length from file
float avgWordLength(t_firstWord *headFirstWord, t_firstWord *stopFW)
{
    int totWords = headFirstWord->totWords;
    int length = 0;
    do
    {
        length += headFirstWord->wordLength * headFirstWord->wordCount; //sumproduct of the length
        headFirstWord = headFirstWord->next;

    } while (headFirstWord != stopFW);
    return (length / (float)totWords);
}

//calculates the mode of the words recorded
int modeWordLenght(t_firstWord *headFirstWord, t_firstWord *stopFW)
{
    //mode is given by the word length with most ocurrencies.

    int max = 0, x = 128, i = 0, maxLenIndex = 0;
    int len[128] = {0}; //stores by word length the number of words.

    //stores the count of chars matching the index of the array +1
    do
    {
        for (i = 0; i < x; i++)
        {
            if (headFirstWord->wordLength == (i + 1))
            {
                len[i] += headFirstWord->wordCount;
                i = x;
            }
        }
        headFirstWord = headFirstWord->next;
    } while (headFirstWord != stopFW);

    //finds the max
    for (i = 0; i < x; i++)
    {
        if (max < len[i])
        {
            max = len[i];
            maxLenIndex = i + 1;
        }
    }
    return maxLenIndex; //returns the length with most ocurrencies
}

//calculates the standard deviation
float stdDeviation(t_firstWord *headFirstWord, t_firstWord *stopFW, float avg)
{
    /*
    *   stddev=sqrt(((val-avg)^2)/count)
    */
    int count = headFirstWord->totWords;
    float sum = 0.0;
    do
    {
        sum += pow((headFirstWord->wordLength) - avg, 2);
        headFirstWord = headFirstWord->next;
    } while (headFirstWord != stopFW);
    return sqrt(sum / headFirstWord->numElements);
}

//Function to print an absolutes frequency table related to the 3rd field (wordType)
void freqAbsTableWT(t_wordType *headWordType, t_wordType *stopWT)
{
    int cumNi = 0;
    int total = headWordType->totWords; //number of types of words

    printf("\n\n");
    printf(YELLOW "\t------------------------------------------------------------------------------------------\n" RESET);
    printf(YELLOW "\t| Categoria Gramatical   | ni          | fi          | Cum ni          | Cum fi          |\n" RESET);
    printf(YELLOW "\t-------------------------+-------------+-------------+-----------------+------------------\n" RESET);

    do
    {
        cumNi += headWordType->count;
        printf(YELLOW "\t| " CYAN "%-22s" YELLOW " |   " CYAN " % 8d" YELLOW " |   " CYAN " % 8.5f" YELLOW " |   " CYAN " % 12d" YELLOW " |       " CYAN " % 8.5f" YELLOW " |\n" RESET, headWordType->type, headWordType->count, headWordType->count / (float)total, cumNi, (cumNi / (float)total));
        printf(YELLOW "\t-------------------------+-------------+-------------+-----------------+------------------\n" RESET);
        headWordType = headWordType->next;
    } while (headWordType != stopWT);

    printf(YELLOW "\t| TOTAL                  |   " CYAN " % 8d" YELLOW " |   " CYAN " % 8d" YELLOW " |                 |                 |\n" RESET, total, 1);
    printf(YELLOW "\t-------------------------+-------------+-------------+-----------------+------------------\n" RESET);
}

//Function to print an absolutes frequency table related to the 1st field (firstWord)
void freqAbsTableFW(t_firstWord *headFirstWord, t_firstWord *stopFW)
{
    int min, max, class, n, x, cumNi;
    float classTemp;
    int c[20] = {0};
    int pos;
    int nElem = headFirstWord->numElements; // Number of different types of words
    int total = headFirstWord->totWords;    //number of types of words

    //Sturges formula - It was made a maximun of 20
    n = (1 + log2(nElem) > 20) ? 20 : 1 + log2(nElem);

    //min and max defined by the sorted list
    min = headFirstWord->wordLength;
    max = headFirstWord->previous->wordLength;

    //defines the class. if even, devide by number of words, if odd, adds 1, and then devide by number of words
    if ((max - min) % 2 == 0)
    {
        classTemp = round((max - min) / (float)n);
    }
    else
    {
        classTemp = round(((max - min) + 1) / (float)n);
    }
    cumNi = 0;
    class = classTemp;
    do
    {
        x = 0;
        pos = 0;
        while ((min + x * class) < (max + class))
        {
            if (headFirstWord->wordLength >= (min + (x * class)) && headFirstWord->wordLength < (min + (x + 1) * class))
            {
                c[pos] += headFirstWord->wordCount; //increments to the array accordingly to the number of grouped words
                x = 20;                             //stop criteria
            }
            x++;
            pos++;
        }
        headFirstWord = headFirstWord->next;
    } while (headFirstWord != stopFW);

    printf("\n\n");
    printf(YELLOW "\t------------------------------------------------------------------------------------\n" RESET);
    printf(YELLOW "\t| classe           | ni          | fi          | Cum ni          | Cum fi          |\n" RESET);
    printf(YELLOW "\t-------------------+-------------+-------------+-----------------+------------------\n" RESET);

    int i = 0;
    while (min < (max + +1))
    {
        cumNi += c[i];
        printf(YELLOW "\t| " CYAN "[ % 4d , % 4d[" YELLOW "   |   " CYAN " % 8d" YELLOW " |   " CYAN " % 8.5f" YELLOW " |   " CYAN " % 12d" YELLOW " |       " CYAN " % 8.5f" YELLOW " |\n" RESET, min, (min + class), c[i], c[i] / (float)total, cumNi, (cumNi / (float)total));
        printf(YELLOW "\t-------------------+-------------+-------------+-----------------+------------------\n" RESET);
        min += class;
        i++;
    }
    printf(YELLOW "\t| TOTAL            |   " CYAN " % 8d" YELLOW " |   " CYAN " % 8d" YELLOW " |                 |                 |\n" RESET, total, 1);
    printf(YELLOW "\t------------------------------------------------------------------------------------\n" RESET);
}

//calculates the average and standard deviation per 3rd field vs probability
void morphAnalysisWT(t_wordType *headWordType, t_wordType *stopWT)
{
    int total = headWordType->totWords, count = 0;
    float probab = 0.0, avg = 0.0, soma = 0.0;

    do //runs the list to gather information
    {
        soma += headWordType->probability;
        count += headWordType->count;
        headWordType = headWordType->next;
    } while (headWordType != stopWT);

    avg = soma / (float)count;
    puts("");
    printf(YELLOW "\t--------------------------------------------------------------------------------\n" RESET);
    printf(YELLOW "\t| Analise Morfologica           | Media aritmetica      | Desvio Padrao        |\n" RESET);
    printf(YELLOW "\t--------------------------------------------------------------------------------\n" RESET);

    do
    {
        probab = headWordType->probability / headWordType->count;

        printf(YELLOW "\t|" CYAN " % 20s " YELLOW "         | " CYAN " % 14.5f " YELLOW "      | " CYAN " % 14.5f " YELLOW "     |\n" RESET, headWordType->type, probab, sqrt((headWordType->count / (float)total) * pow((probab - avg), 2)));
        printf(YELLOW "\t--------------------------------------------------------------------------------\n" RESET);

        headWordType = headWordType->next;
    } while (headWordType != stopWT);
}

//calculates the median for wordType
int medianFW(t_firstWord *headFirstWord, t_firstWord *stopFW)
{
    int nElem = headFirstWord->numElements, medianIndex = 0, median;

    //if nElem odd then index = (numElements+1)/2-1
    //if nelem even then index = numElements/2
    if (nElem % 2 != 0) //checks if number of elements are odd or even
    {
        medianIndex = ((nElem + 1) / 2);
    }
    else
    {
        medianIndex = (nElem / 2);
    }

    for (int i = 0; i < medianIndex + 1; i += headFirstWord->wordCount)
    {
        if (i <= medianIndex && medianIndex < i + headFirstWord->wordCount)
        {
            median = headFirstWord->wordLength;
            break;
        }
        headFirstWord = headFirstWord->next;
    }
    return median;
}

//calculates the quartiles attending to word count
void quartiles(t_firstWord *headFirstWord, t_firstWord *stopFW, t_firstWord *headFirstWordWL, t_firstWord *stopFWWL)
{
    int total = headFirstWord->previous->wordCount; //maximum number of words
    int q1 = 0, q2 = 0, q3 = 0, qt1 = 0;
    char word[100], y[1];
    int found = 0; //0 not found, 1 found

    //define 1st quartile
    if (total % 2 != 0) //checks if number of elements are odd or even
    {
        q1 = ((total + 1) / 4);
    }
    else
    {
        q1 = (total / 4);
    }

    q2 = 2 * q1 + 1; //defines the 2 quartile
    q3 = 3 * q1 + 1; //defines the 3 quartile

    printf(YELLOW "\n\tIndique a palavra que deseja pesquisar:\n" RESET);
    printf(BLUE "\t\t%c%c " RESET, 175, 175);
    clean_stdin();
    scanf("%s", word);

    strncpy(y, word, 1);
    if (y[0] >= 65 && y[0] <= 90) //checks if first word has first letter as capital letter
    {
        word[0] += 32; // lowers case of first word
    }

    //Defines the quartiles for each word on the list
    do
    {
        if (headFirstWord->wordCount >= 1 && headFirstWord->wordCount <= q1)
        {
            headFirstWord->quartile = 1;
        }
        else if (headFirstWord->wordCount >= (q1 + 1) && headFirstWord->wordCount <= q2)
        {
            headFirstWord->quartile = 2;
        }
        else if (headFirstWord->wordCount >= (q2 + 1) && headFirstWord->wordCount <= q3)
        {
            headFirstWord->quartile = 3;
        }
        else
        {
            headFirstWord->quartile = 4;
        }

        headFirstWord = headFirstWord->next;
    } while (stopFW != headFirstWord);

    //searches the list for the provided word
    do
    {
        if (strcmp(word, headFirstWord->firstWord) == 0)
        {
            found = 1;
            break;
        }

        headFirstWord = headFirstWord->next;
    } while (stopFW != headFirstWord);

    printf(YELLOW "\n\tQuartis por numero de ocurrencias:\n" RESET);
    printf(BLUE "\t\t%c " YELLOW "Q1 - 1 a %d\n", 175, q1);
    printf(BLUE "\t\t%c " YELLOW "Q2 - %d a %d\n", 175, q1 + 1, q2);
    printf(BLUE "\t\t%c " YELLOW "Q3 - %d a %d\n", 175, q2 + 1, q3);
    printf(BLUE "\t\t%c " YELLOW "Q4 - %d a %d\n", 175, q3 + 1, total);

    if (found == 1)
    {
        printf(BLUE "\n\t\t\t%c%c%c" YELLOW " Palavra encontrada no quartil: " CYAN "%d " YELLOW " com " CYAN "%d " YELLOW "ocurrencias\n" RESET, 175, 175, 175, headFirstWord->quartile, headFirstWord->wordCount);
    }
    else
    {
        printf(RED "\n\t\t%c Palavra nao encontrada!\n" RESET), 175;
    }

    quartilesByLength(headFirstWordWL, stopFWWL, word); //calls another procedure to calculate the quartiles by word length
}

//calculates the quartiles attending to word length
void quartilesByLength(t_firstWord *headFirstWordWL, t_firstWord *stopFWWL, char *word)
{
    int total = headFirstWordWL->previous->wordLength; //highest word length
    int q1 = 0, q2 = 0, q3 = 0, qt1 = 0;
    char y[1];
    int found = 0; //0 not found, 1 found

    //define 1 quartile
    if (total % 2 != 0) //checks if number of elements are odd or even
    {
        q1 = ((total + 1) / 4);
    }
    else
    {
        q1 = (total / 4);
    }

    q2 = 2 * q1 + 1; //defines the 2 quartile
    q3 = 3 * q1 + 1; //defines the 3 quartile

    //Defines the quartiles for each word on the list
    do
    {
        if (headFirstWordWL->wordLength >= 1 && headFirstWordWL->wordLength <= q1)
        {
            headFirstWordWL->quartile = 1;
        }
        else if (headFirstWordWL->wordLength >= (q1 + 1) && headFirstWordWL->wordLength <= q2)
        {
            headFirstWordWL->quartile = 2;
        }
        else if (headFirstWordWL->wordLength >= (q2 + 1) && headFirstWordWL->wordLength <= q3)
        {
            headFirstWordWL->quartile = 3;
        }
        else
        {
            headFirstWordWL->quartile = 4;
        }

        headFirstWordWL = headFirstWordWL->next;
    } while (stopFWWL != headFirstWordWL);

    //searches the list for the provided word
    do
    {
        if (strcmp(word, headFirstWordWL->firstWord) == 0)
        {
            found = 1;
            break;
        }

        headFirstWordWL = headFirstWordWL->next;
    } while (stopFWWL != headFirstWordWL);

    printf(YELLOW "\n\tQuartis por tamanho de palavra:\n" RESET);
    printf(BLUE "\t\t%c " YELLOW "Q1 - 1 a %d\n", 175, q1);
    printf(BLUE "\t\t%c " YELLOW "Q2 - %d a %d\n", 175, q1 + 1, q2);
    printf(BLUE "\t\t%c " YELLOW "Q3 - %d a %d\n", 175, q2 + 1, q3);
    printf(BLUE "\t\t%c " YELLOW "Q4 - %d a %d\n", 175, q3 + 1, total);

    if (found == 1)
    {
        printf(BLUE "\n\t\t\t%c%c%c" YELLOW " Palavra encontrada no quartil: " CYAN "%d " YELLOW "com " CYAN " %d " YELLOW "caracteres\n" RESET, 175, 175, 175, headFirstWordWL->quartile, headFirstWordWL->wordLength);
    }
    else
    {
        printf(RED "\n\t\t%c Palavra nao encontrada!\n" RESET, 175);
    }
}

//prints a hist ogram on the console
void histogramProbab(t_wordType *headWordType, t_wordType *stopWT)
{

    int countByProbab[5] = {0}, histogramPrint[5] = {0}, i = 0, j = 0, max = 0, min = 0, printMark = 0, aux = 0;
    float x, minPerc = 1.0, maxPerc = 0.0, y;
    int tot = headWordType->totWords;
    int resolution=0.0;
    max = headWordType->previous->count;
    min = headWordType->count;

    resolution=log2(max)*2.3; //resolution of the chart. it is adapted to the size of the population.


    x = 0.2; //defined "class"
    y = x;   // aux variable

    //saves the number of words with the given "class of probability"
    do
    {
        i = 4;
        x = 0;
        while (i >= 0)
        {
            if ((headWordType->probability / headWordType->count) > x && (headWordType->probability / headWordType->count) <= (x + y))
            {
                countByProbab[i] += headWordType->count;
                i = 0; //exit condition
            }
            x += y;
            i--;
        }
        headWordType = headWordType->next;
    } while (headWordType != stopWT);

    //saves the number of "elements" for populating the histogram chart. It was defined a maximun of 10
    i = 0;
    float k;
    while (i < 5)
    {
        k = (countByProbab[i] / (float)max);

        if (k > 1)
        {
            histogramPrint[i] = resolution/3.3;
            
        }
        else if (k < 0.1)
        {
            histogramPrint[i] = k * (resolution);
        }
        else
        {
            histogramPrint[i] = k * (resolution/5);
        }

        i++;
    }

    printf("\n\n\n");
    printf(YELLOW "\tHistograma com base na certeza da medida de etiquetacao\n\n" RESET);
    //prints the chart like a printer
    // row
    for (j = 16; j > 0; j--)
    {
        printf(YELLOW "\t|  " RESET, j);
        // column
        for (i = 4; i >= 0; i--)
        {
            if (j != histogramPrint[i])
            {
                if ((j - 1) == histogramPrint[i])
                {
                    printf(BLUE " % 7d  " RESET, countByProbab[i]);
                }
                else
                {
                    printf("          ");
                }
            }
            else
            {
                printf(CYAN "%c%c%c%c%c%c%c%c%c%c", 178, 178, 178, 178, 178, 178, 178, 178, 178, 178);
                histogramPrint[i]--;
            }
        }

        printf("\n");
    }
    printf(YELLOW "\t|-----------+---------+---------+---------+---------+\n" RESET);

    printf(YELLOW "\t|" RESET);
    x = 0;
    for (i = 0; i <= 5; i++)
    {
        printf(BLUE "%.1f       " RESET, x);
        x += y;
    }
    printf("\n");
}
