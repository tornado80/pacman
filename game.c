#include <stdio.h>
#include <time.h>
#include "terminal.h"
#include "gui.h"

void greetUser () {
    clearScreen();
    setCursor(1, 1);
    printDelayed("Welcome to Pac-Man game simulator", 50, 0);
    delay(0, 2);
    setCursor(1, 2);    
    printDelayed("Written by Amirhosein Rajabi - Copyright 2020", 50, 0);
    setCursor(1, 3);
    
}

int main () {
    greetUser();
}