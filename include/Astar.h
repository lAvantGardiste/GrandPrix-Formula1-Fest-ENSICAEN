#ifndef ASTAR_H
#define ASTAR_H

#include "graph.h"
#include "Queue.h"
#include "List.h"
#include "follow_line.h"
#include "HashTable.h"
#include "DestinationList.h"

int gasConsumption(Vector2D acc,Vector2D speed, int inSand);

double Norme(Vector2D vect);

int collisionDetection(Vector2D start, Vector2D end, TerrainMap circuit, Vector2D secondPosition, Vector2D thirdPosition);

int OutofBoundries(Vector2D vect, TerrainMap circuit);

double calulateDistance(TerrainMap map,Vector2D start,Vector2D end);

GraphNode* createSuccessor(Vector2D newPosition, Vector2D newSpeed, GraphNode* currentNode, int newGas, TerrainMap circuit, GraphNode* end);

GraphList* returnPath(GraphNode* currentNode);

void initializeStartNode(GraphNode* start, int gasLevel, Vector2D currentSpeed,TerrainMap circuit, GraphNode* end);


Vector2D getAcceleration(GraphList* path, Vector2D myPosition, Vector2D mySpeed, TerrainMap circuit);

int reachableNode(Vector2D newPos, TerrainMap circuit, Vector2D currentPos, Vector2D acc);

GraphList* aStar(GraphNode* start, GraphNode* end, TerrainMap circuit, Vector2D secondPoisition, Vector2D thirdPosition, int gasLevel,
			Vector2D currentSpeed, int speedMax, int occupied,int round);

int isPilotAhead(Vector2D myPosition, Vector2D secondPosition, Vector2D thirdPosition,GraphNode* end);

GraphList* run_a_star(GraphNode* start, GraphNode* end, TerrainMap circuit, Vector2D secondPosition, Vector2D thirdPosition, int gasLevel, Vector2D speed, int vitesse, int flag, int round,GraphList* path);

GraphList* find_path(DestinationList endList, GraphNode* start, TerrainMap circuit, Vector2D myPosition,Vector2D secondPosition, Vector2D thirdPosition, int gasLevel, Vector2D speed, int flag, int round, GraphList* path,GraphNode* end);

#endif // ASTAR_H