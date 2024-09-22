#ifndef QUEUE_H
#define QUEUE_H

#include "graph.h"  // Assume this contains the definition for GraphNode

typedef struct GraphNodeQueue {
    GraphNode *node;
    struct GraphNodeQueue *next;
} GraphNodeQueue;

typedef struct GraphQueue {
    GraphNodeQueue *head;
} GraphQueue;

GraphQueue *createGraphQueue();
void enqueueGraph(GraphQueue *queue, GraphNode *node);
GraphNode *dequeueGraph(GraphQueue *queue);
int isGraphQueueEmpty(GraphQueue *queue);
void freeGraphQueue(GraphQueue *queue);
int isGraphNodeInQueue(GraphQueue *queue, GraphNode *node);
GraphNode *getGraphNodeFromQueue(GraphQueue *queue, GraphNode *node);
void removeGraphNodeFromQueue(GraphQueue *queue, GraphNode *node);
int findGraphNodeInQueue(GraphQueue *queue, GraphNode *node);

#endif // QUEUE_H
