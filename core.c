#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "terminal.h"
#include "gui.h"
#include "core.h"
#include "config.h"
#include "linkedlist.h"

static char* mapFileName; 
static char* map;
static int mapRows;
static int mapColumns;

/* Check if char is a valid in map */
static int isMapValidCharacter (char ch) {
    if (ch != '#' && ch != '0' && ch != '1' && ch != '*')
        return 0;
    else
        return 1;
}

/* 
Read the map data (if possible) from file and writes it in an array 
Also checks for illegal characters in file
*/
static int importMap () {
    // Telling user opening file
    printf(ANSI_SAVE_CURSOR);
    fflush(stdout);
    printDelayed(ANSI_BG_CYAN ANSI_FG_WHITE "Opening ", 50, 0);
    printDelayed(mapFileName, 50, 0);
    printf(ANSI_RESET);
    fflush(stdout);    
    delay(0, 1);

    // Open file
    FILE *stream = fopen(mapFileName, "r");
    if (stream == NULL)
        return -1;

    // Reporting user successful task completion
    printf(ANSI_RESTORE_CURSOR);    
    fflush(stdout);    
    eraseCurrentLine();
    printDelayed(ANSI_BG_GREEN ANSI_FG_WHITE "File opened successfully!" ANSI_RESET, 50, 0);
    delay(0, 1);
    clearScreen();
    printDelayed(ANSI_BG_CYAN ANSI_FG_WHITE "Reading file" ANSI_RESET, 50, 0);
    delay(0, 1);

    // Now reading file
    char ch;
    int maxLineLength = 0, thisLineLength = 0, mapFileLength = 0, lineNumbers = 0;
    LinkedListData newChar;
    LinkedListNodePtr mapFile = NULL, curMapFileChar;    
    while ((ch = getc(stream)) != EOF) {
        // Analysis of read char
        if (ch == '\n') {
            lineNumbers++;
            if (maxLineLength == 0) 
                maxLineLength = thisLineLength;
            if (thisLineLength != maxLineLength) {
                fclose(stream);
                deleteLinkedList(&mapFile);
                return 2;
            }
            thisLineLength = 0;
        } else {
            if (isMapValidCharacter(ch)) {
                // update line length and add character to linked list
                thisLineLength++;
                newChar.char_value = ch;
                insertLinkedList(&mapFile, newChar);
                mapFileLength++;
            } else {
                fclose(stream);
                deleteLinkedList(&mapFile);
                return 0;
            }
        }
    }
    fclose(stream);

    // Importing map
    map = malloc(sizeof(char) * mapFileLength);
    curMapFileChar = mapFile;
    for (int i = 0; i < mapFileLength; i++) {
        *(map + i) = curMapFileChar->data.char_value;
        curMapFileChar = curMapFileChar->next;
    }
    mapRows = lineNumbers;
    mapColumns = maxLineLength;

    // Free memory allocated by linked list
    deleteLinkedList(&mapFile);

    // Reading done
    clearScreen();
    printDelayed(ANSI_BG_GREEN ANSI_FG_WHITE "Reading file successfully finished!" ANSI_RESET, 50, 0);
    delay(0, 1);

    // Draw map and get ready for game loop
    //drawMap();
    return 1;      
}

/* Prompts user for map file and initializes map array. */
void initMap () {
    int feedback, mapFileNameLength;
    LinkedListData newChar;
    LinkedListNodePtr fileName = NULL, curFileNameChar;
    do {
        // Prompt for file name
        clearScreen();
        printDelayed(ANSI_BOLD "Please enter a valid filename: " ANSI_RESET, 1, 0);
        mapFileNameLength = 0;
        while((newChar.char_value = getchar()) != '\n') {
            insertLinkedList(&fileName, newChar);
            mapFileNameLength++;
        }
        newChar.char_value = '\0';
        insertLinkedList(&fileName, newChar);  

        // converting the linked list to string
        curFileNameChar = fileName;      
        mapFileName = malloc(sizeof(char) * (mapFileNameLength + 1)); // plus 1 is for '\0'
        for (int i = 0; i < mapFileNameLength + 1; i++) { 
            *(mapFileName + i) = curFileNameChar->data.char_value;
            curFileNameChar = curFileNameChar->next;
        }

        // Free memory allocated by linked list
        deleteLinkedList(&fileName);

        // Analysis of feedback
        feedback = importMap();
        if (feedback == 1)
            break;
        else if (feedback == 0) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nIllegal characters detected in the file." ANSI_RESET ANSI_BOLD  " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        } else if (feedback == 2) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nLines length in the file are not equal. Note there should be exactly one enter after each line (including the last one)." ANSI_RESET ANSI_BOLD " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        } else if (feedback == -1) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nFile could not be opened." ANSI_RESET ANSI_BOLD " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        }
    } while (1);
}