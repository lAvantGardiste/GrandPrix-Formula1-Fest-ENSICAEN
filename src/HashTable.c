#include "../include/HashTable.h"
#include <stdlib.h>
#include <stdio.h>

// Hash function utilizing graph node coordinates and circuit parameters
int hash(GraphNode* item, TerrainMap map) {
    return (31 * item->coordinates.x + 37 * item->coordinates.y + 41 * map.width + 43 * map.height) % HASH_CAPACITY;
}

// Initialize a new hash table
HashTable* createHashTable() {
    HashTable* table = (HashTable*)malloc(sizeof(HashTable));
    for (int i = 0; i < HASH_CAPACITY; i++) {
        table->array[i] = NULL;
    }
    return table;
}

// Insert a graph node into the hash table
void insertGraphNode(HashTable* table, GraphNode* item, TerrainMap map) {
    int index = hash(item, map);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    newNode->data = item;
    newNode->next = table->array[index];
    table->array[index] = newNode;
}

// Search for a graph node in the hash table
GraphNode* searchGraphNode(HashTable* table, GraphNode* item, TerrainMap map) {
    int index = hash(item, map);
    HashNode* current = table->array[index];

    while (current != NULL) {
        if (nodesEqual(current->data, item)) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

// Remove a graph node from the hash table
void removeGraphNode(HashTable* table, GraphNode* item, TerrainMap map) {
    int index = hash(item, map);
    HashNode* current = table->array[index];
    HashNode* previous = NULL;

    while (current != NULL) {
        if (nodesEqual(current->data, item)) {
            if (previous == NULL) {
                table->array[index] = current->next;
            } else {
                previous->next = current->next;
            }
            free(current);
            return;
        }
        previous = current;
        current = current->next;
    }
}

// Free the hash table and all contained nodes
void destroyHashTable(HashTable* table) {
    for (int i = 0; i < HASH_CAPACITY; i++) {
        HashNode* current = table->array[i];
        while (current != NULL) {
            HashNode* next = current->next;
            free(current);  // Consider whether to free the data (GraphNode) if owned by the table
            current = next;
        }
    }
    free(table);
}

int isGraphNodeInHashTable(HashTable* table, GraphNode* item, TerrainMap map) {
    return searchGraphNode(table, item, map) != NULL;
}
