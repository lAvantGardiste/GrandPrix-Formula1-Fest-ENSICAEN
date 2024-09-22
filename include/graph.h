#ifndef GRAPH_H
#define GRAPH_H

#define MAX_NEIGHBOURS 9

typedef struct Vector2D {
    int x;
    int y;
} Vector2D;

typedef struct TerrainMap {
    char** grid;
    int width;
    int height;
} TerrainMap;

typedef struct GraphNode {
    Vector2D coordinates;
    Vector2D velocity;
    double cost, heuristicCost, totalCost;
    struct GraphNode* predecessor;
    int fuel;
    int turbo;
} GraphNode;

TerrainMap initializeTerrainMap(int width, int height);
void displayTerrainMap(TerrainMap map);

GraphNode* createGraphNode(Vector2D coordinates, Vector2D velocity, int fuel, int turbo, GraphNode* from);

double calculateHeuristic(GraphNode* from, GraphNode* to);

int checkOccupancy(Vector2D pos, Vector2D second, Vector2D third);

int nodesEqual(GraphNode* first, GraphNode* second);

void freeTerrainMap(TerrainMap map);
void freeGraphNode(GraphNode* node);


void updateNode(GraphNode *node, Vector2D position, int gasLevel, Vector2D speed);


#endif /* GRAPH_H */
