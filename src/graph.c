#include "../include/graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


TerrainMap initializeTerrainMap(int width, int height) {
    TerrainMap map;
    map.width = width;
    map.height = height;
    map.grid = (char**)calloc(height, sizeof(char*));
    for (int i = 0; i < height; i++) {
        map.grid[i] = (char*)calloc(width, sizeof(char));
    }
    return map;
}

void displayTerrainMap(TerrainMap map) {
    for (int i = 0; i < map.height; i++) {
        fwrite(map.grid[i], sizeof(char), map.width, stderr);
        fprintf(stderr, "\n");
    }
}

GraphNode* createGraphNode(Vector2D coordinates, Vector2D velocity, int fuel, int turbo, GraphNode* from) {
    GraphNode* node = (GraphNode*)calloc(1, sizeof(GraphNode));
    node->coordinates = coordinates;
    node->velocity = velocity;
    node->fuel = fuel;
    node->turbo = turbo;
    node->predecessor = from;
    node->totalCost = node->cost = node->heuristicCost = 0;
    return node;
}

double calculateHeuristic(GraphNode* from, GraphNode* to) {
    double dx = fabs((double)(from->coordinates.x - to->coordinates.x));
    double dy = fabs((double)(from->coordinates.y - to->coordinates.y));
    return dx+dy;
}


int checkOccupancy(Vector2D pos, Vector2D second, Vector2D third) {
    return (pos.x == second.x && pos.y == second.y) || (pos.x == third.x && pos.y == third.y);
}



int nodesEqual(GraphNode* first, GraphNode* second) {
    return first->coordinates.x == second->coordinates.x && first->coordinates.y == second->coordinates.y;
}

void freeTerrainMap(TerrainMap map) {
    for (int i = 0; i < map.height; i++) {
        free(map.grid[i]);
    }
    free(map.grid);
}

void freeGraphNode(GraphNode* node) {
    free(node);
}



void updateNode(GraphNode *node, Vector2D position, int gasLevel, Vector2D speed) {
    node->coordinates = position;
    node->fuel = gasLevel;
    node->velocity = speed;
}
