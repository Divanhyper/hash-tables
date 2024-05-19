#ifndef __HASHMAP_H__
#define __HASHMAP_H__

#include <stdbool.h>
#include <stddef.h>
#include "string.h"

typedef struct hash_map HashMap;

typedef int32_t HashMapKey;

typedef String HashMapValue;

typedef struct hash_map_pair {
    HashMapKey key;
    HashMapValue value;
} HashMapPair;

// Create hash map data structure with a table of given capacity.
// capacity must be greater that 0
// Return NULL if memory allocation failed.
HashMap* hash_map_create_with(const size_t capacity);

// Create hash map data structure with default capacity.
// Return NULL if memory allocation failed.
HashMap* hash_map_create(void);

// Destroy hash map data structure: free memory and set underlying pointer to NULL.
void hash_map_destroy(HashMap** const map);

// Find value in map by key. Return address of existing item, NULL - otherwise.
HashMapValue* hash_map_find(const HashMap* const map, const HashMapKey key);

// Insert value to map by key. Return true if insertion was successful and
// such key-value pair was _unique_, false - otherwise.
bool hash_map_insert(HashMap* const map,
                     const HashMapKey key,
                     const HashMapValue value);

// Insert value to map by key, if item did not exist, otherwise replace value by key.
// Return true if such operation was successful, false - otherwise.
bool hash_map_insert_or_assign(HashMap* const map,
                               const HashMapKey key,
                               const HashMapValue value);

// Remove item by key from map. Return true if item existed, false - otherwise.
bool hash_map_remove(HashMap* const map, const HashMapKey key);

// Check if value exist by key in map.
bool hash_map_contains(const HashMap* const map, const HashMapKey key);

// Clear all items in map and set capacity back to default size.
void hash_map_clear(HashMap* const map);

// Get current item count in map.
size_t hash_map_size(const HashMap* const map);

// Get current maximum capacity in map.
size_t hash_map_capacity(const HashMap* const map);

// Check if none of the items are stored in map.
bool hash_map_empty(const HashMap* const map);

// Calculate current ratio of size to capacity.
double hash_map_load_factor(const HashMap* const map);

// Return first valid occurrence of key-value pair in hashmap, otherwise - NULL
HashMapPair* hash_map_first(const HashMap* const map);

// Return last valid occurence of key-value pair in hash map, otherwise - NULL
HashMapPair* hash_map_last(const HashMap* const map);

// Return next valud occurence of key-value pair after current one, otherwise - NULL
HashMapPair* hash_map_next_pair(const HashMap* const map, const HashMapPair* const pair);

// Example of iterating over all existing elements of hashmap:
//
// HashMap* map;
//
// ...
//
// for (HashMapPair* it = hash_map_first(map);
//      it != NULL;
//      it = hash_map_next_pair(map, it)) {
//     printf("Key: %d, value: %s\n", it->key, it->value);
// }

#endif // __HASHMAP_H__
