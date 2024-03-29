/* *NCurveCalc: A simple curve calculator
 *  * Copyright (C) 2014 Timothy Kiyui, 4316886
 *   * 
 *    * This program is free software: you can redistribute it and/or modify
 *     * it under the terms of the GNU General Public License as published by
 *      * the Free Software Foundation, either version 3 of the License, or
 *       * (at your option) any later version.
 *        * 
 *         * This program is distributed in the hope that it will be useful,
 *          * but WITHOUT ANY WARRANTY; without even the implied warranty of
 *           * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *            * GNU General Public License for more details.
 *             * 
 *              * You should have received a copy of the GNU General Public License
 *               * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *                * */
#include "point.h"
#include "clist.h"
#include "curve.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <ctype.h>
#include <curses.h>
#include <sys/stat.h>
#define putLn() (printw("\n"));

/* A structure to maintain program state */
typedef struct
{
    /* True if there are unsaved changes */
    bool isModified;
    /* True if program is running */
    bool isRunning;
}ProgramStatus;

/* region: General functions */

/* @brief Gets character, switches it to upper-case and inputs a new line
 * */
char getLn()
{
    char inChar = getch();
    inChar = toupper(inChar);
    putLn();
    return inChar;
}

/* @brief Invalid Input Screen
 * */
void invalidInput()
{
    printw("\n@Invalid input! - Press any key to continue.\n");
    refresh();
    getLn();
}

/* @brief Press any key to continue screen
 * */
void anyKey()
{
    printw("\n@Press any key to continue:\n");
    refresh();
    getLn();
}

/* @brief Prompts the user to load coordinates
 * */
void loadCurve()
{
    printw("\n@Please load coordinates before you continue.");
    anyKey();
}

/* @brief Clears curses screen & refreshes 
 * */
void clrscr()
{
    clear();
    refresh();
}

/* @brief Shows number of loaded coordinates
 * @param *list Target List
 * */
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

/* @brief Clear list of Points 
 * @param *curve Target Curve
 * */
void clearCurve(Curve *curve)
{
    Node *node;
    Node *node_now;
    Point *loopPoint;
    node = curve->list->head_node;
    if (curve->list != NULL && curve->list->size > 0)
    {
        while (node != NULL)
        {
            node_now = node;
            node = node_GetNext(node);
            loopPoint = node_now->data;
            rmPoint(loopPoint);
            rmNode(node_now);
        }
    }
    curve->list = mkList();
}

/* @brief Test if file exists 
 * @param *inputFileName String of input file name
 * @return File exist boolean
 * */
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

/* @brief Displays the Welcome Screen 
 * @param *curve Target Curve
 * @param *userInput Last input character
 * @param *isModified Boolean, are there unsaved changes
 * */
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

/* @brief Loads coordinates either from a file or from user input 
 * @param *curve Target Curve
 * @param isModified Boolean isModified value
 * @return Returns true if changes are made
 */
bool optionA(Curve *curve, bool isModified);

/* @brief Main menu option A submenu for loading Points from a file
 * @param *curve Target Curve
 * @return Returns true if changes are made
 */
bool optionAFile(Curve *curve);

/* @brief Main menu option A submenu for loading Points from user input
 * @param *curve Target Curve
 * @return Returns true if changes are made
 */
bool optionAInput(Curve *curve);

/* @brief Analyzes loaded coordinates 
 * @param *curve Target Curve
 */
void optionB(Curve *curve);

/* @brief Modifies loaded coordinates
 * @param *curve Target Curve
 * @param isModified Boolean isModified value 
 * @return Returns true if changes are made
 */
bool optionC(Curve *curve, bool isModified);

/* @brief Saves changes
 * @param *curve Target Curve
 * @return Returns true if changes are made
 */
bool optionD(Curve *curve);

/* @brief Exit program
 * @param isModified Boolean isModified value
 * @return Returns program exit status
 */
bool optionX(bool isModified);
/* endregion */

int main()
{
    ProgramStatus thisProgram;
    char userInput = ' ';
    Curve *curve = (Curve *) malloc(sizeof(Curve));
    curve->list = mkList();
    /* Clear terminal screen before initializing curses */
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    /* Initialize curses screen */
    initscr();
    thisProgram.isRunning = true;
    thisProgram.isModified = false;
    while (thisProgram.isRunning)
    {
        welcomescr(curve->list, userInput, thisProgram.isModified);
        userInput = getLn();
        clrscr();
        switch(userInput)
        {
            case 'A':
                thisProgram.isModified = optionA(curve, thisProgram.isModified);
                break;
            case 'B':
                if (curve->list->size > 0)
                    optionB(curve);
                else
                    loadCurve();
                break;
            case 'C':
                if (curve->list->size > 0)
                    thisProgram.isModified = optionC(curve, thisProgram.isModified);
                else
                    loadCurve();
                break;
            case 'D':
                if (curve->list->size > 0)
                    thisProgram.isModified = optionD(curve);
                else
                    loadCurve();
                break;
            case 'X':
                thisProgram.isRunning = optionX(thisProgram.isModified);
                break;
            default:
                invalidInput();
        }
        clrscr();
    }
    /* Free memory */
    clearCurve(curve);
    free(curve);
    /* End curses screen */
    endwin();
    /* Return to operating system */
    return 0;
}

/* region: Function definitions */
bool optionA(Curve *curve, bool isModified)
{
    char userInput;
    bool continueLoop = true;
    while (continueLoop)
    {
        clrscr();
        coordinatesLoaded(curve->list);
        printw("@Coordinate load menu:\n");
        printw("\tA - Load from file\n");
        printw("\tB - Load from input\n");
        printw("\tC - Clear points\n");
        printw("\tX - Main menu:\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        switch (userInput)
        {
            case 'A':
                isModified = optionAFile(curve);
                break;
            case 'B':
                isModified = optionAInput(curve);
                break;
            case 'C':
                clearCurve(curve);
                break;
            case 'X':
                continueLoop = false;
                break;
            default:
                invalidInput();
        }
    }
    if (isModified)
        initCurve(curve);
    return isModified;
}

void optionB(Curve *curve)
{
    char userInput;
    bool continueLoop = true;
    int loopVar;
    double length;
    Node *node;
    Node *node_now;
    Point *loopPoint;
    Point *loopPointNext;
    while (continueLoop)
    {
        clrscr();
        coordinatesLoaded(curve->list);
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
                node = curve->list->head_node;
                if (curve->list != NULL && curve->list->size > 0)
                {
                    while (node != NULL)
                    {
                        node_now = node;
                        node = node_GetNext(node);
                        loopPoint = node_now->data;
                        loopVar++;
                        clrscr();
                        printw("@Point %i of %i:\n", loopVar, curve->list->size);
                        printw("\tx: %lf\n\ty: %lf\n", loopPoint->x, loopPoint->y);
                        anyKey();
                    }
                }
                break;
            case 'B':
                printw("@Point statistics:\n");
                printw("\tLength of points: %lf\n", curve->length);
                printw("\tArea under the curve: %lf\n", curve->area);
                printw("\tLowest point: X: %lf Y: %lf\n", curve->lowPoint->x, curve->lowPoint->y);
                printw("\tHighest point: X: %lf Y: %lf\n", curve->highPoint->x, curve->highPoint->y);
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

bool optionC(Curve *curve, bool isModified)
{
    char userInput;
    bool continueLoop = true;
    double shiftX, shiftY;
    while (continueLoop)
    {
        clrscr();
        coordinatesLoaded(curve->list);
        printw("@Modify Points menu:\n");
        printw("\tA - Shift Points\n");
        printw("\tX - Main menu:\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        switch (userInput)
        {
            case 'A':
                printw("@Shift Points:\n");
                printw("\tX: ");
                refresh();
                scanw(" %lf", &shiftX);
                printw("\tY: ");
                refresh();
                scanw(" %lf", &shiftY);
                mvCurve(curve, shiftX, shiftY);
                initCurve(curve);
                break;
            case 'X':
                continueLoop = false;
                break;
            default:
                invalidInput();
        }
    }
}

bool optionD(Curve *curve)
{
    char userInput;
    bool continueLoop = true;
    char *inputFileName = (char *) malloc(64 * sizeof(char));
    bool isSaved = false;
    Node *node;
    Node *node_now;
    Point *loopPoint;
    FILE *outputFile;
    while (continueLoop)
    {
        clrscr();
        coordinatesLoaded(curve->list);
        printw("@Save changes:\n");
        printw("\tA - Save to file\n");
        printw("\tX - Main menu:\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        switch (userInput)
        {
            case 'A':
                printw("\tPlease input file name: ");
                scanw(" %s", inputFileName);
                if (fileExists(inputFileName))
                {
                    printw("@File exists!\n");
                    refresh();
                    getLn();
                }
                else
                {
                    outputFile = fopen(inputFileName, "w");
                    node = curve->list->head_node;
                    if (curve->list != NULL && curve->list->size > 0)
                    {
                        loopPoint = node->data;
                        while (node != NULL)
                        {
                            node_now = node;
                            node = node_GetNext(node);
                            loopPoint = node_now->data;
                            fprintf(outputFile, "%lf %lf\n", loopPoint->x, loopPoint->y);
                        }
                    }
                    fclose(outputFile);
                    isSaved = true;
                    printw("File save complete.\n");
                    anyKey();
                }
                break;
            case 'X':
                continueLoop = false;
                break;
            default:
                invalidInput();
        }
    }
    free(inputFileName);
    return !isSaved;
}

bool optionX(bool isModified)
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

bool optionAFile(Curve *curve)
{
    char userInput;
    Node *lastNode;
    Point *newPoint;
    Point *lastPoint;
    double x, y, sequence;
    bool isModified = false;
    bool continueLoop = true;
    char *inputFileName = (char *) malloc(64 * sizeof(char));
    bool gotDirection = false;
    bool typeDirection; /*True: Smaller False: Larger*/
    FILE *inputFile;
    clrscr();
    if (curve->list->size > 0)
    {
        printw("@Your previous coordinates will be removed, continue? (y/n)\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        if (userInput == 'N')
            return isModified;
        else
            clearCurve(curve);
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
        lastNode = curve->list->tail_node;
        if (lastNode != NULL)
        {
            lastPoint = lastNode->data;
            if (!gotDirection)
            {
                gotDirection = true;
                if (x <= lastPoint->x)
                    typeDirection = true;
                else
                    typeDirection = false;
            }
            if (x <= lastPoint->x && typeDirection)
            {
                newPoint = mkPoint(x, y);
                list_Append(curve->list, newPoint);
            }
            else if (x >= lastPoint->x && !typeDirection)
            {
                newPoint = mkPoint(x, y);
                list_Append(curve->list, newPoint);
            }
            else
            {
                printw("\t@Values must be sequential!\n\tPlease fix your file!\n");
                anyKey();
                return isModified;
            }
        }
        else
        {
            newPoint = mkPoint(x, y);
            list_Append(curve->list, newPoint);
        }
    }
    fclose(inputFile);
    free(inputFileName);
    return isModified;
}

bool optionAInput(Curve *curve)
{
    char userInput;
    Node *lastNode;
    Point *newPoint;
    Point *lastPoint;
    Point *headPoint;
    Point *tailPoint;
    double x, y, lastX;
    bool isModified = false;
    bool continueLoop = true;
    bool typeDirection; /*True: Smaller False: Larger*/
    while (continueLoop)
    {
        clrscr();
        coordinatesLoaded(curve->list);
        printw("\tAdd coordinate? (y/n)\n");
        printw("\tSelection: ");
        refresh();
        userInput = getLn();
        switch (userInput)
        {
            case 'Y':
                isModified = true;
                lastNode = curve->list->tail_node;
                if (lastNode != NULL)
                {
                    lastPoint = lastNode->data;
                    lastX = lastPoint->x;
                    headPoint = curve->list->head_node->data;
                    tailPoint = curve->list->tail_node->data;
                    if (headPoint->x > tailPoint->x)
                        typeDirection = true;
                    else if (headPoint->x < tailPoint->x)
                        typeDirection = false;
                }
                printw("\tX: ");
                refresh();
                scanw(" %lf", &x);
                if (lastNode != NULL)
                {
                    if (x < lastX && typeDirection)
                    {
                        printw("\tY: ");
                        refresh();
                        scanw(" %lf", &y);
                        newPoint = mkPoint(x, y);
                        list_Append(curve->list, newPoint);
                    }
                    else if (x > lastX && !typeDirection)
                    {
                        printw("\tY: ");
                        refresh();
                        scanw(" %lf", &y);
                        newPoint = mkPoint(x, y);
                        list_Append(curve->list, newPoint);
                    }
                    else
                    {
                        printw("\t@Values must be sequential!\n");
                        anyKey();
                    }
                }
                else
                {
                    printw("\tY: ");
                    refresh();
                    scanw(" %lf", &y);
                    newPoint = mkPoint(x, y);
                    list_Append(curve->list, newPoint);
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
