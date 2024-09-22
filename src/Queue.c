#include "../include/Queue.h"
#include <stdlib.h>
#include <stdio.h>

GraphQueue *createGraphQueue() {
    GraphQueue *queue = (GraphQueue *)malloc(sizeof(GraphQueue));
    if (queue) {
        queue->head = NULL;
    }
    return queue;
}

void enqueueGraph(GraphQueue *queue, GraphNode *node) {
    GraphNodeQueue *newNode = (GraphNodeQueue *)malloc(sizeof(GraphNodeQueue));
    if (newNode) {
        newNode->node = node;
        newNode->next = NULL;
        if (queue->head == NULL || queue->head->node->totalCost > node->totalCost) {
            newNode->next = queue->head;
            queue->head = newNode;
        } else {
            GraphNodeQueue *current = queue->head;
            while (current->next != NULL && current->next->node->totalCost < node->totalCost) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }
}

GraphNode *dequeueGraph(GraphQueue *queue) {
    if (queue->head == NULL) {
        return NULL;
    }
    GraphNodeQueue *node = queue->head;
    queue->head = queue->head->next;
    GraphNode *result = node->node;
    free(node);
    return result;
}

int isGraphQueueEmpty(GraphQueue *queue) {
    return queue->head == NULL;
}

void freeGraphQueue(GraphQueue *queue) {
    while (queue->head != NULL) {
        GraphNodeQueue *node = queue->head;
        queue->head = queue->head->next;
        free(node);
    }
    free(queue);
}

int isGraphNodeInQueue(GraphQueue *queue, GraphNode *node) {
    GraphNodeQueue *current = queue->head;
    while (current) {
        if (nodesEqual(current->node, node)) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}

GraphNode *getGraphNodeFromQueue(GraphQueue *queue, GraphNode *node) {
    GraphNodeQueue *current = queue->head;
    while (current) {
        if (nodesEqual(current->node, node)) {
            return current->node;
        }
        current = current->next;
    }
    return NULL;
}

void removeGraphNodeFromQueue(GraphQueue *queue, GraphNode *node) {
    GraphNodeQueue *current = queue->head;
    GraphNodeQueue *prev = NULL;
    while (current) {
        if (nodesEqual(current->node, node)) {
            if (prev == NULL) {
                queue->head = current->next;
            } else {
                prev->next = current->next;
            }
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

int findGraphNodeInQueue(GraphQueue *queue, GraphNode *node) {
    GraphNodeQueue *current = queue->head;
    while (current) {
        if (nodesEqual(current->node, node)) {
            return 1;
        }
        current = current->next;
    }
    return 0;
}
