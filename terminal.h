// ANSI TEXT ATTRIBUTES
#define ANSI_CLEAR_SCREEN "\e[2J"
#define ANSI_ERASE_LINE "\e[K"
#define ANSI_SAVE_CURSOR "\e[s"
#define ANSI_RESTORE_CURSOR "\e[u"
#define ANSI_RESET "\e[0m"
#define ANSI_BOLD "\e[1m"
#define ANSI_BLINK "\e[5m"

// ANSI FOREGROUND COLORS
#define ANSI_FG_BLACK "\e[30m"
#define ANSI_FG_RED "\e[31m"
#define ANSI_FG_GREEN "\e[32m"
#define ANSI_FG_YELLOW "\e[33m"
#define ANSI_FG_BLUE "\e[34m"
#define ANSI_FG_MAGENTA "\e[35m"
#define ANSI_FG_CYAN "\e[36m"
#define ANSI_FG_WHITE "\e[37m"

// ANSI BACKGROUND COLORS
#define ANSI_BG_BLACK "\e[40m"
#define ANSI_BG_RED "\e[41m"
#define ANSI_BG_GREEN "\e[42m"
#define ANSI_BG_YELLOW "\e[43m"
#define ANSI_BG_BLUE "\e[44m"
#define ANSI_BG_MAGENTA "\e[45m"
#define ANSI_BG_CYAN "\e[46m"
#define ANSI_BG_WHITE "\e[47m"

// Function Prototypes
void setCursor (int x, int y);
void clearScreen ();
char getch ();
char getche ();