#include "../include/Astar.h"
#include "../include/follow_line.h"
#include <float.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

int gasConsumption(Vector2D acc, Vector2D speed, int inSand) {
    //fprintf(stderr, "Entering gasConsumption function\n");
    int gas = acc.x * acc.x + acc.y * acc.y;
    gas += (int)(sqrt(speed.x * speed.x + speed.y * speed.y) * 3.0 / 2.0);
    if (inSand) {
        gas += 1;
    }
    //fprintf(stderr, "Exiting gasConsumption function\n");
    return -gas;
}

double Norme(Vector2D vect) {
    //fprintf(stderr, "Entering Norme function\n");
    double result = sqrt(vect.x * vect.x + vect.y * vect.y);
    //fprintf(stderr, "Exiting Norme function\n");
    return result;
}

int OutofBoundries(Vector2D vect, TerrainMap circuit) {
    //fprintf(stderr, "Entering OutofBoundries function\n");
    if (vect.x < 0 || vect.x >= circuit.width || vect.y < 0 || vect.y >= circuit.height) {
        //fprintf(stderr, "Out of bounds\n");
        return 1;
    }
    //fprintf(stderr, "Within bounds\n");
    return 0;
}

int collisionDetection(Vector2D start, Vector2D end, TerrainMap circuit, Vector2D secondPosition, Vector2D thirdPosition) {
    //fprintf(stderr, "Entering isRouteFreeFromOccupation function\n");
    InfoLine vline;
    Pos2Dint p;
    initLine(start.x, start.y, end.x, end.y, &vline);
    while (nextPoint(&vline, &p, +1) > 0) {
        Vector2D Vectp;
        Vectp.x = p.x;
        Vectp.y = p.y;
        if (p.x == start.x && p.y == start.y) {
            //fprintf(stderr, "We suppose that the start position is not worth visiting!\n");
            continue;
        }
        if (OutofBoundries(Vectp,circuit)) {
            //fprintf(stderr, "We suppose that the TerrainMap is surrounded by walls!\n");
            return 0;
        }
        if (circuit.grid[p.y][p.x] == '.') {
            //fprintf(stderr, "Obstacle detected\n");
            return 0;
        }
        if (p.y == secondPosition.y && p.x == secondPosition.x) {
            //fprintf(stderr, "Second position occupied\n");
            return 0;
        }
        if (p.y == thirdPosition.y && p.x == thirdPosition.x) {
            //fprintf(stderr, "Third position occupied\n");
            return 0;
        }
    }
    //fprintf(stderr, "Path is clear\n");
    return 1; /*Path is clear*/
}

double calulateDistance(TerrainMap map,Vector2D start,Vector2D end){
    if (map.grid[end.y][end.x]=='~'){
        return 5*sqrt((start.x-end.x)*(start.x-end.x)+(start.y-end.y)*(start.y-end.y));
    }
    return sqrt((start.x-end.x)*(start.x-end.x)+(start.y-end.y)*(start.y-end.y));
}

GraphNode* createSuccessor(Vector2D newPosition, Vector2D newSpeed, GraphNode* currentNode, int newGas, TerrainMap circuit, GraphNode* end) {
    //fprintf(stderr, "Entering createsuccessorNode function\n");
    GraphNode* successor = createGraphNode(newPosition, newSpeed, newGas, currentNode->turbo, currentNode);
    if (successor == NULL) {
        //fprintf(stderr, "Failed to create successor node\n");
        return NULL;
    }

    successor->cost = currentNode->cost + calulateDistance(circuit,currentNode->coordinates,successor->coordinates);
    successor->heuristicCost = calculateHeuristic(successor, end);
    successor->totalCost = successor->cost + successor->heuristicCost;
    //fprintf(stderr, "Exiting createsuccessorNode function\n");
    return successor;
}

int reachableNode(Vector2D newPos, TerrainMap circuit, Vector2D currentPos, Vector2D acc)
{
    //fprintf(stderr, "Entering shouldContinue function\n");
    if (newPos.x == currentPos.x && newPos.y == currentPos.y) {
        //fprintf(stderr, "Ignore the current node\n");
        return 0; /* ignore the current node */
    }
    if (OutofBoundries(newPos, circuit)) {
        //fprintf(stderr, "Out of bounds\n");
        return 0; /* out of bounds */
    }
    if (circuit.grid[newPos.y][newPos.x] == '.') {
        //fprintf(stderr, "Obstacle detected\n");
        return 0; /* obstacle */
    }
    //fprintf(stderr, "Continue with current iteration\n");
    return 1; /* continue with current iteration */
}

GraphList* returnPath(GraphNode* current) {
    //fprintf(stderr, "Entering getPath function\n");
    GraphList* path = createGraphList();
    if (path == NULL) {
        //fprintf(stderr, "Failed to create path list\n");
        return NULL;
    }
    GraphNode* temp = current;
    while (temp != NULL) {
        addGraphNodeToList(path, temp);
        temp = temp->predecessor;
    }
    //fprintf(stderr, "Exiting getPath function\n");
    return path;
}

int isPilotAhead(Vector2D myPosition, Vector2D secondPosition, Vector2D thirdPosition,GraphNode* end) {
    //fprintf(stderr, "Entering isPilotAhead function\n");
    double distance1=sqrt((end->coordinates.x - myPosition.x) * (end->coordinates.x - myPosition.x) + (end->coordinates.y - myPosition.y) * (end->coordinates.y - myPosition.y));
    double distance2=sqrt((end->coordinates.x - secondPosition.x) * (end->coordinates.x - secondPosition.x) + (end->coordinates.y - secondPosition.y) * (end->coordinates.y - secondPosition.y));
    double distance3=sqrt((end->coordinates.x - thirdPosition.x) * (end->coordinates.x - thirdPosition.x) + (end->coordinates.y - thirdPosition.y) * (end->coordinates.y - thirdPosition.y));

    if (distance1 < distance2 && distance1 < distance3) {
        //fprintf(stderr, "Pilot 1 is ahead\n");
        return 0;
    }
    return 1;
}



void initializeStartNode(GraphNode* start, int gasLevel, Vector2D currentSpeed,TerrainMap circuit, GraphNode* end) {
    start->cost = 0;
    start->heuristicCost = calculateHeuristic(start, end);
    start->totalCost = start->cost + start->heuristicCost;
    start->fuel = gasLevel;
    start->velocity = currentSpeed;

}

GraphList* aStar(GraphNode* start, GraphNode* end, TerrainMap circuit, Vector2D secondPoisition, Vector2D thirdPosition, int gasLevel,
            Vector2D currentSpeed, int speedMax, int occupied,int round) {
    //fprintf(stderr, "Entering aStar function\n");
    int accX;
    int accY;
    Vector2D newSpeed;
    Vector2D newPosition;
    int newGas;
    GraphNode* successor;
    Vector2D currentPos;
    Vector2D newPos;

    GraphQueue* openSet = createGraphQueue();
    HashTable* closedSet = createHashTable();



    initializeStartNode(start, gasLevel, currentSpeed, circuit, end);


    enqueueGraph(openSet, start);

    while (!isGraphQueueEmpty(openSet)) {
        GraphNode* currentNode = dequeueGraph(openSet);

        if (nodesEqual(currentNode, end) == 1) {
            return returnPath(currentNode);
        }

    

        insertGraphNode(closedSet, currentNode, circuit);
        
        for (accX = -1; accX <= 1; accX++) {
            for (accY = -1; accY <= 1; accY++) {
                Vector2D acc={accX,accY};
                newSpeed.x = currentNode->velocity.x + accX;
                newSpeed.y = currentNode->velocity.y + accY;

                if ((newSpeed.x * newSpeed.x) + (newSpeed.y * newSpeed.y) <= speedMax) {
                    newPosition.x = currentNode->coordinates.x + newSpeed.x;
                    newPosition.y = currentNode->coordinates.y + newSpeed.y;
                    if (reachableNode(newPosition, circuit, currentNode->coordinates, acc) == 0) {
                        continue;
                    }

                    currentPos.x = currentNode->coordinates.x;
                    currentPos.y = currentNode->coordinates.y;
                    newPos.x = newPosition.x;
                    newPos.y = newPosition.y;

                    if (occupied == 1) {
                        if ((newPosition.x == secondPoisition.x && newPosition.y == secondPoisition.y) || (newPosition.x == thirdPosition.x && newPosition.y == thirdPosition.y)) {
                            continue;
                        }
                    }

                    if (!collisionDetection(currentPos, newPos, circuit, secondPoisition, thirdPosition)) {
                        continue;
                    }

                    newGas = currentNode->fuel + gasConsumption(acc,newSpeed, circuit.grid[newPosition.y][newPosition.x] == '~');
                    if (newGas < 0) {
                        continue;
                    }

                    successor = createSuccessor(newPosition, newSpeed ,currentNode,newGas, circuit, end);
                    successor->turbo = currentNode->turbo - ((accX == 2) ? 1 : 0) - ((accY == 2) ? 1 : 0);

                    if (!isGraphNodeInHashTable(closedSet, successor,circuit)) {
                        GraphNode* existingNodeInOpenSet = getGraphNodeFromQueue(openSet, successor);
                        if (existingNodeInOpenSet == NULL || successor->cost < existingNodeInOpenSet->cost) {
                            if (existingNodeInOpenSet != NULL) {
                                removeGraphNodeFromQueue(openSet, existingNodeInOpenSet);
                            }

                               enqueueGraph(openSet, successor);
                            }
                        }
                    
                }
            }
        }
    }
    
    freeGraphQueue(openSet);
    destroyHashTable(closedSet);

    //fprintf(stderr, "Exiting aStar function due to no path found\n");
    return NULL;
}



Vector2D getAcceleration(GraphList* path, Vector2D myPosition, Vector2D mySpeed, TerrainMap circuit) {
    //fprintf(stderr, "Entering getAcceleration function\n");
    Vector2D acceleration;
    Vector2D nextPosition;
    if (path == NULL || path->head == NULL || path->head->next == NULL || path->head->next->node == NULL) {
        acceleration.x = 0;
        acceleration.y = 0;
        //fprintf(stderr, "Exiting getAcceleration function early due to NULL next node\n");
        return acceleration;
    }
    nextPosition = path->head->next->node->coordinates;

    if (myPosition.x + mySpeed.x == nextPosition.x && myPosition.y + mySpeed.y == nextPosition.y) {
        acceleration.x = 0;
        acceleration.y = 0;
    } else {
        acceleration.x = nextPosition.x - myPosition.x - mySpeed.x;
        acceleration.y = nextPosition.y - myPosition.y - mySpeed.y;
    }

    if (circuit.grid[myPosition.y][myPosition.x] == '~') {
        Vector2D NewSpeed = {mySpeed.x + acceleration.x, mySpeed.y + acceleration.y};
        if (Norme(NewSpeed) > 1) {
            acceleration.x = 0;
            acceleration.y = 0;
        }
    }
    //fprintf(stderr, "Exiting getAcceleration function\n");
    return acceleration;
}

GraphList* run_a_star(GraphNode* start, GraphNode* end, TerrainMap circuit, Vector2D secondPosition, Vector2D thirdPosition, int gasLevel, Vector2D speed, int vitesse, int flag, int round,GraphList* path) {
    path = aStar(start, end, circuit, secondPosition, thirdPosition, gasLevel, speed, vitesse, flag, round);
    while (path == NULL && vitesse > 0) {
        vitesse--;
        path = aStar(start, end, circuit, secondPosition, thirdPosition, gasLevel, speed, vitesse, flag, round);
    }
    return path;
    
}

GraphList* find_path(DestinationList endList, GraphNode* start, TerrainMap circuit,Vector2D myPosition, Vector2D secondPosition, Vector2D thirdPosition, int gasLevel, Vector2D speed, int flag, int round, GraphList* path,GraphNode* end){
    int i = 0;
    while (i < endList.count && path == NULL) {
        int vitesse=25;
        end = createGraphNode(endList.entries[i].coordinates, speed, gasLevel, 0, NULL);
        path = aStar(start, end, circuit, secondPosition, thirdPosition, gasLevel, speed, vitesse, flag, round);
        run_a_star(start, end, circuit, secondPosition, thirdPosition, gasLevel,speed, vitesse, flag, round, path);
        i++;
    }
    return path;
    
}


