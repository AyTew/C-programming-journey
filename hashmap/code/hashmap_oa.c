#include<stdlib.h>
#include "hashmap_oa.h"

struct HashmapOA *hashmap_create(int size, int (*comptr)(void*, void*), unsigned long (*hashptr)(void*)) {
    struct HashmapOA *hashmap = malloc(sizeof(struct HashmapOA));

    hashmap->size = size;
    hashmap->slots = calloc(size, sizeof(struct Slot));
    hashmap->comptr = comptr;
    hashmap->hashptr = hashptr;

    return hashmap;
}

void hashmap_set(struct HashmapOA *hashmap, void* key, void* value) {
    int size = hashmap->size;

    int index = hashmap->hashptr(key)%size;
    int counter=0;
    while(hashmap->slots[index].status==OCCUPIED) {
        index = (index+1)%size;
        counter++;
        if(counter==size) {
            break;
        }
    }

    if(counter!=size) {
        struct Slot *slot = &hashmap->slots[index];
        slot->key = key;
        slot->val = value;
        slot->status = OCCUPIED;
    }
}

void* hashmap_get(struct HashmapOA *hashmap, void* key) {
    int size=hashmap->size;
    
    int index = hashmap->hashptr(key) % size;
    
    if(hashmap->slots[index].status==EMPTY) return NULL;
    
    int counter = 0;
    
    while(hashmap->slots[index].status!=EMPTY) {
        
        if(hashmap->slots[index].status==DELETED) {
            index=(index+1)%size;
            counter++;
            continue;
        }else if(hashmap->comptr(hashmap->slots[index].key, key)) {
            return hashmap->slots[index].val;
        }

        index=(index+1)%size;
        counter++;
        if(counter==size) break;
    }

    return NULL;
}

void hashmap_delete(struct HashmapOA *hashmap, void* key) {
    int size = hashmap->size;

    int index = hashmap->hashptr(key) % size;

    int counter=0;

    while(hashmap->slots[index].status!=EMPTY) {
        if(hashmap->slots[index].status==DELETED) {
            index = (index+1)%size;
            counter++;
            continue;
        } else if(hashmap->comptr(hashmap->slots[index].key, key)) {
            hashmap->slots[index].status = DELETED;
            return;
        }

        index = (index+1)%size;
        counter++;
        if(counter==size) break;
    }
}

void hashmap_free(struct HashmapOA *hashmap) {
    free(hashmap->slots);
    free(hashmap);
}