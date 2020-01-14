#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "terminal.h"
#include "gui.h"
#include "core.h"
#include "config.h"
#include "linkedlist.h"

static char* mapFileName = NULL; 
static char* map = NULL;
static int mapRows;
static int mapColumns;
static int totalApples;
static int playMode; // 1 is manual, 2 is auto(ai)

/* starting position is (1, 1) */
typedef struct {
    int x;
    int y;
} Position;

static struct {
    Position location;
    int score;
    int path;
    int apples;
    int time;
} PacMan; 


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
    printDelayed(ANSI_BG_CYAN ANSI_FG_WHITE "Opening ", 10, 0);
    printDelayed(mapFileName, 10, 0);
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
    printDelayed(ANSI_BG_GREEN ANSI_FG_WHITE "File opened successfully" ANSI_RESET, 10, 0);
    delay(0, 1);
    clearScreen();
    printDelayed(ANSI_BG_CYAN ANSI_FG_WHITE "Reading file" ANSI_RESET, 10, 0);
    delay(0, 1);

    // Now reading file
    char ch;
    int maxLineLength = 0, thisLineLength = 0, mapFileLength = 0, lineNumbers = 0, countPacMan = 0;
    LinkedListData newChar;
    LinkedListNodePtr mapFile = NULL, curMapFileChar;    
    while ((ch = getc(stream)) != EOF) {
        //Check Grid config
        if (MAP_GRID_LIMIT) {
            if (lineNumbers > MAP_MAX_ROWS)
                return 3;
            if (maxLineLength > MAP_MAX_COLS)
                return 4; 
        }
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

                // ensure there is one pacman in board also setup pacman
                if (ch == '0') {                                      
                    countPacMan++;
                    Position p = {mapFileLength, 0};
                    PacMan.location = p;
                    PacMan.apples = 0;
                    PacMan.path = 0;
                    PacMan.score = 0;
                    PacMan.time = 0;
                }

                // count apples
                if (ch == '*') {
                    totalApples++;
                }

                //simple check            
                if (countPacMan > 1)
                    return 5;
            } else {                    
                fclose(stream);
                deleteLinkedList(&mapFile);
                if (ch == 13) // carriage return
                    return 7;
                return 0;
            }
        }
    }
    fclose(stream);
    // check for last \n in file 
    if (thisLineLength != 0)
        return 8;

    // multi pacman error
    if (countPacMan != 1)
        return 6;

    // Importing map
    map = malloc(sizeof(char) * mapFileLength);
    curMapFileChar = mapFile;
    for (int i = 0; i < mapFileLength; i++) {
        *(map + i) = curMapFileChar->data.char_value;
        curMapFileChar = curMapFileChar->next;
    }
    mapRows = lineNumbers;
    mapColumns = maxLineLength;

    // Update pacman exact position
    PacMan.location.y = PacMan.location.x/mapColumns + (PacMan.location.x%mapColumns == 0 ? 0 : 1);
    PacMan.location.x = PacMan.location.x%mapColumns == 0 ? mapColumns : PacMan.location.x%mapColumns;    

    // Free memory allocated by linked list
    deleteLinkedList(&mapFile);

    // Reading done
    clearScreen();
    printDelayed(ANSI_BG_GREEN ANSI_FG_WHITE "Reading file successfully finished" ANSI_RESET, 10, 0);
    delay(0, 1);
    return 1;      
}

/* Prompts user for map file and initializes map array. */
void initMap () {
    // declarations
    int feedback, mapFileNameLength;
    LinkedListData newChar;
    LinkedListNodePtr fileName = NULL, curFileNameChar;

    do {
        // Reset some variables
        totalApples = 0;
        if (map != NULL)
            free(map);
        if (mapFileName != NULL)
            free(mapFileName);

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
        } else if (feedback == 3) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nMap grid limit is set and is exceeded. [ROWS>" STR(MAP_MAX_ROWS) "]" ANSI_RESET ANSI_BOLD " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        } else if (feedback == 4) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nMap grid limit is set and is exceeded. [COLS>" STR(MAP_MAX_COLS) "]" ANSI_RESET ANSI_BOLD " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        } else if (feedback == 5) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nMore than one PacMan found in the file." ANSI_RESET ANSI_BOLD " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        } else if (feedback == 6) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nNo PacMan found in the file." ANSI_RESET ANSI_BOLD " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        } else if (feedback == 7) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nIndicated file seems to have carriage returns. They are illegal. Maybe your file is MS-Windows based." ANSI_RESET ANSI_BOLD " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        } else if (feedback == 8) {
            printDelayed(ANSI_BG_RED ANSI_BOLD ANSI_FG_WHITE "\nLast line of the file should be a newline (\\n) character." ANSI_RESET ANSI_BOLD " Press any key to continue: " ANSI_RESET, 10, 0);
            getch();
        }  
    } while (1);
}

void setPlayMode () {
    clearScreen();
    printDelayed(
                ANSI_BOLD "Please type 1 or 2 based on below playing modes:\n"ANSI_RESET
                "\t " ANSI_BOLD "1." ANSI_RESET " Manual (user plays).\n"
                "\t " ANSI_BOLD "2." ANSI_RESET " AI (computer plays).\n"
                ANSI_SAVE_CURSOR ANSI_BLINK "? " ANSI_RESET
                , 10, 0);
    // prompt user
    char ch;
    do {
        ch = getche();
        if (ch == '1') {
            break;
        } else if (ch == '2') {
            break;
        } else if (ch == '\e') {
            getch();
            getch();
            printDelayed(ANSI_RESTORE_CURSOR ANSI_ERASE_LINE ANSI_RESTORE_CURSOR ANSI_BG_RED ANSI_FG_WHITE "Invalid." ANSI_RESET ANSI_BLINK" ? " ANSI_RESET, 10, 0);
        } else {
            printDelayed(ANSI_RESTORE_CURSOR ANSI_ERASE_LINE ANSI_RESTORE_CURSOR ANSI_BG_RED ANSI_FG_WHITE "Invalid." ANSI_RESET ANSI_BLINK" ? " ANSI_RESET, 10, 0);
        }
    } while (1);

    if (ch == '1') {
        playMode = 1;
        printDelayed(ANSI_RESTORE_CURSOR ANSI_ERASE_LINE ANSI_RESTORE_CURSOR ANSI_BG_GREEN ANSI_FG_WHITE "Playing mode set to manual." ANSI_RESET, 30, 0);
    } else {
        playMode = 2;
        printDelayed(ANSI_RESTORE_CURSOR ANSI_ERASE_LINE ANSI_RESTORE_CURSOR ANSI_BG_GREEN ANSI_FG_WHITE "Playing mode set to automatic." ANSI_RESET, 30, 0);
    }
    delay(0, 1);
}

/* Check if given position is in map */
static int isInMap (Position pos) {
    if (pos.x >= 1 && pos.x <= mapColumns && pos.y >= 1 && pos.y <= mapRows)
        return 1;
    else
        return 0;
}

/* Return linear format of a position used in indexing map */
static char * toLinear (Position pos) {
    return map + mapColumns * (pos.y - 1) - 1 + pos.x;
}

/* Check if given position is a block */
static int isBlock (Position pos) {
    if (isInMap(pos))
        if (*toLinear(pos) == '#')
            return 1;
        else
            return 0;
    else
        return 0;
}

/* Check if given position is a path */
static int isPath (Position pos) {
    if (isInMap(pos))
        if (*toLinear(pos) == '1')
            return 1;
        else
            return 0;
    else
        return 0;
}

/* Check if given position is pacman */
static int isPacMan (Position pos) {
    if (isInMap(pos))
        if (*toLinear(pos) == '0')
            return 1;
        else
            return 0;
    else
        return 0;
}

/* Check if given position is apple */
static int isApple (Position pos) {
    if (isInMap(pos))
        if (*toLinear(pos) == '*')
            return 1;
        else
            return 0;
    else
        return 0;
}

static void drawFrame () {
    clearScreen();
    printDelayed(B2RD, 5, 0);
    for (int i = 0; i < mapColumns; i++)
        printDelayed(B2H, 5, 0);
    printDelayed(B2LD, 5, 0);
    for (int i = 0; i < mapRows; i++) {
        setCursor(1, i + 2);
        printDelayed(B2V, 5, 0);
        setCursor(mapColumns + 2, i + 2);
        printDelayed(B2V, 5, 0);        
    }
    setCursor(1, mapRows + 2);
    printDelayed(B2RU, 5, 0);
    for (int i = 0; i < mapColumns; i++)
        printDelayed(B2H, 5, 0);
    printDelayed(B2LU, 5, 0);        
}

void drawMap () {
    // loading splash and drawing board frame
    clearScreen();
    printf(ANSI_BG_CYAN ANSI_FG_WHITE ANSI_BOLD);
    fflush(stdout);
    printDelayed("Loading Map ...", 100, 0);
    printf(ANSI_RESET);
    fflush(stdout);    
    delay(0, 1);
    drawFrame();

    // drawing map
    Position pos;
    for (int i = 0; i < mapRows; i++) {
        for (int j = 0; j < mapColumns; j++) {
            setCursor(2 + j, 2 + i);
            pos.x = j + 1;
            pos.y = i + 1;
            if (isBlock(pos)) {
                printf(ANSI_BG_RED ANSI_FG_WHITE);
                printDelayed("#", 5, 0);
                printf(ANSI_RESET);
                fflush(stdout);
            }
            if (isPath(pos)) {
                printf(ANSI_BG_WHITE ANSI_FG_BLACK);
                printDelayed(" ", 5, 0);
                printf(ANSI_RESET);
                fflush(stdout);
            }
            if (isApple(pos)) {
                printf(ANSI_BG_GREEN ANSI_FG_WHITE);
                printDelayed("*", 5, 0);
                printf(ANSI_RESET);
                fflush(stdout);
            }
            if (isPacMan(pos)) {
                printf(ANSI_BG_BLUE ANSI_FG_BLACK);
                printDelayed("0", 5, 0);
                printf(ANSI_RESET);
                fflush(stdout);
            }                                    
        }
    }
}

static int canMove (Position pos) {
    if (!isBlock(pos) && isInMap(pos))
        return 1;
    else
        return 0;
}

static Position nextPosition (Position pos, char ch) {
    if (ch == 'u')
        pos.y--;
    if (ch == 'd')
        pos.y++;
    if (ch == 'r')
        pos.x++;
    if (ch == 'l')
        pos.x--;   
    return pos;                 
}

static int movePacMan (char (*moveFunction) (char *, Position, int, int, int)) {
    char direction;
    if (moveFunction == NULL) {
        //setCursor(1, mapRows + 3);
        //eraseLine(mapRows + 3);                
        printDelayed("? ", 50, 0);        
        char ch;
        ch = getch();
        if (ch == '\e') {
            ch = getch();
            if (ch == '[') {
                ch = getch();
                if (ch == 'A')
                    direction = 'u';
                else if (ch == 'B')
                    direction = 'd';
                else if (ch == 'C')
                    direction = 'r';
                else if (ch == 'D')
                    direction = 'l';   
                else {
                    setCursor(1, mapRows + 3);
                    eraseLine(mapRows + 3);
                    printf(ANSI_BG_RED ANSI_FG_WHITE ANSI_BOLD "Illegal key. Use arrow keys." ANSI_RESET " ");
                    fflush(stdout);                      
                    //delay(200, 0);
                    return 0;
                }
            } else {
                setCursor(1, mapRows + 3);
                eraseLine(mapRows + 3);
                printf(ANSI_BG_RED ANSI_FG_WHITE ANSI_BOLD "Illegal key. Use arrow keys." ANSI_RESET " ");
                fflush(stdout);  
                //delay(200, 0);              
                return 0;
            }
        } else {
            setCursor(1, mapRows + 3);
            eraseLine(mapRows + 3);
            printf(ANSI_BG_RED ANSI_FG_WHITE ANSI_BOLD "Illegal key. Use arrow keys." ANSI_RESET " ");
            fflush(stdout);  
            //delay(200, 0);
            return 0;
        }
    } else {
        direction = moveFunction(map, PacMan.location, totalApples, PacMan.apples, PacMan.path);            
    }
    Position newPos = nextPosition(PacMan.location, direction);
    if (!canMove(newPos)) {
        setCursor(1, mapRows + 3);
        eraseLine(mapRows + 3);
        printf(ANSI_BG_RED ANSI_FG_WHITE ANSI_BOLD "PacMan can not move in that direction. Try again." ANSI_RESET " ");
        fflush(stdout);  
        //delay(200, 0);
        return 0;
    } else {
        // update alert line
        setCursor(1, mapRows + 3);
        eraseLine(mapRows + 3);

        // update board
        setCursor(PacMan.location.x + 1, PacMan.location.y + 1);
        printf(ANSI_BG_WHITE ANSI_FG_BLACK);
        printf(" ");
        printf(ANSI_RESET);
        fflush(stdout);
                     
        // update pacman and map
        if (isApple(newPos)) {
            *toLinear(PacMan.location) = '1';
            PacMan.location = newPos;
            PacMan.apples++;
            PacMan.path++;
            *toLinear(PacMan.location) = '0';           
        } else if (isPath(newPos)) {
            *toLinear(PacMan.location) = '1';
            PacMan.location = newPos;
            PacMan.path++;
            *toLinear(PacMan.location) = '0';            
        }

        // update map
        setCursor(PacMan.location.x + 1, PacMan.location.y + 1);
        printf(ANSI_BG_BLUE ANSI_FG_BLACK);
        printf("0");
        printf(ANSI_RESET);
        fflush(stdout);
        setCursor(1, mapRows + 3);
        return 1;
    }
} 

void gameLoop () {
    setCursor(1, mapRows + 3);
    printDelayed(ANSI_BG_CYAN ANSI_FG_YELLOW ANSI_BOLD "You have to use Arrow Keys to move PacMan. Press any key to start the game ..." ANSI_RESET, 1, 0);
    getch();
    time_t timeElapsed = time(NULL);
    setCursor(1, mapRows + 3);
    eraseLine(mapRows + 3);         
    while (PacMan.apples != totalApples) {
        if (playMode == 1)
            movePacMan(NULL);
        else {
            printf("Not available yet!");
            exit(0);
        }
    }
    timeElapsed = time(NULL) - timeElapsed;
    setCursor(1, mapRows + 3);
    eraseLine(mapRows + 3);
    printDelayed(ANSI_BG_GREEN ANSI_FG_WHITE ANSI_BOLD "PacMan has eaten all the apples. Congratulations.\n" ANSI_RESET, 20, 0);
    printf(ANSI_BG_GREEN ANSI_FG_WHITE ANSI_BOLD "PacMan took %ld seconds to eat all foods.\n" ANSI_RESET, timeElapsed);
}
