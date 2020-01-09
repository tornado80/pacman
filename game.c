#include <stdio.h>
#include <time.h>
#include "terminal.h"
#include "gui.h"
#include "core.h"
#include "config.h"
//#include "ai.h"

void greetUser () {
    clearScreen();
    setCursor(1, 1);
    printDelayed("Welcome to Pac-Man game simulator", 50, 0);
    delay(0, 1);
    setCursor(1, 2);    
    printDelayed("Written by Amirhosein Rajabi - Copyright 2020", 50, 0);
    delay(500, 1);
}

void showRules () {
    clearScreen();
    setCursor(1, 1);
    printDelayed("The goal is that " ANSI_FG_YELLOW "Pac-Man " ANSI_RESET "eats all " ANSI_FG_GREEN "green apples " ANSI_RESET "in the game board.\n"
                 "Note that:\n"
                 "\t1. The faster you go, the better you score. Also shortest path has its own points.\n"
                 "\t2. You can not pass through the houses with shaded blocks(#) or walls(" B2H ", " B2V ", " B4 ", " B3D ", " B3L ", " B3R ", " B3U ", " B3D ", ...).\n"
                 "\t3. Empty houses for free walk are indicated with white houses with 1s in them.\n"
                 "\t4. Apples are indicated with " ANSI_FG_GREEN "green" ANSI_RESET " houses and an asterisks(*).\n"
                 "\t5. Finally, " ANSI_FG_YELLOW "Pac-Man" ANSI_RESET " is a " ANSI_FG_YELLOW "yellow" ANSI_RESET " \"o\".\n"
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
    initMap();
    //gameLoop(NULL);
    /*
    if( playOrAI() ) // user playing
        gameLoop(NULL);
    else // AI playing
        gmaeLoop(smartMove);
    */
}