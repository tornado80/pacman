#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "terminal.h"
#include "gui.h"
#include "core.h"
#include "config.h"
#include "linkedlist.h"

static char* mapFile; 
static char* map;

/* Prompts user for map file and initializes map array. */
void initMap () {
    LinkedListData newChar;
    LinkedListNodePtr fileNameFirstChar = NULL, fileNameLastChar = NULL;
    char ch;
    /*
    do {
        printDelayed(ANSI_BOLD "Please enter a valid filename: " ANSI_RESET, 10, 0);

        while((ch = getchar()) != '\n') {

        }

    } while (1);
    */
}