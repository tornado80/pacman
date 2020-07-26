# Pac-Man
Pac-Man Game Simulation & AI\
This project is licensed under MIT software licenses.

## Supported platforms
This project can run **only** on linux-based operating systems supporting **GNU compiler** and **make** tool.
This is due to support for ANSI special characters.

## How to build the project?
1. Change directory to project files.
2. Type ```$ make``` and enter.
3. Then run program with ```$ ./main```.

## Where are sample Map files? What are the conventions to make a new one?
You can find some samples in ``test`` directory. You can also build new ones using the following conventions:
* Map should be a rectangular grid following rules stated in ``utils/config.h``. If map lines don't have equal length or limits are set in ``utils/config.h`` then a proper error is diaplyed.
* Map characters should only be ``0``, ``1``,``#``,``\n`` ,and ``*``. Any other characters are meant to be illegal. Note that carriage returns (``\r``) in MS-Windows genreated files  are considered to be illegal. Also new line feeds (``\n``) should be at the end of each line (namely grid rows).
* ``0`` represents PacMan position. There should be exactly one ``0`` in map otherwise an error is displayed. 
* ``1`` represents free cells that PacMan can walk through.
* ``#`` represents blocked cells that PacMan can not pass over them.
* ``*`` represents apples that PacMan should eat all to win.
**Note:** After you pass the map file and it is validated to be ok, then you can choose machine solves the puzzle or you control it with arrow keys. In case of machine strategy, it applies BFS algorithm to find nearest apple and proceed to next one. (It is not the best optimized way obviously). Machine moves the PacMan for you in a proper speed to be seen by eyes (change the speed in ``game/ai.c`` within ``aiMove`` function ``delay`` statement. It is set to 100 miliseconds by default).
