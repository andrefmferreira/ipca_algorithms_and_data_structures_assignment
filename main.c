#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./bin/misc.h"
#include "./bin/colors.h"
#include "./bin/statisticFun.h"
#include "./bin/import.h"

/*
 * Date: 26-04-2020
 * Authors: André Ferreira (a18865@alunos.ipca.pt) & José Pereira (a18867@alunos.ipca.pt)
 * Description: Code to solve a AED2 and Statistcs UC's practical task. The project has the goal of importing a text file with strings,
 *              and make statistical analysis about the words morfologic syntax and other metrics.
 * Version: 0.1
 *
*/

void main()
{
    fullscreen(); //goes full screen
    loadingHeader(); //displays an intro.

    int choice, exitConf = 0, count = 0, i = 0, k = 0;
    float averageWordLength;
    BTree *tree, *treeFW, *treeWT = NULL;
    t_firstWord *headFirstWord, *stopFW = NULL;
    t_wordType *headWordType, *stopWT = NULL;
    t_firstWord *headFirstWordWL, *stopFWWL = NULL;

    //imports the file content into bin tree and then stores the worked (grouped information) on 2 files
    readFromFile();

    headWordType = wtListFromFile(headWordType);
    headFirstWord = fwListFromFile(headFirstWord);
    headFirstWordWL = fwListFromFileWL(headFirstWordWL);

    //defines a stopPoint for the circular lists
    stopWT = headWordType;
    stopFW = headFirstWord;
    stopFWWL = headFirstWordWL;

    do // loops the menu, until user gives order to leave
    {
        switch (mainMenu(choice))
        {
        case 1:
            optionHeader("Tabela de frequencias por categoria gramatical");
            freqAbsTableWT(headWordType, stopWT);
            backToMenu(); //prompts the user to press any key in order to go back to the main menu
            break;
        case 2:
            optionHeader("Tabela de frequencias por tamanho das palavras");
            freqAbsTableFW(headFirstWordWL, stopFWWL);
            backToMenu(); //prompts the user to press any key in order to go back to the main menu
            break;
        case 3:
            optionHeader("Analise morfologica - Media aritmetica e desvio padrao");
            morphAnalysisWT(headWordType, stopWT);
            backToMenu(); //prompts the user to press any key in order to go back to the main menu
            break;
        case 4:
            count = headFirstWord->numElements; // returns the number of different words stored on the list
            optionHeader("Medidas de dispersao e de localizacao");

            printf("\n\n");
            //175 ascci table for ">>"
            printf(YELLOW "\t%c Palavras diferentes lidas:" CYAN " %ld\n" RESET, 175, count);
            averageWordLength = avgWordLength(headFirstWord, stopFW);
            printf(YELLOW "\t%c Tamanho medio das palavras:" CYAN " %.3lf\n" RESET, 175, averageWordLength);
            printf(YELLOW "\t%c Moda das palavras lidas:" CYAN " \"%d\"\n" RESET, 175, modeWordLenght(headFirstWord, stopFW));
            printf(YELLOW "\t%c Desvio padrao:" CYAN " %.3f\n" RESET, 175, stdDeviation(headFirstWord, stopFW, averageWordLength));
            printf(YELLOW "\t%c Mediana do tamanho das palavras: " CYAN "\"%d\"\n" RESET, 175, medianFW(headFirstWord, stopFW));

            backToMenu(); //prompts the user to press any key in order to go back to the main menu
            break;
        case 5:
            optionHeader("Pesquisar palavras e identificar quartil");
            quartiles(headFirstWord, stopFW, headFirstWordWL, stopFWWL);
            backToMenu(); //prompts the user to press any key in order to go back to the main menu
            break;
        case 6:
            optionHeader("Histograma das probabilidades");
            histogramProbab(headWordType, stopWT);
            //histogram(headWordType, stopWT);
            backToMenu(); //prompts the user to press any key in order to go back to the main menu
            break;
        case 7:
            do
            {
                system("cls");
                printf(RED "<<<< Tem a certeza que deseja sair do programa? >>>>\n" RESET);
                printf(BLUE "   1." CYAN " Confirmar\n" BLUE "   2." CYAN " Cancelar\n\n" RESET);
                printf(YELLOW "\t>> " RESET);
                scanf(" %d", &exitConf);

                if (exitConf != 1 & exitConf != 2)
                {
                    wrongOptionError(); //sets a message if user inputs a wrong choice
                }
                else if (exitConf == 1)
                {
                    int i = 3; //Creats an count down animation set for 3 seconds. After it exits the program
                    while (i > 0)
                    {
                        system("cls");
                        printf(BLUE "Obrigado por ter usado este programa\n\n" RESET);
                        printf(RED "\nO programa vai encerrar automaticamente em %d segundos\n\n\n\n" RESET, i);
                        delay(1);
                        i--;
                    }
                    choice = 8;
                }
                else if (exitConf == 2) //user cancels exit the program action.
                {
                    break; //Exits the loop and backs to main menu
                }
            } while (exitConf != 1 & exitConf != 2);
            break;
        default:
            break;
        }
    } while (exitConf != 1);
}
