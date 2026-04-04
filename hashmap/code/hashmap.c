#include<stdlib.h>
#include "hashmap.h"

struct Hashmap *hashmap_create(int size, int (*comptr)(void*, void*), unsigned long (*hashptr)(void*)) {
    struct Hashmap *hashmap = malloc(sizeof(struct Hashmap));

    hashmap->size=size;

    hashmap->nodes = calloc(size, sizeof(struct Node*));

    hashmap->comptr=comptr;

    hashmap->hashptr=hashptr;

    return hashmap;
}

void hashmap_set(struct Hashmap *hashmap, void* key, void* value) {
    int index = hashmap->hashptr(key)%hashmap->size;
    struct Node *node = malloc(sizeof(struct Node));

    node->key=key;
    node->value=value;

    if(!hashmap->nodes[index]) {
        hashmap->nodes[index] = node;
        node->next=NULL;
    } else {
        struct Node *temp;
        temp = hashmap->nodes[index];
        hashmap->nodes[index] = node;
        node->next = temp;
    }
}

void* hashmap_get(struct Hashmap *hashmap, void* key) {
    int index = hashmap->hashptr(key)%hashmap->size;

    struct Node **buckets = hashmap->nodes;
    struct Node *temp = buckets[index];
    while(temp) {
        if(hashmap->comptr(temp->key, key)) {
            return temp->value;
        }
        temp=temp->next;
    }

    return NULL;
}

void hashmap_delete(struct Hashmap *hashmap, void* key) {
    int index = hashmap->hashptr(key)%hashmap->size;

    struct Node *temp = hashmap->nodes[index];
    
    if(!temp) return;
    
    struct Node *prev = NULL;

    while(temp) {
        if(hashmap->comptr(temp->key, key)) {
            if(!prev) {
                hashmap->nodes[index] = temp->next;
            } else {
                prev->next = temp->next;
            }

            free(temp);
            return;
        }
        prev=temp;
        temp=temp->next;
    }
}

void hashmap_free(struct Hashmap *hashmap) {
    int size = hashmap->size;
    struct Node **buckets = hashmap->nodes;
    //free each and every node
    for(int i=0; i<size; i++) {
        struct Node *node = buckets[i];            
        while(node) {
            struct Node *temp = node;
            node = temp->next;
            free(temp);
        }
    }
    free(buckets);
    free(hashmap);
}
