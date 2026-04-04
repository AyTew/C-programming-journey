#include<stdlib.h>

struct Node {
    void* key;
    void* value;
    struct Node *next;
};

struct Hashmap {
    int size;
    struct Node **nodes;
    int (*comptr)(void*, void*);
    unsigned long (*hashptr)(void*);
};

struct Hashmap *hashmap_create(int size, int (*comptr)(void*, void*), unsigned long (*hashptr)(void*));

void hashmap_set(struct Hashmap *hashmap, void* key, void* value);

void* hashmap_get(struct Hashmap *hashmap, void* key);

void hashmap_delete(struct Hashmap *hashmap, void* key);

void hashmap_free(struct Hashmap *hashmap);
