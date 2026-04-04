#include<stdio.h>
#include<stdlib.h>

struct Person {
    char *name;
    int age;
};

int main() {
    struct Person *p = malloc(sizeof(struct Person));
    
    p->name = "Ayush";
    p->age = 28;

    printf("Name: %s, age: %d\n", p[0].name, p[0].age);
    free(p);
}
