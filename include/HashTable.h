#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "graph.h" // Include the CustomGraph header
#define HASH_CAPACITY 1000

typedef struct HashNode {
    GraphNode* data;  // Using GraphNode instead of Node
    struct HashNode* next;
} HashNode;

typedef struct {
    HashNode* array [HASH_CAPACITY];
} HashTable;

int hash(GraphNode* item, TerrainMap Terrain);
HashTable* createHashTable();
void insertGraphNode(HashTable* table, GraphNode* item, TerrainMap Terrain);
GraphNode* searchGraphNode(HashTable* table, GraphNode* item, TerrainMap Terrain);
void removeGraphNode(HashTable* table, GraphNode* item, TerrainMap Terrain);
void destroyHashTable(HashTable* table);
int isGraphNodeInHashTable(HashTable* table, GraphNode* item, TerrainMap Terrain);

#endif // HASH_TABLE_H
