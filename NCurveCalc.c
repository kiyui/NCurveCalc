#include "point.h"
#include "clist.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <sys/stat.h>
#define putLn() (printw("\n"));

typedef struct
{
    /* True if List contains Points */
    bool isLoaded;
    /* True if there are unsaved changes */
    bool isModified;
    /* Truw if program is running */
    bool isRunning;
}ProgramStatus;

/* region: General functions */
/* getch() + toupper() + putLn() */
char getLn()
{
    char inChar = getch();
    inChar = toupper(inChar);
    putLn();
    return inChar;
}

/* Invalid Input Screen */
void invalidInput()
{
    printw("@Invalid input! - Press any key to continue.");
    getLn();
}

void anyKey()
{
    printw("@Press any key to continue:\n");
    refresh();
    getLn();
}

/* Clears curses screens */
void clrscr()
{
    clear();
    refresh();
}

/* Shows number of loaded coordinates */
void coordinatesLoaded(List *list)
{
    if (list->size == 0)
        printw("@No coordinates loaded.\n");
    else if (list->size == 1)
        printw("@1 coordinate loaded.\n");
    else
        printw("@%i coordinates loaded.\n", list->size);
    refresh();
}

/* Clear list of Points */
void clearList(List *list)
{
    Node *node;
    Node *node_now;
    Point *loopPoint;
    node = list->head_node;
    if (list != NULL && list->size > 0)
    {
        while (node != NULL)
        {
            node_now = node;
            node = node_GetNext(node);
            loopPoint = node_now->data;
            rmPoint(loopPoint);
        }
    }
    rmList(list);
    list = mkList();
}

/* Test if file exists */
bool fileExists(char *inputFileName)
{
    int exists;
    #ifdef _WIN32
        struct _stat fileTest;
        exists = _stat(inputFileName, &fileTest);
    #else
        struct stat fileTest;
        exists = stat(inputFileName, &fileTest);
    #endif
    return exists >= 0;
}

/* Displays the Welcome Screen */
void welcomescr(List *list, char userInput, bool isModified)
{
    printw("################################\n");
    printw("##########~NCurveCalc~##########\n");
    printw("################################\n");
    coordinatesLoaded(list);
    if (isModified)
        printw("@There are unsaved changes.\n");
    printw("@Previous selection: %c\n\n", userInput);
    printw("\tA - Load coordinates\n");
    printw("\tB - Analyze coordinates\n");
    printw("\tC - Modify coordinates\n");
    printw("\tD - Save changes\n");
    printw("\tX - Exit program\n");
    printw("\tSelection: ");
    refresh();
}
/* endregion */

/* region: Function prototypes */
/* Loads coordinates either from a file or from user input */
bool optionA(List *list, bool *isPModified);
bool optionAFile(List *list);
bool optionAInput(List *list);

/* Analyzes loaded coordinates */
void optionB(List *list);

/* Modifies loaded coordinates */
bool optionC(List *list, bool *isPModified);

/* Saves changes */
bool optionD(List *list);

/* Exit program */
bool optionX(List *list, bool isModified);
/* endregion */

int main()
{
    ProgramStatus thisProgram;
    char userInput = ' ';
    List *list = mkList();
    /* Clear terminal screen before initializing curses */
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    /* Initialize curses screen */
    initscr();
    thisProgram.isRunning = true;
    thisProgram.isLoaded = false;
    thisProgram.isModified = false;
    while (thisProgram.isRunning)
    {
        thisProgram.isLoaded = list->size > 0;
        welcomescr(list, userInput, thisProgram.isModified);
        userInput = getLn();
        clrscr();
        switch(userInput)
        {
            case 'A':
                thisProgram.isModified = optionA(list, &thisProgram.isModified);
                break;
            case 'B':
                optionB(list);
                break;
            case 'C':
                thisProgram.isModified = optionC(list, &thisProgram.isModified);
                break;
            case 'D':
                thisProgram.isModified = optionD(list);
                break;
            case 'X':
                thisProgram.isRunning = optionX(list, thisProgram.isModified);
                break;
            default:
                invalidInput();
        }
        clrscr();
    }
    /* Free memory */
    rmList(list);
    /* End curses screen */
    endwin();
    /* Return to operating system */
    return 0;
}

/* region: Function definitions */
bool optionA(List *list, bool *isPModified)
{
    char userInput;
    bool isModified = *isPModified;
    bool continueLoop = true;
    while (continueLoop)
    {
        clrscr();
        printw("@Coordinate load menu:\n");
        printw("\tA - Load from file\n");
        printw("\tB - Load from input\n");
        printw("\tX - Main menu:\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        switch (userInput)
        {
            case 'A':
                isModified = optionAFile(list);
                break;
            case 'B':
                isModified = optionAInput(list);
                break;
            case 'X':
                continueLoop = false;
                break;
            default:
                invalidInput();
        }
    }
    return isModified;
}

void optionB(List *list)
{
    char userInput;
    bool continueLoop = true;
    int loopVar;
    double length;
    Node *node;
    Node *node_now;
    Point *loopPoint;
    Point *loopPointNext;
    node = list->head_node;
    while (continueLoop)
    {
        clrscr();
        printw("@Analyze Points menu:\n");
        printw("\tA - Display points\n");
        printw("\tB - Point statistics\n");
        printw("\tX - Main menu:\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        switch (userInput)
        {
            case 'A':
                clrscr();
                loopVar = 0;
                node = list->head_node;
                if (list != NULL && list->size > 0)
                {
                    while (node != NULL)
                    {
                        node_now = node;
                        node = node_GetNext(node);
                        loopPoint = node_now->data;
                        loopVar++;
                        clrscr();
                        printw("@Point %i of %i:\n", loopVar, list->size);
                        printw("\tx: %lf\n\ty: %lf\n", loopPoint->x, loopPoint->y);
                        anyKey();
                    }
                }
                break;
            case 'B':
                length = 0;
                node = list->head_node;
                if (list->size > 0)
                {
                    if (list != NULL && list->size > 0)
                    {
                        while (node != NULL)
                        {
                            node_now = node;
                            node = node_GetNext(node);
                            loopPoint = node_now->data;
                            if (node != NULL)
                            {
                                loopPointNext = node->data;
                                length += calcPointLength(loopPoint, loopPointNext);
                            }
                        }
                    }
                }
                printw("@Point statistics:\n");
                printw("\tLength of points: %lf\n", length);
                anyKey();
                break;
            case 'X':
                continueLoop = false;
                break;
            default:
                invalidInput();
        }
        putLn();
    }
}

bool optionC(List *list, bool *isPModified)
{
}

bool optionD(List *list)
{
}

bool optionX(List *list, bool isModified)
{
    char userInput;
    bool isRunning;
    bool continueLoop = true;
    while (continueLoop)
    {
        clrscr();
        printw("@Exit Program.\n");
        if (isModified)
            printw("@Warning! There are unsaved changes!\n");
        printw("@Are you sure you want to exit? (y/n)\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        switch (userInput)
        {
            case 'Y':
                isRunning = false;
                continueLoop = false;
                break;
            case 'N':
                isRunning = true;
                continueLoop = false;
                break;
            default:
                invalidInput();
        }
        putLn();
    }
    return isRunning;
}

bool optionAFile(List *list)
{
    char userInput;
    Point *newPoint;
    double x, y;
    bool isModified = false;
    bool continueLoop = true;
    char *inputFileName = (char *) malloc(64 * sizeof(char));
    FILE *inputFile;
    clrscr();
    if (list->size > 0)
    {
        printw("@Your previous coordinates will be removed, continue? (y/n)\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        if (userInput == 'N')
            return isModified;
        else
            clearList(list);
    }
    printw("@Please input file name: ");
    refresh();
    scanw(" %s", inputFileName);
    if (!fileExists(inputFileName))
    {
        printw("@File does not exist!\n");
        refresh();
        getLn();
        return isModified;
    }
    inputFile = fopen(inputFileName, "r");
    if (inputFile != NULL)
        isModified = true;
    while (fscanf(inputFile, "%lf %lf", &x, &y) == 2)
    {
        newPoint = mkPoint(x, y);
        list_Append(list, newPoint);
    }
    return isModified;
}

bool optionAInput(List *list)
{
    char userInput;
    Node *lastNode;
    Point *newPoint;
    Point *lastPoint;
    double x, y, lastX;
    bool isModified = false;
    bool continueLoop = true;
    while (continueLoop)
    {
        clrscr();
        coordinatesLoaded(list);
        printw("\tAdd coordinate? (y/n)\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        switch (userInput)
        {
            case 'Y':
                isModified = true;
                lastNode = list->tail_node;
                if (lastNode != NULL)
                {
                    lastPoint = lastNode->data;
                    lastX = lastPoint->x;
                }
                printw("\tX: ");
                refresh();
                scanw(" %lf", &x);
                if (lastNode != NULL && x <= lastX)
                {
                    printw("@Value must be greater than %lf!\n", lastX);
                    anyKey();
                }
                else
                {
                    printw("\tY: ");
                    refresh();
                    scanw(" %lf", &y);
                    newPoint = mkPoint(x, y);
                    list_Append(list, newPoint);
                    if (lastNode != NULL)
                        rmPoint(lastPoint);
                }
                break;
            case 'N':
                continueLoop = false;
                break;
            default:
                invalidInput();
        }
    }
    return isModified;
}
/* endregion */
