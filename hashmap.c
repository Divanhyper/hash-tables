#include "hashmap.h"
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

// Define hash map struct
typedef struct hash_map {
    size_t capacity;
    size_t size;
    HashMapPair *pairs;
} HashMap;

// Hash function
size_t hash_map_hash(HashMap* hm, HashMapKey key) {
    return (size_t)(key % hm->capacity);
}

// Create hash map data structure with a table of given capacity.
// Capacity must be greater than 0.
// Return NULL if memory allocation failed.
HashMap* hash_map_create_with(const size_t capacity) {
    if (capacity <= 0) {
        return NULL;
    }

    HashMap* hm = (HashMap*)malloc(sizeof(HashMap));
    if (hm == NULL) {
        return NULL;
    }

    hm->capacity = capacity;
    hm->size = 0;
    hm->pairs = (HashMapPair*)calloc(capacity, sizeof(HashMapPair));
    if (hm->pairs == NULL) {
        free(hm);
        return NULL;
    }

    return hm;
}

// Create hash map data structure with default capacity.
// Return NULL if memory allocation failed.
HashMap* hash_map_create(void) {
    return hash_map_create_with(16);  // Default capacity of 16
}

// Destroy hash map data structure: free memory and set underlying pointer to NULL.
void hash_map_destroy(HashMap** const map) {
    if (map == NULL || *map == NULL) {
        return;
    }

    for (size_t i = 0; i < (*map)->capacity; i++) {
        string_deinit(&(*map)->pairs[i].value);
    }
    free((*map)->pairs);
    free(*map);
    *map = NULL;
}

// Find value in map by key. Return address of existing item, NULL - otherwise.
HashMapValue* hash_map_find(const HashMap* const map, const HashMapKey key) {
    HashMapPair* pair = &map->pairs[hash_map_hash((HashMap*)map, key)];
    for (size_t i = 0; i < map->capacity; i++) {
        if (pair->key == key && !string_empty(&pair->value)) {
            return &pair->value;
        }
        pair++;
        if (pair == map->pairs + map->capacity) {
            pair = map->pairs;
        }
    }
    return NULL;
}

// Insert value to map by key. Return true if insertion was successful and
// such key-value pair was _unique_, false - otherwise.
bool hash_map_insert(HashMap* const map, const HashMapKey key, const HashMapValue value) {
    if (hash_map_load_factor(map) >= 1) {
        return false;  // Map is full
    }

    HashMapPair* pair = &map->pairs[hash_map_hash(map, key)];
    for (size_t i = 0; i < map->capacity; i++) {
        if (string_empty(&pair->value)) {  // Found an empty slot
            pair->key = key;
            pair->value = string_create_from(&value);
            map->size++;
            return true;
        } else if (pair->key == key) {  // Key already exists
            return false;
        }
        pair++;
        if (pair == map->pairs + map->capacity) {
            pair = map->pairs;
        }
    }
    return false;  // Should not happen
}

// Insert value to map by key, if item did not exist, otherwise replace value by key.
// Return true if such operation was successful, false - otherwise.
bool hash_map_insert_or_assign(HashMap* const map, const HashMapKey key, const HashMapValue value) {
    HashMapValue* existing_value = hash_map_find(map, key);
    if (existing_value == NULL) {  // Key not found
        return hash_map_insert(map, key, value);
    } else {  // Key already exists, replace value
        string_deinit(existing_value);
        *existing_value = string_create_from(&value);
        return true;
    }
}

// Remove item by key from map. Return true if item existed, false - otherwise.
bool hash_map_remove(HashMap* const map, const HashMapKey key) {
    HashMapPair* pair = &map->pairs[hash_map_hash(map, key)];
    for (size_t i = 0; i < map->capacity; i++) {
        if (pair->key == key && !string_empty(&pair->value)) {
            string_deinit(&pair->value);
            pair->value = (String){0};
            map->size--;
            return true;
        }
        pair++;
        if (pair == map->pairs + map->capacity) {
            pair = map->pairs;
        }
    }
    return false;  // Key not found
}

// Check if value exist by key in map.
bool hash_map_contains(const HashMap* const map, const HashMapKey key) {
    return hash_map_find(map, key) != NULL;
}

// Clear all items in map and set capacity back to default size.
void hash_map_clear(HashMap* const map) {
    for (size_t i = 0; i < map->capacity; i++) {
        string_deinit(&map->pairs[i].value);
        map->pairs[i].key = 0;
        map->pairs[i].value = (String){0};
    }
    map->size = 0;
}

// Get current item count in map.
size_t hash_map_size(const HashMap* const map) {
    return map->size;
}

// Get current maximum capacity in map.
size_t hash_map_capacity(const HashMap* const map) {
    return map->capacity;
}

// Check if none of the items are stored in map.
bool hash_map_empty(const HashMap* const map) {
    return map->size == 0;
}

// Calculate current ratio of size to capacity.
double hash_map_load_factor(const HashMap* const map) {
    return (double)map->size / map->capacity;
}

// Return first valid occurrence of key-value pair in hashmap, otherwise - NULL
HashMapPair* hash_map_first(const HashMap* const map) {
    HashMapPair* pair = &map->pairs[0];
    for (size_t i = 0; i < map->capacity; i++) {
        if (!string_empty(&pair->value)) {
            return pair;
        }
        pair++;
    }
    return NULL;  // Map is empty
}

// Return last valid occurence of key-value pair in hash map, otherwise - NULL
HashMapPair* hash_map_last(const HashMap* const map) {
    HashMapPair* pair = &map->pairs[map->capacity - 1];
    for (size_t i = 0; i < map->capacity; i++) {
        if (!string_empty(&pair->value)) {
            return pair;
        }
        pair--;
    }
    return NULL;  // Map is empty
}

// Return next valid occurence of key-value pair after current one, otherwise - NULL
HashMapPair* hash_map_next_pair(const HashMap* const map, const HashMapPair* const pair) {
    if (pair == NULL) {
        return NULL;
    }

    HashMapPair* next_pair = (HashMapPair*)pair + 1;
    for (size_t i = 1; i < map->capacity; i++) {
        if (next_pair == map->pairs + map->capacity) {
            next_pair = map->pairs;
        }
        if (!string_empty(&next_pair->value)) {
            return next_pair;
        }
        next_pair++;
    }
    return NULL;  // End of map
}