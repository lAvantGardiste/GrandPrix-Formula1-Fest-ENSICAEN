#ifndef LIST_H
#define LIST_H

#include "graph.h" // Assuming graph.h defines GraphNode and related types

typedef struct GraphNodeList {
    GraphNode *node;
    struct GraphNodeList *next;
} GraphNodeList;

typedef struct GraphList {
    GraphNodeList *head;
} GraphList;

GraphList *createGraphList();
void addGraphNodeToList(GraphList *list, GraphNode *node);
int isGraphNodeInList(GraphList *list, GraphNode *node);
void freeGraphList(GraphList *list);
void sortGraphList(GraphList *list); // Additional function for sorting, if necessary

#endif // LIST_H
