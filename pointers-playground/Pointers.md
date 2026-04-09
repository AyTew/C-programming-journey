# Pointer Dojo

### 🎯 Goal
Pointer dojo — deep dive into pointer arithmetic, double pointers, dangling pointers

### Session Summary

✅ **Pointer fundamentals locked in!**

**Technical Skills Gained:**
- Pointer arithmetic — scaling by type size automatically
- Negative indexing — `p[-2]` is valid C
- Pre vs post increment with pointers
- Double pointers — pointer to pointer
- Dangling pointers — witnessed live with stack corruption
- Raw address subtraction vs pointer subtraction
- Struct pointer arithmetic
- XOR swap trick

---

### 📝 Concepts Learned

#### `&` and `*` — The Two Operators

`&` — address-of. Gives you the address of a variable:
```c
int x = 42;
&x        // memory address where x lives, e.g. 0x1000
```

`*` — two completely different roles:
```c
// Role 1: Declaration — this variable IS a pointer
int *p;       // p holds an address

// Role 2: Dereference — go to the address, give me the value
*p            // follow p's address, return value there
```

They are opposites:
```c
*(&x) == x    // always true
```

#### Pointer Arithmetic Scales by Type
```c
int arr[] = {10, 20, 30, 40, 50};
int *p = arr;
p++;          // moves forward by sizeof(int) = 4 bytes, not 1
```

```c
int *q = arr + 3;

q - p         // → 3  (element count — pointer arithmetic)
(int)q - (int)p  // → 12 (byte count — raw address subtraction)
```

Same distance, different units. Pointer arithmetic automatically scales by `sizeof(type)`. Raw integer subtraction doesn't.

#### Negative Indexing
```c
int arr[] = {10, 20, 30, 40, 50};
int *p = arr + 2;  // points to 30

p[-2]   // *(p - 2) = 10 ✅ perfectly legal
p[-1]   // *(p - 1) = 20 ✅
```

`p[n]` is always `*(p + n)`. Negative n is valid as long as you stay in bounds.

#### Pre vs Post Increment
```c
int arr[] = {10, 20, 30, 40, 50};
int *p = arr;

*(p++)   // dereference first, then increment → 10, p now at arr[1]
*(++p)   // increment first, then dereference → p jumps to arr[2], gives 30
```

#### Double Pointers
```c
int x = 42;
int *p = &x;    // p holds address of x
int **pp = &p;  // pp holds address of p

*pp    // gives you p
**pp   // gives you x

**pp = 100;  // same as *p = 100, same as x = 100
```

Visual:
```
pp → [ p ] → [ x = 42 ]
```

Three ways to change x:
```c
x = 100;      // direct
*p = 100;     // one level of indirection
**pp = 100;   // two levels of indirection
```

Reading types right-to-left:
```c
int*   p;   // pointer to int
int**  pp;  // pointer to pointer to int
int*** ppp; // pointer to pointer to pointer to int
```

#### The `int* a, b` Trap
```c
int* a, b;   // looks like two pointers — WRONG!
             // a is int*, b is plain int
```
`*` binds to the variable, not the type. Always write `int *a, *b` if you want two pointers.

#### Dangling Pointers — Witnessed Live
```c
int *foo() {
    int x = 42;
    return &x;   // ❌ returning address of stack memory!
}
```

`x` lives on the stack. Stack frame dies when function returns. Pointer now points to dead memory.

**Witnessed in action:**
```c
int *foo() { int x = 42; return &x; }

void bar() { int a=1; int b=2; int c=3; char str[]="hello"; }

int main() {
    int *p = foo();
    bar();              // overwrites foo's old stack frame
    printf("%d\n", *p); // prints 2 — bar's variable b, not 42!
}
```

Not a crash — a **silent wrong answer**. The worst kind of bug.

Fix:
```c
// Option 1 — heap (outlives the function)
int *foo() {
    int *x = malloc(sizeof(int));
    *x = 42;
    return x;  // ✅
}

// Option 2 — caller provides buffer
void foo(int *out) {
    *out = 42;  // ✅
}
```

#### Struct Pointer Arithmetic
```c
struct Point { int x; int y; };
struct Point points[] = {{1,2}, {3,4}, {5,6}};
struct Point *p = points;

p++;           // moves forward by sizeof(struct Point), lands on {3,4}
p->x           // 3
p->y           // 4

&p->y - &p->x  // → 1 (x and y are 1 int apart inside the struct)
```

#### XOR Swap — No Temp Variable
```c
void swap(int *a, int *b) {
    *a = *a ^ *b;
    *b = *a ^ *b;
    *a = *a ^ *b;
}
```
XOR property: `a ^ a = 0`, `a ^ 0 = a`. So `a ^ b ^ b = a`.
Swaps two integers without a temporary variable. Classic C trick. Pointers make the swap affect the original variables.

---

### 🏆 Pointer Dojo Challenges Solved

| Challenge | Concept | Result |
|-----------|---------|--------|
| `p++`, `p[-2]` | Pointer arithmetic + negative index | ✅ |
| `*(p++)` vs `*(++p)` | Pre vs post increment | ✅ |
| `**pp = 100` | Double pointers | ✅ |
| Dangling pointer | Stack corruption witnessed live | ✅ |
| `q-p` vs `(int)q-(int)p` | Pointer vs raw subtraction | ✅ |
| `&p->y - &p->x` | Struct pointer arithmetic | ✅ |
| XOR swap | Bitwise + pointer combo | ✅ |

---

### Your Questions & Learning Moments

**Q: "Does `p[-2]` work?"**
A: Yes! `p[-2]` = `*(p-2)`. Negative indexing is valid as long as you stay in bounds.

**Q: "Is `int**` an array of pointers?"**
A: Technically pointer-to-pointer. Behaves like array of pointers when combined with `calloc` + indexing. Same reason `int *p = arr` works — contiguous memory + pointer = array behavior.

**Q: "How do I see the dangling pointer bug?"**
A: Call another function after the dangling pointer is created. Its stack frame overwrites the dead memory. Witnessed `bar()`'s `b=2` overwriting `foo()`'s `x=42`.

**Q: "I've always ignored bitwise operators"**
A: XOR is the most useful one in systems programming. Rule: same bits cancel (`a^a=0`), different bits survive. Property: `a^b^b=a` — XORing twice undoes itself.

**Q: "It makes more sense to write `int*` instead of `int *`"**
A: Both identical to compiler. `int*` reads better for types. But beware: `int* a, b` — only `a` is a pointer! `*` binds to variable not type. Know the trap, pick your style.

---

### Next Session
- Start Project 3: Network Packet Analyzer
- Add network section to System Monitor
- Raw sockets, packet capture, protocol parsing
- Start fresh context window!
