#include "./core.h"
#include "../ds/queue.h"
#include "../ds/stack.h"
#include "../utils/gui.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct {
    int* distances; // store min distance to that node from origin
    int* path; // stores father of each node
    int origin; // origin linear format index
} BFSResult;
typedef BFSResult* BFSResultPtr;

static int** Graph; // for each node in map grid stores adjacent nodes
static Queue Moves = {NULL, NULL};

/* Convert Grid to Graph */
static int** GridToGraph (const char* grid, int gridRows, int gridColumns) {
    // init position and next position
    Position p, next_p;

    // Allocate memory for graph
    int** graph = (int**) malloc(sizeof(int*) * gridRows * gridColumns);

    // for each node in grid
    for (int i = 0; i < gridRows * gridColumns; i++) {
        // evaluate position format
        p = toPosGen(i, gridColumns);

        // check not a block
        if (isBlock(p)) {
            graph[i] = NULL;
            continue;
        }

        // allocate memory for adjacent nodes (obviously each node degree is at most 4)
        graph[i] = (int*) malloc(sizeof(int) * 4);

        // check next adjacent position not block or grid frame wall
        if (canMove(next_p = nextPosition(p, 'u')))
            graph[i][0] = toLinearGen(next_p, gridColumns);
        else
            graph[i][0] = -1;
        if (canMove(next_p = nextPosition(p, 'd')))
            graph[i][1] = toLinearGen(next_p, gridColumns);
        else
             graph[i][1] = -1;
        if (canMove(next_p = nextPosition(p, 'l')))
            graph[i][2] = toLinearGen(next_p, gridColumns); 
        else
             graph[i][2] = -1;
        if (canMove(next_p = nextPosition(p, 'r')))
            graph[i][3] = toLinearGen(next_p, gridColumns);
        else
            graph[i][3] = -1;                  
    }
    return graph;
}

/* BFS Algorithm */
static int bfs (int** graph, int gridRows, int gridColumns, int origin, BFSResultPtr* resultPtr) {
    // Search queue for bfs algorithm
    Queue searchQueue = {NULL, NULL};

    // allocate memory for result
    BFSResultPtr result = malloc(sizeof(BFSResult));

    // curent vertex linear index for iteration
    int curVertix;

    // finding nearest apple
    int minDistance = gridColumns * gridRows, nextAppleIndex = -1;

    // allocated memory for visited nodes and initialize it to zero
    int* visited = (int*) malloc(sizeof(int) * gridRows * gridColumns);
    for (int i = 0; i < gridColumns * gridRows; i++)
        visited[i] = 0;
    visited[origin] = 1;

    // allocate memory for distances and path of BFSResult (initialize distances[origin] to zero)
    result->distances = (int*) malloc(sizeof(int) * gridRows * gridColumns);   
    result->path = (int*) malloc(sizeof(int) * gridRows * gridColumns);
    result->distances[origin] = 0;

    // enqueue origin root
    enQueue(&searchQueue, origin);

    // traverse tree
    while (!isQueueEmpty(searchQueue)) {
        curVertix = deQueue(&searchQueue);

        // check adjacent nodes and add not visited to queue
        for (int i = 0; i < 4; i++) {
            if (graph[curVertix][i] != -1 && visited[graph[curVertix][i]] == 0) {// if vertix exists (or equivalently not block or wall) and not visited
                result->distances[graph[curVertix][i]] = result->distances[curVertix] + 1; // update distance
                result->path[graph[curVertix][i]] = curVertix; // update father
                visited[graph[curVertix][i]] = 1; // mark as visited
                enQueue(&searchQueue, graph[curVertix][i]);
                if (isApple(toPosGen(graph[curVertix][i], gridColumns))) {                                      
                    if (result->distances[graph[curVertix][i]] < minDistance) {                        
                        minDistance = result->distances[graph[curVertix][i]];
                        nextAppleIndex = graph[curVertix][i];
                    }
                }
            }
        } // end for
    } // end while

    // free memory allocated by visited
    free(visited);

    // save results
    *resultPtr = result;

    // return next apple index   
    return nextAppleIndex;
}

/* Find shortest paths and routes */
int aiSetup (const char* MapGrid, Position PacManLocation, int MapApples, int MapRows, int MapColumns) {
    // find pac man location linear index from position
    int PacManLocationIndex = toLinearGen(PacManLocation, MapColumns);

    // setup graph from grid
    Graph = GridToGraph(MapGrid, MapRows, MapColumns);

    // apples locations
    int* apples = malloc(sizeof(int) * MapApples);

    // call bfs consequently for shortest paths
    StackNodePtr tempStack = NULL;
    BFSResultPtr bfs_result_ptr;
    int source, destination, current;
    source = PacManLocationIndex;
    for (int i = 0; i < MapApples; i++) {
        current = destination = bfs(Graph, MapRows, MapColumns, source, &bfs_result_ptr);

        // check if pacman can move
        if (destination == -1)
            return -1;

        // add apple index
        apples[i] = destination;

        // eat apple
        setMap(toPosGen(destination, MapColumns), '1'); 

        // finding path
        while (current != source) {
            pushStack(&tempStack, current);
            current = bfs_result_ptr->path[current];
        }

        // moving for next iteration
        source = destination;

        // finding exact path
        while (!isStackEmpty(tempStack)) {
            current = popStack(&tempStack);
            enQueue(&Moves, current);
        }

        // deleting bfs allocated memory
        free(bfs_result_ptr->distances);
        free(bfs_result_ptr->path);
        free(bfs_result_ptr);
    }

    // delete graph memory
    for (int i = 0; i < MapColumns * MapRows; i++)
        free(Graph[i]);
    free(Graph);

    // recover apples
    for (int i = 0; i < MapApples; i++)
        setMap(toPosGen(apples[i], MapColumns), '*'); 
    free(apples);
}

char aiMove (Position PacManLocation, int MapColumns) {
    Position next = toPosGen(deQueue(&Moves), MapColumns);
    delay(100, 0);
    if (nextPosition(PacManLocation, 'u').x == next.x && nextPosition(PacManLocation, 'u').y == next.y) {
        return 'u';
    } else if (nextPosition(PacManLocation, 'd').x == next.x && nextPosition(PacManLocation, 'd').y == next.y) {
        return 'd';
    } else if (nextPosition(PacManLocation, 'l').x == next.x && nextPosition(PacManLocation, 'l').y == next.y) {
        return 'l';
    } else if (nextPosition(PacManLocation, 'r').x == next.x && nextPosition(PacManLocation, 'r').y == next.y) {
        return 'r';
    }
}
