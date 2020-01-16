#include "core.h"
#include "queue.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int* distances; // store min distance to that node from origin
    int* path; // stores father of each node
    int origin; // origin linear format index
} BFSResult;
typedef BFSResult* BFSResultPtr;

static BFSResultPtr BFSResults; // stores all bfs results
static int** Graph; // for each node in map grid stores adjacent nodes

char aiMove (const char* MapGrid, Position PacManLocation, int PacManApples, int MapApples, int MapRows, int MapColumns) {}

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
static BFSResultPtr bfs (int** graph, int gridRows, int gridColumns, int origin) {
    // Search queue for bfs algorithm
    Queue searchQueue = {NULL, NULL};

    // allocate memory for result
    BFSResultPtr result = malloc(sizeof(BFSResult));

    // curent vertex linear index for iteration
    int curVertix;

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
            }
        } // end for
    } // end while

    // free memory allocated by visited
    free(visited);

    return result;
}

/* Find shortest paths and routes */
void aiSetup (const char* MapGrid, Position PacManLocation, int MapApples, int MapRows, int MapColumns) {
    // find pac man location linear index from position
    int PacManLocationIndex = toLinearGen(PacManLocation, MapColumns);

    // setup graph from grid
    Graph = GridToGraph(MapGrid, MapRows, MapColumns);

    /*
    // test graph
    FILE * stream = fopen("grah.txt", "w");
    for (int i = 0; i < MapRows * MapColumns; i++) {
        if (Graph[i] != NULL)
        fprintf(stream, "index %d : adjacents %d %d %d %d\n", i, Graph[i][0], Graph[i][1], Graph[i][2], Graph[i][3]);
    }
    fclose(stream);
    // test bfs
    int now;
    FILE * stream = fopen("grah.txt", "w");
    for (int i = 0; i < MapRows * MapColumns; i++) { 
        if (!isBlock(toPosGen(i, MapColumns))) {       
            fprintf(stream, "Distance of (%d, %d) from origin is %d\nPath: ", toPosGen(i, MapColumns).x, toPosGen(i, MapColumns).y, BFSResults[0].distances[i]);
            now = i;
            while (now != PacManLocationIndex) {
                fprintf(stream, "(%d, %d) ", toPosGen(now, MapColumns).x, toPosGen(now, MapColumns).y);
                now = BFSResults[0].path[now];
            }
            fprintf(stream, "\n");
        }
    }
    fclose(stream);      
    */

    // allocate memory for bfs results
    //BFSResults = (BFSResultPtr) malloc(sizeof(BFSResult) * (MapApples + 1));

    // call bfs for origin as pac man location
    //BFSResults[0] = bfs(Graph, MapRows, MapColumns, PacManLocationIndex);
    
}