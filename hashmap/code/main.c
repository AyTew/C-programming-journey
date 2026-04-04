#include<stdlib.h>
#include<stdio.h>
#include"hashmap.h"
#include<string.h>

unsigned long string_hash(void *key);

int string_compare(void *a, void *b);

int main() {
    struct Hashmap *hashmap_string = hashmap_create(10, string_compare, string_hash);

    hashmap_set(hashmap_string, "abc", "def");
    char *val = hashmap_get(hashmap_string, "abc");

    printf("ans: %s", val);
    return 0;
}

unsigned long string_hash(void *key) {
    unsigned long hash = 5381;
    char *k = (char *)key;
    while(*k) {
        hash = hash*33 + *k++;
    }

    return hash;
}

int string_compare(void *a, void *b) {
    return strcmp((char *)a, (char *)b)==0;
}
