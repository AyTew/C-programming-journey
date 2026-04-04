## Class 1 - 2026-04-03

### 🎯 Goal
Build a hashmap from scratch in C — chaining implementation

### Session Summary

✅ **Hashmap core implementation complete!**

**Technical Skills Gained:**
- `malloc` and `free` — heap memory management
- Structs on heap vs stack
- `->` operator for struct pointers
- `calloc` vs `malloc` — zeroed vs uninitialized memory
- Double pointers `struct Node **` — array of pointers
- Hash function implementation
- Chaining for collision handling
- Linked list deletion with `prev` pointer tracking
- Header files — splitting into `.h`, `.c`, `main.c`

**Key Debugging Moments:**
- Allocating `Hashmap` struct but forgetting to allocate the `nodes` array separately
- `while(!temp)` vs `while(temp)` — flipped condition in `hashmap_get`
- `hashmap->size--` in `hashmap_set` — size is bucket count, never changes
- Forgetting to set `node->key` and `node->value` after `malloc`
- `node->next` uninitialized — always set to `NULL` after malloc

**Next Session:**
- `hashmap_free` — free all memory properly
- String keys — `char *` instead of `int`
- Open addressing — second implementation, no linked lists

---

### 📝 Concepts Learned

#### Stack vs Heap (memory regions)
```c
// Stack — automatic, dies when function returns
int x = 5;
char buf[1024];
struct Person p;

// Heap — manual, lives until free()
int *x = malloc(sizeof(int));
struct Person *p = malloc(sizeof(struct Person));
free(x);
free(p);
```

**Stack:** Fast, automatic, limited (~8MB). Every local variable lives here. Dies when function returns.
**Heap:** Large, manual, flexible. Lives until YOU call `free()`. Both freed by OS on program exit.

#### `malloc` vs `calloc`
```c
// malloc — allocates memory, contents uninitialized (garbage)
struct Node **buckets = malloc(sizeof(struct Node*) * size);

// calloc — allocates AND zeroes memory
struct Node **buckets = calloc(size, sizeof(struct Node*));
```

Use `calloc` for arrays of pointers — you want them all `NULL` by default, not garbage.

#### `->` operator
```c
struct Person *p = malloc(sizeof(struct Person));

p->name = "Ayush";      // same as (*p).name
p->age = 28;            // same as (*p).age
```

`->` = dereference pointer + access field. Only for struct pointers. For plain types use `*`.

#### `char name[50]` vs `char *name`
```c
struct Person {
    char name[50];   // IS the memory block — can't reassign, use strcpy
    char *name;      // POINTS TO a memory block — can reassign freely
};

// char name[50]
strcpy(p.name, "Ayush");   // ✅
p.name = "Ayush";          // ❌ array not assignable

// char *name
p->name = "Ayush";         // ✅
```

#### Double pointers `struct Node **`
```c
struct Node **buckets;  // pointer to array of Node pointers
```
```
buckets (Node **)  →  [Node *][Node *][Node *]...[Node *]
                          ↓        ↓
                        Node     Node
                          ↓
                        Node
                          ↓
                        NULL
```

Each bucket is a `Node *` pointing to the head of a linked list.

#### Returning heap pointers from functions
```c
struct Hashmap *hashmap_create(int size) {
    struct Hashmap *hashmap = malloc(sizeof(struct Hashmap));
    // ...
    return hashmap;  // ✅ safe — heap memory outlives the function
}
```

Stack memory dies when function returns. Heap memory lives until `free()`. Returning a heap pointer is always safe.

#### Header files
```
hashmap.h   ← struct definitions + forward declarations
hashmap.c   ← function implementations (#include "hashmap.h")
main.c      ← main() (#include "hashmap.h")
```
```c
#include "hashmap.h"   // local file — look in current directory
#include <stdio.h>     // system file — look in system includes
```

---

### 🔧 Implementation

#### Hash function
```c
int hashing(int key, int size) {
    return key % size;
}
```

#### `hashmap_create`
```c
struct Hashmap *hashmap_create(int size) {
    struct Hashmap *hashmap = malloc(sizeof(struct Hashmap));
    hashmap->size = size;
    hashmap->nodes = calloc(size, sizeof(struct Node*));
    return hashmap;
}
```

#### `hashmap_set`
```c
void hashmap_set(struct Hashmap *hashmap, int key, int value) {
    int index = hashing(key, hashmap->size);
    struct Node *node = malloc(sizeof(struct Node));

    node->key = key;
    node->value = value;
    node->next = NULL;

    if(!hashmap->nodes[index]) {
        hashmap->nodes[index] = node;
    } else {
        struct Node *temp = hashmap->nodes[index];
        hashmap->nodes[index] = node;
        node->next = temp;
    }
}
```

#### `hashmap_get`
```c
int hashmap_get(struct Hashmap *hashmap, int key) {
    int index = hashing(key, hashmap->size);
    struct Node *temp = hashmap->nodes[index];

    while(temp) {
        if(temp->key == key) return temp->value;
        temp = temp->next;
    }

    return -1e9;
}
```

#### `hashmap_delete`
```c
void hashmap_delete(struct Hashmap *hashmap, int key) {
    int index = hashing(key, hashmap->size);
    struct Node *temp = hashmap->nodes[index];
    if(!temp) return;

    struct Node *prev = NULL;
    while(temp) {
        if(temp->key == key) {
            if(!prev)
                hashmap->nodes[index] = temp->next;
            else
                prev->next = temp->next;
            free(temp);
            return;
        }
        prev = temp;
        temp = temp->next;
    }
}
```

---

### Your Questions & Learning Moments

**Q: "We do not do refactoring in C, right? Like creating a different function?"**
A: We absolutely do! Functions are core C. Breaking code into functions is especially important in C because main gets messy fast.

**Q: "So p[0] = *p = value in memory?"**
A: Yes! `p[0]` = `*(p+0)` = `*p`. Array indexing is just pointer arithmetic. That's why arrays and pointers are so closely related in C.

**Q: "Pointers are confusing"**
A: They are at first. But you now understand: `int *` → pointer to int, `struct Node *` → pointer to struct, `struct Node **` → pointer to array of pointers. The confusion means your brain is building new wiring.

**Q: "Structs returned from functions — why no dangling pointer?"**
A: Because the struct lives on the heap (malloc'd). Stack memory dies with the function. Heap memory lives until free(). Returning a heap pointer is always safe.

**Q: "hashmap seems simpler than I thought"**
A: That feeling is growth. The REAL complexity is ahead — string keys, resizing, open addressing, memory leak detection, benchmarks. You built the foundation. 🥋

**Q: "Will learning C genuinely help me become a better engineer?"**
A: For getting a job quickly — inefficient. For becoming a genuinely better engineer — absolutely not a waste. Engineers who understand what the machine is doing underneath debug faster, design better, make fewer fundamental mistakes. In the age of LLMs, vibe coding raises the floor. The ceiling belongs to people who understand what the generated code actually does.

---
## Class 2 - 2026-04-04

### 🎯 Goal
Complete the chaining hashmap — `hashmap_free` + make it fully generic with `void *`

### Session Summary

✅ **Generic hashmap complete!**

**Technical Skills Gained:**
- `hashmap_free` — freeing linked lists correctly (save next before freeing!)
- `void *` — generic pointers in C
- Function pointers — storing and calling functions through a struct field
- Callbacks pattern — caller teaches the hashmap how to compare and hash
- djb2 hash function — hashing string contents, not pointer addresses
- `strcmp` for string comparison — why `==` fails for strings
- Casting `void *` back to concrete types inside callbacks

---

### 📝 Concepts Learned

#### Why write `hashmap_free` if OS cleans up anyway?
- Long-running programs (servers) accumulate leaks and eventually crash
- Memory sanitizers (Valgrind) will flag leaks even if OS cleans up
- Discipline — whoever `malloc`s is responsible for `free`ing
- You'll embed this hashmap in your database and interpreter later — those are long-running

#### `void *` — The Generic Pointer
```c
void *p;        // points to anything — compiler doesn't know what

int x = 42;
void *p = &x;   // ✅ any pointer assignable to void *

*p;             // ❌ can't dereference — compiler doesn't know the size
*(int *)p;      // ✅ cast first, then dereference
```

`void *` is C's only generic mechanism. No templates, no generics — just "trust me, I know the type."

#### Why `==` fails for strings
```c
// char * stores the address of the first character
// Two identical strings can live at different addresses

hashmap_set(map, "hello", val);   // "hello" at address 0x1000
hashmap_get(map, "hello");        // "hello" at address 0x2000

0x1000 == 0x2000  // ❌ false — same contents, different addresses
```

Use `strcmp()` — walks both strings byte by byte, compares contents.

#### Why hashing `void *` by address is wrong for strings
```c
return (size_t)key % size;  // hashes the ADDRESS, not the contents
```
Same string literal at different addresses → different hash → wrong bucket. Always hash the **contents** for strings.

#### djb2 — hash function for strings
```c
unsigned long string_hash(void *key) {
    unsigned long hash = 5381;
    char *k = (char *)key;
    while(*k) {
        hash = hash * 33 + *k++;
    }
    return hash;
}
```
Magic number `5381` and multiplier `33` are empirically chosen for good distribution. Walks characters until `\0`.

#### Function pointers
```c
// declaring a function pointer field in a struct
int (*comptr)(void*, void*);            // pointer to function taking 2 void*, returning int
unsigned long (*hashptr)(void*);        // pointer to function taking void*, returning unsigned long

// assigning
hashmap->comptr = string_compare;       // just the name — already an address
hashmap->comptr = &string_compare;      // ❌ pointer to pointer — wrong

// calling
hashmap->comptr(temp->key, key);        // call it like a normal function
```

#### Callbacks pattern
The hashmap doesn't know the key type. The caller does. So the caller provides the behavior:
```c
// caller writes these
unsigned long string_hash(void *key) { ... }
int string_compare(void *a, void *b) { return strcmp((char*)a, (char*)b) == 0; }

// caller teaches the hashmap
hashmap_create(10, string_compare, string_hash);
```
This is how `qsort` in the standard library works too. Same pattern everywhere in C.

#### Struct with function pointers ≈ OOP class
A struct holding both data and function pointers is essentially a class — data + behavior together. This is literally how C++ was first implemented under the hood.

---

### 🔧 Final Implementation

#### `hashmap.h`
```c
#include <stdlib.h>

struct Node {
    void *key;
    void *value;
    struct Node *next;
};

struct Hashmap {
    int size;
    struct Node **nodes;
    int (*comptr)(void*, void*);
    unsigned long (*hashptr)(void*);
};

struct Hashmap *hashmap_create(int size, int (*comptr)(void*, void*), unsigned long (*hashptr)(void*));
void hashmap_set(struct Hashmap *hashmap, void *key, void *value);
void *hashmap_get(struct Hashmap *hashmap, void *key);
void hashmap_delete(struct Hashmap *hashmap, void *key);
void hashmap_free(struct Hashmap *hashmap);
```

#### `hashmap_free`
```c
void hashmap_free(struct Hashmap *hashmap) {
    int size = hashmap->size;
    struct Node **buckets = hashmap->nodes;
    for(int i = 0; i < size; i++) {
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
```
Order matters: free each node → free buckets array → free hashmap struct.

#### `main.c` — string usage
```c
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "hashmap.h"

unsigned long string_hash(void *key) {
    unsigned long hash = 5381;
    char *k = (char *)key;
    while(*k) {
        hash = hash * 33 + *k++;
    }
    return hash;
}

int string_compare(void *a, void *b) {
    return strcmp((char *)a, (char *)b) == 0;
}

int main() {
    struct Hashmap *map = hashmap_create(10, string_compare, string_hash);
    hashmap_set(map, "abc", "def");
    char *val = hashmap_get(map, "abc");
    printf("ans: %s\n", val);
    return 0;
}
```

---

### Your Questions & Learning Moments

**Q: "When the program terminates, won't memory be freed by the system?"**
A: Yes! OS reclaims everything on exit. But long-running programs, sanitizers, and discipline all demand we free manually. The OS is your safety net — a craftsman doesn't rely on the safety net.

**Q: "Can we not do generics like C++ templates in C?"**
A: No templates in C. C's answer is function pointers — cruder, more explicit, and you see exactly what's happening. C++ templates are just a fancy way of generating this same machinery automatically.

**Q: "`hashmap->comptr = comptr` or `&comptr`?"**
A: Just `comptr`. A function name is already an address. `&comptr` gives you a pointer to the pointer — wrong type entirely.

**Q: "Shouldn't the hash function take size as a parameter too?"**
A: No — the hashmap owns the `% size` step. If the hashmap ever resizes (rehashing), the caller's hash function stays unchanged. Clean boundary of responsibility.

**Q: "We are gonna implement resizing also then?"**
A: Yes! That's called dynamic resizing / rehashing. Coming after open addressing. 🔥

---

### Next Session
- Helper functions for common types (int hash, int compare, etc.)
- Open addressing — second hashmap implementation, no linked lists
- Dynamic resizing / rehashing
---
