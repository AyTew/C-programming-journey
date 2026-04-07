#include<stdlib.h>

enum STATUS {EMPTY, OCCUPIED, DELETED}; //tombstoning -> calloc initializes all to 0, so need to keep EMPTY at position 0

struct Slot {
    void* key;
    void* val;
    enum STATUS status;
};

struct HashmapOA {
    int size;
    struct Slot *slots;
    int (*comptr)(void*, void*);
    unsigned long (*hashptr)(void*);
};

struct HashmapOA *hashmap_create(int size, int (*comptr)(void*, void*), unsigned long (*hashptr)(void*));

void hashmap_set(struct HashmapOA *hashmap, void* key, void* value);

void* hashmap_get(struct HashmapOA *hashmap, void* key);

void hashmap_delete(struct HashmapOA *hashmap, void* key);

void hashmap_free(struct HashmapOA *hashmap);
