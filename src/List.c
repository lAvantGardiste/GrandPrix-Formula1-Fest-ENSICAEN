#include "../include/List.h"
#include <stdlib.h>

// Create a new graph list
GraphList *createGraphList() {
    GraphList *list = (GraphList *) malloc(sizeof(GraphList));
    if (list) {
        list->head = NULL;
    }
    return list;
}

// Add a graph node to the list
void addGraphNodeToList(GraphList *list, GraphNode *node) {
    GraphNodeList *newNode = (GraphNodeList *) malloc(sizeof(GraphNodeList));
    if (newNode) {
        newNode->node = node;
        newNode->next = list->head;
        list->head = newNode;
    }
}

// Check if a graph node is in the list
int isGraphNodeInList(GraphList *list, GraphNode *node) {
    GraphNodeList *current = list->head;
    while (current) {
        if (nodesEqual(current->node, node)) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

// Free the entire graph list
void freeGraphList(GraphList *list) {
    GraphNodeList *current = list->head;
    GraphNodeList *next;
    while (current) {
        next = current->next;
        free(current);
        current = next;
    }
    free(list);
}

// Reversing the list as a simple form of sorting, depending on your requirements
void sortGraphList(GraphList *list) {
    GraphNodeList *prev = NULL, *current = list->head, *next = NULL;
    while (current!= NULL) {
        next = current->next;
        current->next = prev;
        prev = current;
        current = next;
    }
    list->head = prev;
}
