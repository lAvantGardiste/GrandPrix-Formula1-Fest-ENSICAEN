#include"../include/DestinationList.h"
#include <stdio.h>
#include <stdlib.h>


DestinationList generateDestinationList(TerrainMap map, GraphNode* start) {
    DestinationList destinations;
    destinations.count = 0;
    destinations.entries = (Destination*)malloc(sizeof(Destination));
    for (int i = 0; i < map.height; i++) {
        for (int j = 0; j < map.width; j++) {
            if (map.grid[i][j] == '=') {
                destinations.count++;
                destinations.entries = (Destination*)realloc(destinations.entries, destinations.count * sizeof(Destination));
                destinations.entries[destinations.count - 1].coordinates.x = j;
                destinations.entries[destinations.count - 1].coordinates.y = i;
                Vector2D speed = {0, 0};
                destinations.entries[destinations.count - 1].estimatedDistance = calculateHeuristic(start, createGraphNode(destinations.entries[destinations.count - 1].coordinates, speed, 0, 0, NULL));
            }
        }
    }
    return destinations;
}

int compareDestinations(const void* a, const void* b) {
    Destination* destA = (Destination*)a;
    Destination* destB = (Destination*)b;
    return (destA->estimatedDistance > destB->estimatedDistance) - (destA->estimatedDistance < destB->estimatedDistance);
}

DestinationList orderDestinations(DestinationList destinations) {
    qsort(destinations.entries, destinations.count, sizeof(Destination), compareDestinations);
    return destinations;
}

void showDestinationList(DestinationList destinations) {
    for (int i = 0; i < destinations.count; i++) {
        fprintf(stderr, "Destination %d: %d %d\n", i, destinations.entries[i].coordinates.x, destinations.entries[i].coordinates.y);
    }
}

GraphNode* selectOptimalDestination(DestinationList destinations, Vector2D second, Vector2D third) {
    DestinationList sorted = orderDestinations(destinations);
    for (int i = 0; i < sorted.count; i++) {
        if (!checkOccupancy(sorted.entries[i].coordinates, second, third)) {
            Vector2D speed = {0, 0};
            return createGraphNode(sorted.entries[i].coordinates, speed, 0, 0, NULL);
        }
    }
    return NULL;
}

void showOptimalDestination(GraphNode* destination) {
    fprintf(stderr, "Optimal destination: %d %d\n", destination->coordinates.x, destination->coordinates.y);
}

void freeDestinationList(DestinationList destinations) {
    free(destinations.entries);
}