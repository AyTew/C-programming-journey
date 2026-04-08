#include<stdlib.h>
#include<stdio.h>
#include "hashmap_oa.h"

static void hashmap_rehash(struct HashmapOA *hashmap);

static const float LOAD_FACTOR_THRESHOLD = 0.7;

struct HashmapOA *hashmap_oa_create(int size, int (*comptr)(void*, void*), unsigned long (*hashptr)(void*)) {
    struct HashmapOA *hashmap = malloc(sizeof(struct HashmapOA));

    hashmap->size = size;
    hashmap->slots = calloc(size, sizeof(struct Slot));
    hashmap->comptr = comptr;
    hashmap->hashptr = hashptr;
    hashmap->occupied_count=0;

    printf("occupied count: %d\n", hashmap->occupied_count);

    return hashmap;
}

void hashmap_oa_set(struct HashmapOA *hashmap, void* key, void* value) {
    float load_factor = (float)(hashmap->occupied_count+1)/hashmap->size;

    if(load_factor>LOAD_FACTOR_THRESHOLD) hashmap_rehash(hashmap);

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
        hashmap->occupied_count++;
    }
}

void* hashmap_oa_get(struct HashmapOA *hashmap, void* key) {
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

void hashmap_oa_delete(struct HashmapOA *hashmap, void* key) {
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
            hashmap->occupied_count--;
            return;
        }

        index = (index+1)%size;
        counter++;
        if(counter==size) break;
    }
}

void hashmap_oa_free(struct HashmapOA *hashmap) {
    free(hashmap->slots);
    free(hashmap);
}

static void hashmap_rehash(struct HashmapOA *hashmap) {
    printf("Rehashing triggered, old size: %d\n", hashmap->size);
    int size=hashmap->size;
    hashmap->size*=2;
    struct Slot *new_slots = calloc(hashmap->size, sizeof(struct Slot));
    struct Slot *slots = hashmap->slots;
    
    for(int i=0; i<size; i++) {
        if(slots[i].status==OCCUPIED) {
            void* key = slots[i].key;
            int index = hashmap->hashptr(key)%hashmap->size;    //now it is old size*2;
            int counter=0;
            while(new_slots[index].status!=EMPTY) {
                index = (index+1)%hashmap->size;
                counter++;
                if(counter == hashmap->size) {
                    break;
                }
            }

            if(counter==hashmap->size) return;  //will not happen

            new_slots[index].key = key;
            new_slots[index].val = slots[i].val;
            new_slots[index].status = OCCUPIED;
        }
    }
    hashmap->slots = new_slots;
    printf("Rehashing concluded, new size: %d, \n", hashmap->size);
    free(slots);
}
