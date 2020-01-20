#include <stdio.h>
#include <time.h>
#include "./utils/terminal.h"
#include "./utils/gui.h"
#include "./game/core.h"

void greetUser () {
    clearScreen();
    setCursor(1, 1);
    printDelayed(ANSI_BOLD "Welcome to Pac-Man game simulator", 50, 0);
    delay(0, 1);
    setCursor(1, 2);    
    printDelayed("Written by Amirhosein Rajabi - Copyright 2020" ANSI_RESET, 50, 0);
    delay(500, 1);
}

void showRules () {
    clearScreen();
    setCursor(1, 1);
    printDelayed("The goal is that " ANSI_FG_YELLOW "Pac-Man " ANSI_RESET "eats all " ANSI_FG_GREEN "green apples " ANSI_RESET "in the game board.\n"
                 "Note that:\n"
                 //"\t1. The faster you go, the better you score. Also shortest path has its own points.\n"
                 "\t1. You can not pass through the houses with sharps(#).\n"
                 "\t2. Empty houses for free walk are indicated with white houses.\n"
                 "\t3. Apples are indicated with " ANSI_FG_GREEN "green" ANSI_RESET " houses and an asterisks(*).\n"
                 "\t4. Finally, " ANSI_FG_YELLOW "Pac-Man" ANSI_RESET " is a " ANSI_FG_BLUE "blue" ANSI_RESET " \"0\".\n"
                 ANSI_FG_RED ANSI_BOLD "IMPORTANT: " ANSI_RESET ANSI_BOLD "Before playing you have to enter a valid file name for the map.\n" ANSI_RESET
                 "Good luck!\n"
                 , 50, 0);
    printf(ANSI_BLINK ANSI_BG_CYAN ANSI_FG_YELLOW "Press any key to continue..." ANSI_RESET);
    fflush(stdout);
    getch();
} 

int main () {
    greetUser();
    showRules();
    while(1) {
        initMap();
        setPlayMode();
        drawMap();
        gameLoop();
        printDelayed(ANSI_BG_CYAN ANSI_FG_WHITE ANSI_BOLD "Press enter to play a new map" ANSI_RESET, 50, 0);
        while (getchar() != '\n');
    }
}
