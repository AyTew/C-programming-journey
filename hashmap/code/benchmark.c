#include <time.h>
#include "hashmap_oa.h"
#include "hashmap.h"
#include "hashmap_helpers.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
    struct HashmapOA *hashmap_oa = hashmap_oa_create(2000000, int_compare, int_hash);
    struct Hashmap *hashmap = hashmap_create(2000000, int_compare, int_hash);

    clock_t start = clock();
    for(int i=0; i<1000000; i++) {
        int key = rand();

        hashmap_set(hashmap, &key, &key);
    }

    clock_t end = clock();

    double chaining_set_duration = (double)(end - start) / CLOCKS_PER_SEC;

    start = clock();
    for(int i=0; i<10000; i++) {
        int key = rand();
        hashmap_oa_set(hashmap_oa, &key, &key);
    }
    end = clock();
    double open_addressing_set_duration = (double)(end - start) / CLOCKS_PER_SEC;

    printf("Duration for setting 1,000,000 random keys:\n");
    printf("Chaining: %f\n", chaining_set_duration);
    printf("Open Addressing: %f\n", open_addressing_set_duration);

    hashmap_free(hashmap);
    hashmap_oa_free(hashmap_oa);
}
