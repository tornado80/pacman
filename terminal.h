/* UNICODE BLOCK CODES */
#define LIGHT_SHADE "\u2591"
#define MEDIUM_SHADE "\u2592"
#define DARK_SHADE "\u2593"

/* UNICODE BOX CODES */
#define B4 "\u256C"
#define B3U "\u2569"
#define B3D "\u2566"	
#define B3R "\u2560"
#define B3L "\u2563"
#define B2V "\u2551"
#define B2H "\u2550"
#define B2RD "\u2554"
#define B2RU "\u255A"
#define B2LU "\u255D"
#define B2LD "\u2557"

/* ANSI TEXT ATTRIBUTES */
#define ANSI_CLEAR_SCREEN "\e[2J"
#define ANSI_ERASE_LINE "\e[K"
#define ANSI_SAVE_CURSOR "\e[s"
#define ANSI_RESTORE_CURSOR "\e[u"
#define ANSI_RESET "\e[0m"
#define ANSI_BOLD "\e[1m"
#define ANSI_BLINK "\e[5m"

/* ANSI KEYBOARD */
#define ANSI_KEY_UP "\e[A"
#define ANSI_KEY_DOWN "\e[B"
#define ANSI_KEY_RIGHT "\e[C"
#define ANSI_KEY_LEFT "\e[D"

/* ANSI FOREGROUND COLORS */
#define ANSI_FG_BLACK "\e[30m"
#define ANSI_FG_RED "\e[31m"
#define ANSI_FG_GREEN "\e[32m"
#define ANSI_FG_YELLOW "\e[33m"
#define ANSI_FG_BLUE "\e[34m"
#define ANSI_FG_MAGENTA "\e[35m"
#define ANSI_FG_CYAN "\e[36m"
#define ANSI_FG_WHITE "\e[37m"

/* ANSI BACKGROUND COLORS */
#define ANSI_BG_BLACK "\e[40m"
#define ANSI_BG_RED "\e[41m"
#define ANSI_BG_GREEN "\e[42m"
#define ANSI_BG_YELLOW "\e[43m"
#define ANSI_BG_BLUE "\e[44m"
#define ANSI_BG_MAGENTA "\e[45m"
#define ANSI_BG_CYAN "\e[46m"
#define ANSI_BG_WHITE "\e[47m"

void clearScreen ();
char getch ();
char getche ();