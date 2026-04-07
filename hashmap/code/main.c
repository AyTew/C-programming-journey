#include<stdlib.h>
#include<stdio.h>
#include"hashmap_oa.h"
#include"hashmap_helpers.h"
#include<string.h>

unsigned long string_hash(void *key);

int string_compare(void *a, void *b);

int main() {
    struct HashmapOA *hashmap_string = hashmap_create(10, string_compare, string_hash);

    hashmap_set(hashmap_string, "abc", "def");

    char *val2 = hashmap_get(hashmap_string, "xyz");

    hashmap_delete(hashmap_string, "abc");
    char *val = hashmap_get(hashmap_string, "abc");

    printf("ans: %s\n", val);
    printf("ans: %s\n", val2);

    hashmap_free(hashmap_string);
    return 0;
}
