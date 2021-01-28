#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <windows.h>
#include "colors.h"
#include "misc.h"
#include "import.h"

/*
 * This code is resposible to handle miscellaneous things, such as:
 *      > animations
 *      > menus
 *      > headers
*/

//! holds the code until the number of seconds passes
void delay(int seconds)
{
    // time in milliseconds
    int milliSeconds;
    milliSeconds = seconds * 1000;
    //defines a time start point so we can use clock's function to create delays
    clock_t startTime = clock();
    while (clock() < startTime + milliSeconds)
    {
    }
}

//! holds the screen, and makes a small "animation" before moving on
void waitToRedirect(int i)
{
    while (i > 0)
    {
        printf(YELLOW ".");
        delay(1);
        i--;
    }
    printf("" RESET);
}

//!Wrong choice error display
void wrongOptionError()
{
    printf(RED "\tOpcao invalida. Por favor tente novamente ");
    waitToRedirect(3);
}
//!procedure for main menu
int mainMenu(int num)
{
    int numChoices = 7;
    do
    {
        system("cls");
        puts(CYAN "Por favor escolha uma opcao:\n" RESET);
        puts(CYAN "\t1." YELLOW " Tabela de frequencias por categoria gramatical");
        puts(CYAN "\t2." YELLOW " Tabela de frequencias por tamanho das palavras");
        puts(CYAN "\t3." YELLOW " Analise morfologica - Media aritmetica e desvio padrao");
        puts(CYAN "\t4." YELLOW " Medidas de dispersao e de localizacao");
        puts(CYAN "\t5." YELLOW " Pesquisar palavras e identificar quartis");
        puts(CYAN "\t6." YELLOW " Histograma das probabilidades");
        puts(CYAN "\t7." YELLOW " Sair\n\n");
        printf(YELLOW "\t>> " RESET);
        scanf(" %d", &num);

        if (num < 0 || num > numChoices)
        {
            wrongOptionError(); //sets a message if user inputs a wrong choice
        }

    } while (num < 0 || num > numChoices);

    return num;
}

// Prints a header related to the option choosed by the user
void optionHeader(char *string)
{
    system("cls");
    puts("");
    headerLine(strlen(string) + 6); //prints a line with a defined color
    printf(YELLOW "\t%c%c"
                  " %s "
                  "%c%c\n" RESET,
           219, 219, string, 219, 219);
    headerLine(strlen(string) + 6); //prints a line with a defined color
}
//Yellow line to dashboards (header and footer)
void headerLine(int n)
{
    int j;
    printf("\t");
    j = 0;
    while (j < n)
    {
        printf(YELLOW "%c" RESET, 219);
        j++;
    }
    puts("");
}

//Yellow line to dashboards (header and footer)
void baseLine(int n)
{
    int j;
    printf("\t");
    j = 0;
    while (j < n)
    {
        printf(YELLOW "%c" RESET, 223);
        j++;
    }
    puts("");
}

//! Back to menu
void backToMenu()
{
    printf("\n");
    baseLine(100);
    printf(YELLOW "\tPrima qualquer tecla para voltar ao menu" RESET);
    clean_stdin();
    getchar();
}

//alternative to fflush
void clean_stdin(void)
{
    int c;
    do
    {
        c = getchar();
    } while (c != '\n' && c != EOF);
}

//prints a message while loading data
void loadingHeader()
{
    system("cls");
    //defines a time start displaying the intro
    clock_t startTime = clock();
    while (clock() < startTime + 500); // creates a delay half a second
    {
    }

    printf(YELLOW"\n\n\t    *********************************************************************************************  \n");
    printf("\t   *********************************************************************************************** \n");
    printf("\t  *************************************************************************************************\n\n");
    printf("\t  \t\t\t\t   Programa desenvolvido por\n");
    printf("\t\t\t\t    "CYAN"Andre Ferreira     "YELLOW"&"CYAN"     Jose Pereira\n"YELLOW);
    printf("\t\t\t       (a18865@alunos.ipca.pt)   (a18867@alunos.ipca.pt)\n\n");
    printf("\t  \t\t\t       Trabalho practico para as UC's de \n");
    printf("\t  \t\t\t\t\t  Estatistica\n");
    printf("\t  \t\t\t\t\t       e\n");
    printf("\t  \t\t\t        Algoritmos e Estrutura de Dados 2\n\n");
    printf("\t  *************************************************************************************************\n");
    printf("\t   *********************************************************************************************** \n");
    printf("\t    *********************************************************************************************  \n\n"RESET);

    printf(YELLOW"\tEstamos a carregar os dados.\n\tPor favor aguarde..."RESET);
}

//Full screen mode
void fullscreen()
{
    keybd_event(VK_MENU, 0x38, 0, 0);
    keybd_event(VK_RETURN, 0x1c, 0, 0);
    keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
    keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
}