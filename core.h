/* starting position is (1, 1) */
typedef struct {
    int x;
    int y;
} Position;

int isInMap (Position pos);
int isBlock (Position pos);
int isPath (Position pos);
int isApple (Position pos);
int isPacMan (Position pos);
int toLinearGen (Position pos, int gridCols);
Position toPosGen (int linearPos, int gridCols);
Position nextPosition (Position pos, char ch);
void setMap (Position pos, char ch);
int canMove (Position pos);
void initMap ();
void setPlayMode ();
void gameLoop ();
void drawMap ();

