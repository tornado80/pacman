#include <stdio.h>
#include <time.h>
#include "./terminal.h"

/* Delay for given miliseconds */
void delay (int mili_seconds, int seconds) {
    struct timespec ts;
    ts.tv_sec = seconds;
    ts.tv_nsec = mili_seconds * (int)1e6;
    nanosleep(&ts, NULL);    
}

/* Set the terminal cursor at (x, y) */
void setCursor (int x, int y) {
    printf("\e[%d;%df", y, x);
    fflush(stdout);
}

/* Clear terminal screen using ANSI */
void ansiClearScreen () {
    printf(ANSI_CLEAR_SCREEN);
    fflush(stdout);
}

/* Artistic printing each character of a string after specified delay */
void printDelayed (char* str, int mili_seconds_delay, int seconds_delay) {
    while (*str != '\0') {
        printf("%c", *str++);
        fflush(stdout); 
        delay(mili_seconds_delay, seconds_delay);
    }
}

/* Erase line given the line number */
void eraseLine (int line_number) {
    printf(ANSI_SAVE_CURSOR);
    setCursor(1, line_number);
    printf(ANSI_ERASE_LINE);
    printf(ANSI_RESTORE_CURSOR);
    fflush(stdout);
}

/* 
Erase current line
Cursor should be at the beginning of line
*/
void eraseCurrentLine () {
    printf(ANSI_SAVE_CURSOR);
    printf(ANSI_ERASE_LINE);
    printf(ANSI_RESTORE_CURSOR);
    fflush(stdout);
}