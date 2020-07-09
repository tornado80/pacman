#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../utils/terminal.h"
#include "../utils/gui.h"
#include "./core.h"
#include "./ai.h"
#include "../utils/config.h"
#include "../ds/linkedlist.h"

static int playMode; // 1 is manual, 2 is auto(ai)

static struct {
    char* fileName;
    char* grid;
    int rows;
    int columns;
    int apples;
} Map = {NULL, NULL};

static struct {
    Position location;
    int score;
    int path;
    int apples;
    int time;
} PacMan; 

/* Return linear format of a position used in indexing Map.grid */
static int toLinear (Position pos) {
    return Map.columns * (pos.y - 1) - 1 + pos.x; // -1 is for zero-indexed of Map.grid
}

/* 
Return linear format of a position used in indexing zero-base grid
This function use a general grid column count
*/
int toLinearGen (Position pos, int gridCols) {
    return gridCols * (pos.y - 1) - 1 + pos.x; // -1 is for zero-indexed of Map.grid
}

/* Return position format of a linear indexing of Map.grid */
static Position toPos (int linearPos) {
    linearPos++; // convert zero-indexed of Map.grid to 1-indexed
    Position pos;
    pos.y = linearPos/Map.columns + (linearPos%Map.columns == 0 ? 0 : 1);
    pos.x = linearPos%Map.columns == 0 ? Map.columns : linearPos%Map.columns; 
    return pos;
} 

/* 
Return position format of a linear indexing of zero-base grid
This function use a general grid column count
*/
Position toPosGen (int linearPos, int gridCols) {
    linearPos++; // convert zero-indexed of Map.grid to 1-indexed
    Position pos;
    pos.y = linearPos/gridCols + (linearPos%gridCols == 0 ? 0 : 1);
    pos.x = linearPos%gridCols == 0 ? gridCols : linearPos%gridCols; 
    return pos;
} 

/* Set a given position of map a character */
void setMap (Position pos, char ch) {
    *(Map.grid + toLinear(pos)) = ch;
}

/* Get the character of given position of map */
static char getMap (Position pos) {
    return *(Map.grid + toLinear(pos));
}

/* Check if char is a valid in Map.grid */
static int isMapValidCharacter (char ch) {
    if (ch != '#' && ch != '0' && ch != '1' && ch != '*')
        return 0;
    else
        return 1;
}

/* 
Read the Map.grid data (if possible) from file and writes it in an array 
Also checks for illegal characters in file
*/
static int importMap () {
    // Telling user opening file
    printf(ANSI_SAVE_CURSOR);
    fflush(stdout);
    printDelayed(ANSI_BG_CYAN ANSI_FG_WHITE "Opening ", 10, 0);
    printDelayed(Map.fileName, 10, 0);
    printf(ANSI_RESET);
    fflush(stdout);    
    delay(0, 1);

    // Open file
    FILE *stream = fopen(Map.fileName, "r");
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
    int maxLineLength = 0, thisLineLength = 0, mapFileLength = 0, lineNumbers = 0, countPacMan = 0, linearPacManPosition = 0;;
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
                    linearPacManPosition = mapFileLength;
                    PacMan.apples = 0;
                    PacMan.path = 0;
                    PacMan.score = 0;
                    PacMan.time = 0;
                }

                // count apples
                if (ch == '*') {
                    Map.apples++;
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

    // Importing Map.grid data read from file to memory
    Map.grid = malloc(sizeof(char) * mapFileLength);
    curMapFileChar = mapFile;
    for (int i = 0; i < mapFileLength; i++) {
        *(Map.grid + i) = curMapFileChar->data.char_value;
        curMapFileChar = curMapFileChar->next;
    }
    Map.rows = lineNumbers;
    Map.columns = maxLineLength;

    // Update pacman exact position
    PacMan.location = toPos(linearPacManPosition - 1); // -1 is for zero-indexed of   

    // Free memory allocated by linked list
    deleteLinkedList(&mapFile);

    // Reading done
    clearScreen();
    printDelayed(ANSI_BG_GREEN ANSI_FG_WHITE "Reading file successfully finished" ANSI_RESET, 10, 0);
    delay(0, 1);
    return 1;      
}

/* Prompts user for Map.grid file and initializes Map.grid array. */
void initMap () {
    // declarations
    int feedback, mapFileNameLength;
    LinkedListData newChar;
    LinkedListNodePtr fileName = NULL, curFileNameChar;

    do {
        // Reset some variables
        Map.apples = 0;
        if (Map.grid != NULL)
            free(Map.grid);
        if (Map.fileName != NULL)
            free(Map.fileName);

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
        Map.fileName = malloc(sizeof(char) * (mapFileNameLength + 1)); // plus 1 is for '\0'
        for (int i = 0; i < mapFileNameLength + 1; i++) { 
            *(Map.fileName + i) = curFileNameChar->data.char_value;
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
        ch = getch();
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

/* Check if given position is in Map.grid */
int isInMap (Position pos) {
    if (pos.x >= 1 && pos.x <= Map.columns && pos.y >= 1 && pos.y <= Map.rows)
        return 1;
    else
        return 0;
}

/* Check if given position is a block */
int isBlock (Position pos) {
    if (isInMap(pos))
        if (getMap(pos) == '#')
            return 1;
        else
            return 0;
    else
        return 0;
}

/* Draws a Block, the cursor should have been set before this command */ 
static void drawBlock (int delay_ms, int delay_s) {
    printf(ANSI_BG_RED ANSI_FG_WHITE);
    printDelayed("#", delay_ms, delay_s);
    printf(ANSI_RESET);
    fflush(stdout);
}

/* Check if given position is a path */
int isPath (Position pos) {
    if (isInMap(pos))
        if (getMap(pos) == '1')
            return 1;
        else
            return 0;
    else
        return 0;
}

/* Draws a Path, the cursor should have been set before this command */ 
static void drawPath(int delay_ms, int delay_s) {
    printf(ANSI_BG_WHITE ANSI_FG_BLACK);
    printDelayed(" ", delay_ms, delay_s);
    printf(ANSI_RESET);
    fflush(stdout); 
}

/* Check if given position is pacman */
int isPacMan (Position pos) {
    if (isInMap(pos))
        if (getMap(pos) == '0')
            return 1;
        else
            return 0;
    else
        return 0;
}

/* Draws a PacMan , the cursor should have been set before this command */ 
static void drawPacMan(int delay_ms, int delay_s) {
    printf(ANSI_BG_BLUE ANSI_FG_WHITE);
    printDelayed("0", delay_ms, delay_s);
    printf(ANSI_RESET);
    fflush(stdout);   
}

/* Check if given position is apple */
int isApple (Position pos) {
    if (isInMap(pos))
        if (getMap(pos) == '*')
            return 1;
        else
            return 0;
    else
        return 0;
}

/* Draws an Apple , the cursor should have been set before this command */ 
static void drawApple(int delay_ms, int delay_s) {
    printf(ANSI_BG_GREEN ANSI_FG_WHITE);
    printDelayed("*", delay_ms, delay_s);
    printf(ANSI_RESET);
    fflush(stdout);    
}

static void drawFrame () {
    clearScreen();
    printDelayed(B2RD, 5, 0);
    for (int i = 0; i < Map.columns; i++)
        printDelayed(B2H, 5, 0);
    printDelayed(B2LD, 5, 0);
    for (int i = 0; i < Map.rows; i++) {
        setCursor(1, i + 2);
        printDelayed(B2V, 5, 0);
        setCursor(Map.columns + 2, i + 2);
        printDelayed(B2V, 5, 0);        
    }
    setCursor(1, Map.rows + 2);
    printDelayed(B2RU, 5, 0);
    for (int i = 0; i < Map.columns; i++)
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

    // drawing Map.grid
    Position pos;
    for (int i = 0; i < Map.rows; i++) {
        for (int j = 0; j < Map.columns; j++) {
            setCursor(2 + j, 2 + i);
            pos.x = j + 1;
            pos.y = i + 1;
            if (isBlock(pos))
                drawBlock(5, 0);
            if (isPath(pos))
                drawPath(5, 0);
            if (isApple(pos))
                drawApple(5, 0);
            if (isPacMan(pos))
                drawPacMan(5, 0);                                    
        }
    }
}

int canMove (Position pos) {
    if (!isBlock(pos) && isInMap(pos))
        return 1;
    else
        return 0;
}

Position nextPosition (Position pos, char ch) {
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

static int movePacMan (char (*moveFunction) (Position, int)) {
    char direction;
    if (moveFunction == NULL) {               
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
                    setCursor(1, Map.rows + 3);
                    eraseLine(Map.rows + 3);
                    printf(ANSI_BG_RED ANSI_FG_WHITE ANSI_BOLD "Illegal key. Use arrow keys." ANSI_RESET " ");
                    fflush(stdout);                      
                    return 0;
                }
            } else {
                setCursor(1, Map.rows + 3);
                eraseLine(Map.rows + 3);
                printf(ANSI_BG_RED ANSI_FG_WHITE ANSI_BOLD "Illegal key. Use arrow keys." ANSI_RESET " ");
                fflush(stdout);              
                return 0;
            }
        } else {
            setCursor(1, Map.rows + 3);
            eraseLine(Map.rows + 3);
            printf(ANSI_BG_RED ANSI_FG_WHITE ANSI_BOLD "Illegal key. Use arrow keys." ANSI_RESET " ");
            fflush(stdout);  
            return 0;
        }
    } else {
        direction = moveFunction(PacMan.location, Map.columns);         
    }
    Position newPos = nextPosition(PacMan.location, direction);
    if (!canMove(newPos)) {
        setCursor(1, Map.rows + 3);
        eraseLine(Map.rows + 3);
        printf(ANSI_BG_RED ANSI_FG_WHITE ANSI_BOLD "PacMan can not move in that direction. Try again." ANSI_RESET " ");
        fflush(stdout);  
        return 0;
    } else {
        // update alert line
        setCursor(1, Map.rows + 3);
        eraseLine(Map.rows + 3);

        // update board
        setCursor(PacMan.location.x + 1, PacMan.location.y + 1);
        drawPath(0, 0);
                     
        // update pacman and Map.grid
        if (isApple(newPos)) {
            setMap(PacMan.location, '1');
            PacMan.location = newPos;
            PacMan.apples++;
            PacMan.path++;
            setMap(PacMan.location, '0');           
        } else if (isPath(newPos)) {
            setMap(PacMan.location, '1');
            PacMan.location = newPos;
            PacMan.path++;
            setMap(PacMan.location, '0');            
        }

        // update Map.grid
        setCursor(PacMan.location.x + 1, PacMan.location.y + 1);
        drawPacMan(0, 0);
        setCursor(1, Map.rows + 3);
        return 1;
    }
} 

void gameLoop () {
    setCursor(1, Map.rows + 3);
    printDelayed(ANSI_BG_CYAN ANSI_FG_YELLOW ANSI_BOLD "You have to use Arrow Keys to move PacMan. Press any key to start the game ..." ANSI_RESET, 1, 0);
    getche();
    time_t timeElapsed = time(NULL);
    if (playMode == 2) {
        if(aiSetup(Map.grid, PacMan.location, Map.apples, Map.rows, Map.columns) == -1) exit(0);
        while (PacMan.apples != Map.apples)
            movePacMan(aiMove);      
    } else if (playMode == 1) {
        setCursor(1, Map.rows + 3);
        eraseLine(Map.rows + 3); 
        while (PacMan.apples != Map.apples)
            movePacMan(NULL); 
    }
    timeElapsed = time(NULL) - timeElapsed;
    setCursor(1, Map.rows + 3);
    eraseLine(Map.rows + 3);
    printDelayed(ANSI_BG_GREEN ANSI_FG_WHITE ANSI_BOLD "PacMan has eaten all the apples. Congratulations.\n" ANSI_RESET, 20, 0);
    printf(ANSI_BG_GREEN ANSI_FG_WHITE ANSI_BOLD "PacMan took %ld seconds to eat all foods.\n" ANSI_RESET, timeElapsed);
}
