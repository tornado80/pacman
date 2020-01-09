#include <stdio.h>
#include <stdlib.h>
#include <termios.h>

static struct termios oldSettings, newSettings;

/* 
Clear terminal screen using system call 
NOTE: this returns to beginning of line.
*/
void clearScreen () {
    system("clear");
}

/* Initialize new terminal i/o settings */
static void initTermios(int echo) {
    tcgetattr(0, &oldSettings); //grab old terminal i/o settings
    newSettings = oldSettings; //make new settings same as old settings
    newSettings.c_lflag &= ~ICANON; //disable buffered i/o
    newSettings.c_lflag &= echo ? ECHO : ~ECHO; //set echo mode
    tcsetattr(0, TCSANOW, &newSettings); //apply terminal i/o settings
}
 
/* Restore old terminal i/o settings */
static void resetTermios() {
    tcsetattr(0, TCSANOW, &oldSettings);
}
 
/* Read 1 character. Also echo defines echo mode. */
static char getch_ (int echo) {
    char ch;
    initTermios(echo);
    ch = getchar();
    resetTermios();
    return ch;
}
 
/* Read 1 character without echo getch() function definition. */
char getch () {
    return getch_(0);
}
 
/* Read 1 character with echo getche() function definition. */
char getche () {
    return getch_(1);
}