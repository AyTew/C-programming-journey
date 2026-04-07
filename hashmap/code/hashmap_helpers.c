#include"hashmap_helpers.h"
#include<string.h>
#include<math.h>

union {
    double d;
    unsigned long u;
} pun;

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

unsigned long int_hash(void *key) {
    return (unsigned long)*(int *)key;
}

int int_compare(void* a, void *b) {
    return *(int *)a==*(int *)b;
}

unsigned long double_hash(void *key) {
    pun.d = *(double *)key;
    
    return pun.u;
}

int double_compare(void *a, void*b) {
    return fabs(*(double *)a - *(double *)b) < 1e-9;
}
