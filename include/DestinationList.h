#ifndef _DESTINATIONLIST_H
#define _DESTINATIONLIST_H

#include "graph.h"

typedef struct Destination {
    Vector2D coordinates;
    double estimatedDistance;
} Destination;

typedef struct DestinationList {
    Destination* entries;
    int count;
} DestinationList;

DestinationList generateDestinationList(TerrainMap map, GraphNode* start);

DestinationList orderDestinations(DestinationList destinations);

void showDestinationList(DestinationList destinations);

GraphNode* selectOptimalDestination(DestinationList destinations, Vector2D second, Vector2D third);

void showOptimalDestination(GraphNode* destination);

int compareDestinations(const void* a, const void* b);

void freeDestinationList(DestinationList destinations);




#endif // _DESTINATIONLIST_H