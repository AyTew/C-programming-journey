#include<stdlib.h>
#include<stdio.h>
#include"hashmap_oa.h"
#include"hashmap_helpers.h"
#include<string.h>

unsigned long string_hash(void *key);

int string_compare(void *a, void *b);

int main() {
    struct HashmapOA *hashmap_int = hashmap_create(20000, int_compare, int_hash);

    hashmap_set(hashmap_string, "abc", "def");
    hashmap_set(hashmap_string, "yoyo", "Honey Singh");

    hashmap_free(hashmap_string);
    return 0;
}
