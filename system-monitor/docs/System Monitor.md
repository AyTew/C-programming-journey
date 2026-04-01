# C Programming System Monitor - Daily Learning Notes

## Overview

Building a system resource monitor in C using hands-on, challenge-response learning method.

---

## Day 1 - June 22, 2025

### 🎯 Goal

Create basic "Hello World" program and learn to read system information on macOS.

### Session Summary

✅ **Complete beginner to working system monitor in one session!**

**Technical Skills Gained:**

- C program structure and compilation process
- File I/O with `popen()` and `fgets()`
- String parsing with `strchr()`
- **Fundamental pointer concepts** - biggest breakthrough!
- System programming on macOS

**Key Debugging Moments:**

- Fixed compilation errors (missing headers, semicolons)
- Corrected function syntax (`fgets()` parameters)
- Solved pointer vs character comparison issues
- Understanding why strings work with pointers

**Next Session Preview:**

- Challenge #5: Add user-friendly labels
- Learn about arrays of strings
- More advanced string manipulation
- Introduction to conditional statements

### 📝 Concepts Learned

#### Basic C Program Structure

```c
#include <stdio.h>

int main() {
    printf("Hello, System Monitor!");
    return 0;
}
```

**Key Points:**

- `#include <stdio.h>` - Header file for input/output functions
- `int main()` - Entry point of every C program
- `printf()` - Function to print to terminal
- `return 0;` - Indicates successful program execution
- **Every statement ends with semicolon `;`**

#### Common Beginner Mistakes ❌

- Forgetting `#include <stdio.h>`
- Missing semicolons after statements
- Not returning a value from main()

#### Compilation Process

```bash
gcc system_monitor.c -o system_monitor
./system_monitor
```

### 🔧 Challenge Progress

#### Challenge 1: ✅ Complete

**Task:** Write basic "Hello, System Monitor!" program **Status:** Successfully compiled and ran

#### Challenge 2: ✅ Complete

**Task:** Use `popen()` to run `vm_stat` command and capture output

**Final Working Code:**

```c
#include <stdio.h>

int main() {
    printf("Hello, System Monitor!\n");
    FILE *stat = popen("vm_stat", "r");
    char buffer[256];
    fgets(buffer, 256, stat);
    printf("%s", buffer);
    return 0;
}
```

### 📚 Function Explanations

#### `printf()`

**Purpose:** Print formatted text to terminal **Syntax:** `printf("format string", arguments...)` **Examples:**

- `printf("Hello World");` - Simple text
- `printf("Hello, %s!\n", name);` - With variable
- `printf("%d apples\n", count);` - With number

**Format specifiers:**

- `%s` - String
- `%d` - Integer
- `%f` - Float
- `\n` - Newline character

#### `popen()`

**Purpose:** Execute a shell command and return a file pointer to read its output **Syntax:** `FILE *popen(const char *command, const char *mode)` **Parameters:**

- `command` - The shell command to run (e.g., "vm_stat")
- `mode` - How to open it ("r" for read, "w" for write) **Returns:** `FILE *` pointer to read the command's output **Example:** `FILE *fp = popen("ls -l", "r");`

#### `fgets()`

**Purpose:** Read a line from a file (including from command output) **Syntax:** `char *fgets(char *buffer, int size, FILE *stream)` **Parameters:**

- `buffer` - Where to store the data (character array)
- `size` - Maximum number of characters to read (including null terminator)
- `stream` - Where to read from (FILE pointer) **Returns:** Pointer to buffer on success, NULL on error/EOF **Example:** `fgets(line, 256, file);`

> [!important] Key Behavior **`fgets()` reads ONLY ONE LINE at a time** (stops at newline `\n`)
> 
> - From that one line, it takes up to `size` characters maximum
> - **Includes the `\n` character in the buffer**
> - Each call to `fgets()` moves to the next line automatically
> - To read multiple lines, you need multiple `fgets()` calls
> 
> **File Position Pointer:** The `FILE *` maintains an internal position pointer that automatically advances after each read. You never get the same line twice!

**Why these parameters?**

- `buffer` - We need somewhere to put the data
- `size` - Prevents buffer overflow (safety)
- `stream` - Tells it which file/command to read from

#### Variable Declarations

```c
FILE *stat;      // Pointer to a file
char buffer[256]; // Array of 256 characters
```

**Key Concept:** `FILE *` is a pointer to a file structure, not the file data itself. You need to read FROM it into a buffer.

**Successful Output:**

```
./system_monitor
Hello, System Monitor!Mach Virtual Memory Statistics: (page size of 16384 bytes)
```

#### Challenge 3: ✅ Complete

**Task:** Read and display multiple lines of vm_stat output

**Final Working Code:**

```c
#include <stdio.h>

int main() {
    printf("Hello, System Monitor!\n");
    FILE *stat = popen("vm_stat", "r");
    char buffer[256];
    for(int i=0; i<5; i++){
        fgets(buffer, 256, stat);
        printf("%s", buffer);
    }
    return 0;
}
```

**Successful Output:**

```
Hello, System Monitor!
Mach Virtual Memory Statistics: (page size of 16384 bytes)
Pages free:                                4234.
Pages active:                            155635.
Pages inactive:                          154641.
Pages speculative:                          490.
```

**Key Learning:**

- `popen("vm_stat", "r")` runs terminal command `vm_stat` and captures output
- `FILE *` maintains internal position pointer that automatically advances
- Each `fgets()` call reads the next line automatically
- `fgets()` includes the `\n` character in the buffer

#### Challenge 4: ✅ Complete

**Task:** Extract just the numbers from vm_stat output

**The Problem:** Output showed colon and spaces: `: 2406.` **The Goal:** Get clean numbers: `2406.`

**Your Code Evolution:**

**Attempt 1:**

```c
strchr (buffer);  // ❌ Missing parameters
```

**Attempt 2:**

```c
char *start_index = strchr (buffer, ':');
printf("%s", buffer[start_index]);  // ❌ Wrong syntax
```

**Attempt 3:**

```c
while(*start_index == " "){  // ❌ String vs char comparison
    start_index++;
}
```

**Compiler Error:**

```
warning: comparison between pointer and integer ('char' and 'char *')
```

**Final Working Solution:**

```c
#include <stdio.h>
#include <string.h>

int main() {
    printf("Hello, System Monitor!\n");
    FILE *stat = popen("vm_stat", "r");
    char buffer[256];
    fgets(buffer, 256, stat);
    printf("%s", buffer);
    for(int i=0; i<5; i++){
        fgets(buffer, 256, stat);
        char *start_index = strchr (buffer, ':');
        start_index++;                    // Skip past the colon
        while(*start_index == ' '){       // Skip all spaces (single quotes!)
            start_index++;
        }
        printf("%s", start_index);
    }
    return 0;
}
```

**Perfect Output:**

```
Hello, System Monitor!
Mach Virtual Memory Statistics: (page size of 16384 bytes)
3993.
142877.
141708.
289.
0.
```

**Key Breakthroughs:**

- Understanding pointer arithmetic (`start_index++`)
- Character vs string literals (`' '` vs `" "`)
- Why `printf("%s", pointer)` works (strings are pointers!)
- Dereferencing with `*start_index` to get the character

#### Challenge 5: 🔄 Next Steps

**Goal:** Add user-friendly labels (e.g., "Free Memory: 3993 pages")

---

## macOS vs Linux Differences

### File System Access

- **Linux:** Uses `/proc` filesystem (`/proc/meminfo`, `/proc/stat`)
- **macOS:** Uses system commands and `sysctl` calls
- **Our Approach:** Start with `vm_stat` command via `popen()`

### Future Learning Path

- **Week 1-2:** Command parsing with `popen()`
- **Week 3-4:** Direct system calls with `sysctl()`

---

## Key C Concepts to Master

### Memory Management (Upcoming)

- [ ] Pointers and memory addresses
- [ ] `malloc()` and `free()`
- [ ] Memory leaks and debugging

### File I/O

- [x] `printf()` for output
- [ ] `fopen()` and `fclose()`
- [ ] `popen()` and `pclose()`
- [ ] `fgets()` for reading lines

### String Handling

- [ ] Character arrays vs strings
- [ ] String parsing and manipulation
- [ ] `strchr()`, `strtok()`, etc.

---

## Daily Challenges

### Template for Each Day

1. **Small Challenge** - Specific coding task
2. **Your Attempt** - Write code first
3. **Debug Together** - Fix issues and understand why
4. **Concept Explanation** - Learn the underlying principles
5. **Next Step** - Build on what was learned

---

## Resources & Commands

### Compilation

```bash
gcc filename.c -o output_name
./output_name
```

### macOS System Info Commands

- `vm_stat` - Virtual memory statistics
- `top -l 1` - System processes
- `sysctl` - System configuration

---

## Your Questions & Learning Moments

### Session 1 Questions

**Q: "So fgets just takes one line?? And from the one line it selects it takes 256 chars"** A: Exactly! `fgets()` reads ONE LINE at a time (stops at `\n`) and from that one line, takes up to 256 characters maximum.

**Q: "So how does it work. Why doesn't fgets give me the same line again and again? I never specified to go to the next line. I suspect there is some pointer play at work here"** A: Brilliant observation! The `FILE *` maintains an internal position pointer that automatically advances after each read. You never get the same line twice because the file pointer moves forward each time.

**Q: "What does popen('vm_stat', 'r'); do? What does 'r' do specifically? What is the equivalent terminal command?"** A: `popen()` runs the shell command `vm_stat` and creates a pipe to read its output. The "r" means read mode - you read the command's output into your program. Terminal equivalent is just typing `vm_stat`.

**Q: "Why did it print with the start_index variable? It is a pointer not a string or char[]"** A: In C, strings ARE pointers! `printf("%s", start_index)` works because it expects a `char *` pointer to the first character, and prints until it hits `\0`.

*_Q: "To get the value we use _start_index. Otherwise start_index is int?"__ A: No! `start_index` is a `char *` (pointer/memory address), not an int. `*start_index` gets the character AT that address.

**Q: "How do I print the actual value of that memory address?"** A: Use `%p` format specifier: `printf("Address: %p\n", start_index);`

---
## Class 2 

### 🎯 Goal
Continue System Monitor - Challenge 5 (add labels) + Networking deep dive

### Session Summary

✅ **Networking fundamentals + deep C string debugging in one session!**

**Technical Skills Gained:**
- Socket programming (UDP and TCP servers from scratch)
- File descriptors and socket API
- Packet analysis with tcpdump
- Deep understanding of null termination bugs in C
- Stack memory behavior with uninitialized arrays

**Key Debugging Moments:**
- Discovered `gets()` doesn't work with `FILE *` (stdin only)
- Caught `type[]` array missing null terminator — worked "by accident" for multi-digit numbers, crashed for single digit `0`
- Understood undefined behavior: bugs hiding due to lucky stack layout
- Traced exact byte positions using `dot`, `pos2` debug prints

**Next Session Preview:**
- Challenge 5: Add custom labels (array of strings)
- Challenge 6: Real-time updates

---

### 📝 Concepts Learned

#### Rule #1: Every C String Must End With `\0`
C has no idea where your string ends. `printf`, `strlen`, `strcpy` — all walk memory byte by byte until they hit `\0`. If it's missing, they keep going into whatever is next on the stack.

- **Library functions like `fgets`** — handle `\0` for you
- **You manually copying chars** — your responsibility to add `\0`

**The bug pattern:**
```c
char val[dot-pos2];       // ❌ No room for \0
char val[dot-pos2+1];     // ✅ +1 for \0
val[dot-pos2] = '\0';     // ✅ Always null terminate manually copied strings
```

#### Undefined Behavior
When null terminator is missing, the program doesn't always crash — sometimes there happens to be a `\0` nearby on the stack. This is "working by accident." The bug is always there, just not always visible.

Rule: **never trust luck in C.**

#### `gets()` vs `fgets()`
- `gets(buffer)` — reads from `stdin` only, no `FILE *` support
- `fgets(buffer, size, stream)` — works with any `FILE *` including `stdin`, `popen()` output, file handles

#### `stdin`, `stdout`, `stderr`
All three are just `FILE *` pointers, already open and ready to use.
```c
fgets(buffer, sizeof(buffer), stdin);   // reads keyboard input
fgets(buffer, sizeof(buffer), pointer); // reads from popen()
```

#### Pointer Arithmetic for String Parsing
```c
int dot = strchr(buffer, '.') - buffer;  // index of '.'
int pos2 = dot;
while(buffer[pos2] != ' ') pos2--;       // walk backwards to space
pos2++;                                   // move past the space
```

---

### 🔧 Challenge Progress

#### Challenge 5: 🔄 In Progress
**Task:** Add user-friendly labels to vm_stat output

**Current Output:**
```
Pages free: 4228
Pages active: 88354
Pages inactive: 85168
Pages speculative: 2286
Pages throttled: 0
```

**Target Output:**
```
Free Memory: 4228 pages
Active Memory: 88354 pages
Inactive Memory: 85168 pages
Speculative Memory: 2286 pages
Throttled Memory: 0 pages
```

**Current Working Code:**
```c
#include<stdio.h>
#include<string.h>

int main() {
    FILE *pointer = popen("vm_stat", "r");
    char buffer[1024];
    fgets(buffer, 1024, pointer);
    for(int i=0; i<5; i++) {
        fgets(buffer, 1024, pointer);
        char *pos = strchr(buffer, ':');
        size_t type_size = pos-buffer;
        char type[type_size+2];
        type[type_size+1] = '\0';
        for(int j=0; j<=type_size; j++) {
            type[j] = buffer[j];
        }
        int dot = strchr(buffer, '.')-buffer;
        int pos2=dot;
        while(buffer[pos2]!=' ') {
            pos2--;
        }
        pos2++;
        char val[dot-pos2+1];
        for(int j=pos2; j<dot; j++){
            val[j-pos2]=buffer[j];
        }
        val[dot-pos2] = '\0';
        printf("%s %s\n", type, val);
    }
}
```

**Remaining:** Replace raw vm_stat labels with custom labels using array of strings + add "pages" suffix

---

### 🌐 Networking Deep Dive

#### UDP Echo Server
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sockaddr_in server_addr;
const int BUFFER_SIZE = 1024;

int main() {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));

    while(1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char data[BUFFER_SIZE];

        int bytes = recvfrom(sock, data, BUFFER_SIZE, 0,
                            (struct sockaddr*)&client_addr, &client_len);

        printf("Received %d bytes from %s:%d\n",
               bytes,
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));

        sendto(sock, data, bytes, 0,
              (struct sockaddr*)&client_addr, client_len);
    }
    return 0;
}
```

#### TCP Echo Server
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

struct sockaddr_in server_addr;
const int BUFFER_SIZE = 1024;

int main() {
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(sock, 5);

    while(1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char data[BUFFER_SIZE];

        int client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_len);
        int bytes = recv(client_sock, data, BUFFER_SIZE, 0);
        printf("Received %d bytes: %s\n", bytes, data);

        send(client_sock, data, bytes, 0);
        close(client_sock);
    }
    return 0;
}
```

#### Key Networking Concepts
- **File descriptors:** 0=stdin, 1=stdout, 2=stderr, 3+=sockets
- **sockaddr vs sockaddr_in:** Generic vs IPv4-specific address structures
- **UDP:** Connectionless, unreliable, fast — 8-byte header. Use `recvfrom`/`sendto`
- **TCP:** Connection-based, reliable, ordered — 20-60 byte header. Use `recv`/`send`
- **TCP 3-way handshake:** SYN → SYN-ACK → ACK
- **Ephemeral ports:** Random client ports assigned by OS
- **`htons()`/`ntohs()`:** Convert between host and network byte order

#### tcpdump for Packet Analysis
```bash
sudo tcpdump -i lo0 udp port 8080   # Capture UDP on loopback
sudo tcpdump -i lo0 tcp port 8080   # Capture TCP on loopback
```

---

### Your Questions & Learning Moments

**Q: "So fgets automatically adds a null when it encounters a newline?"**
A: Yes! `fgets` always null terminates. But when YOU manually copy characters into a new array, null termination is your responsibility.

**Q: "And it doesn't automatically assume so?"**
A: Nope. C assumes nothing. Ever. You are responsible for every byte. That's the whole philosophy — the language does exactly what you tell it, nothing more.

**Q: "Why no issue with other numbers but throttled breaks?"**
A: Undefined behavior. For multi-digit numbers, there happened to be a `\0` nearby on the stack by luck. For `0` (single digit), the stack layout was different and garbage showed up. The bug was always there — just not always visible.

---

## Class 3 - 2026-03-31

### 🎯 Goal
Complete System Monitor Month 1 — Challenges 5, 6, 7 + stretch goal

### Session Summary

✅ **System Monitor Month 1 COMPLETE!**

**Technical Skills Gained:**
- Arrays of strings (`char *arr[]` vs `char arr[][N]`)
- Custom labels with loop counter indexing
- Pages → KB → MB → GB unit conversion
- Real-time updates with `while(1)`, `sleep()`, `system("clear")`
- `popen`/`pclose` resource management
- Progress bars with Unicode block characters
- Functions in C with `static` local variables
- Forward declarations (function before `main`)
- Unicode byte encoding (`\xe2\x96\x88` = █)

**Key Debugging Moments:**
- `gets()` only works with stdin — not `FILE *`
- Missing null terminator on `type[]` — worked by accident for multi-digit numbers, crashed for single digit `0`
- `return bar` inside for loop — returned on first iteration
- Integer division trap: `(int)val/max*10` always = 0, fixed with `(int)(10*val)/max`
- Dangling pointer from returning local array — fixed with `static`
- `strlen` lies about Unicode strings — counts bytes not visible characters

**Known Issues to Fix Later (Month 3):**
- Progress bar breaks column alignment — Unicode characters are 3 bytes each but render as 1 character, so `%*s` padding gets confused
- `strlen` on Unicode strings returns byte count, not visible character count

**Next Session:**
- Month 2: Hashmap from scratch
- Two implementations: chaining + open addressing
- First real taste of `malloc` and `free`

---

### 📝 Concepts Learned

#### `char *arr[]` vs `char arr[][N]`
```c
char *keys[5] = {"Free", "Active", ...};   // array of pointers to string literals
                                            // read-only, each string exact size
                                            // 5 pointers × 8 bytes = 40 bytes total

char keys[5][1024] = {"Free", "Active"};   // 2D array, every slot gets 1024 bytes
                                           // modifiable, expensive
                                           // 5 × 1024 = 5120 bytes total
```

Rule: hardcoded strings that never change → `char *arr[]`. Strings you build or modify → `char arr[][N]`.

#### Functions in C
```c
// Forward declaration (if function is defined after main)
char *progress_bar(double val, int max);

int main() {
    char *bar = progress_bar(mem_kb, total_mem);
}

char *progress_bar(double val, int max) {
    // implementation
}
```

#### `static` local variables
```c
char *progress_bar(double val, int max) {
    static char bar[33];  // lives for entire program lifetime, not just this call
    // ...
    return bar;           // safe to return — memory won't disappear
}
```

Without `static`: local array lives on the stack, disappears when function returns. Returning it gives a dangling pointer — undefined behavior.

With `static`: array lives in the data segment for the entire program. Safe to return. Tradeoff: only one copy exists — not safe if called from multiple threads (Month 3+ concern).

#### Unicode block characters
```c
'\xe2', '\x96', '\x88'   // █ filled block (3 bytes)
'\xe2', '\x96', '\x91'   // ░ empty block  (3 bytes)
```

Each Unicode character = multiple bytes in UTF-8. Terminal renders 3 bytes as 1 visible character. `strlen` counts bytes, not visible characters — so `strlen` on a 10-block bar returns 30, not 10.

#### Integer division trap
```c
int fill = (int)val/max*10;    // ❌ evaluates left-to-right
                                // (int)val / max = 0 (fraction truncated)
                                // 0 * 10 = 0. Always zero.

int fill = (int)(val*10.0/max); // ✅ multiply first, then divide
```

Rule: in C, integer division truncates immediately. Always think about order of operations when mixing multiplication and division.

#### `popen` / `pclose` resource management
```c
FILE *pointer = popen("vm_stat", "r");  // opens pipe, allocates FILE struct
// ... use pointer ...
pclose(pointer);                         // frees FILE struct, closes pipe
```

Without `pclose` in a loop: new FILE struct created every iteration, never freed. Eventually hits OS file descriptor limit (~1024) and crashes with "too many open files."

OS reclaims everything on process exit (Ctrl+C) — but for long-running programs, leaks accumulate. Build the habit now.

#### Pages → MB/GB conversion (macOS vm_stat)
```c
// 1 page = 16384 bytes (printed in vm_stat header)
double mem_kb = (double)pages * 16384 / 1024;
double mem_mb = mem_kb / 1024;
double mem_gb = mem_mb / 1024;  // only if mem_mb >= 1024
```

---

### 🔧 Challenge Progress

#### Challenge 5: ✅ Complete — Custom labels
```c
char *keys[5] = {"Free Memory", "Active Memory", "Inactive Memory", 
                 "Speculative Memory", "Throttled Memory"};
// index with loop counter i
printf("%s: %s\n", keys[i], val);
```

#### Challenge 6: ✅ Complete — Real-time updates
```c
while(1) {
    // read and display
    pclose(pointer);
    sleep(2);
    system("clear");
}
```

#### Challenge 7: ✅ Complete — Clean formatting + GB conversion + progress bars

**Final Working Code:**
```c
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>

char *progress_bar(double val, int max) {
    static char bar[33];
    bar[0]='[';
    bar[31]=']';

    int fill = (int)(10*val)/max;
    for(int i=1; i<=30; i=i+3) {
        if(i<=fill) {
            bar[i] = '\xe2';
            bar[i+1] = '\x96';
            bar[i+2] = '\x88';
        } else {
            bar[i] = '\xe2';
            bar[i+1] = '\x96';
            bar[i+2] = '\x91';
        }
    }
    bar[32]='\0';
    return bar;
}

int main() {
    const int memory_conversion_factor = 1024;
    char buffer[1024];
    char *keys[5] = {"Free Memory", "Active Memory", "Inactive Memory", 
                     "Speculative Memory", "Throttled Memory"};
    char *headers[3] = {
        "╔══════════════════════════════════╗",
        "║      SYSTEM MONITOR v1.0         ║",
        "╚══════════════════════════════════╝"
    };

    while(1){
        for(int i=0; i<3; i++) printf("%s\n", headers[i]);

        FILE *pointer = popen("vm_stat", "r");
        fgets(buffer, 1024, pointer);

        for(int i=0; i<5; i++) {
            fgets(buffer, 1024, pointer);
            int dot = strchr(buffer, '.') - buffer;

            int pos2 = dot;
            while(buffer[pos2] != ' ') pos2--;
            pos2++;

            char val[dot-pos2+1];
            for(int j=pos2; j<dot; j++) val[j-pos2] = buffer[j];
            val[dot-pos2] = '\0';

            double mem_kb = (double)atoi(val) * 16384 / memory_conversion_factor;
            double mem_mb = mem_kb / memory_conversion_factor;
            double mem_gb = 0.0;
            if(mem_mb / memory_conversion_factor >= 1) mem_gb = mem_mb / 1024;

            int total_mem = 8 * memory_conversion_factor * memory_conversion_factor;
            char *bar = progress_bar(mem_kb, total_mem);

            if(mem_gb > 0)
                printf("%s: %.3f GB %s\n", keys[i], mem_gb, bar);
            else
                printf("%s: %.3f MB %s\n", keys[i], mem_mb, bar);
        }
        pclose(pointer);
        sleep(2);
        int clear_status = system("clear");
        if(clear_status == -1) {
            printf("ERROR: system call for clear failed\n");
            break;
        }
    }
}
```

---

### Your Questions & Learning Moments

**Q: "We do not do refactoring in C, right? Like creating a different function for this"**
A: We absolutely do! Functions are core C. Breaking code into functions is especially important in C because main gets messy fast.

**Q: "Does the function need to be inside main?"**
A: No. Functions are defined outside main. If defined after main, add a forward declaration at the top.

**Q: "So fgets automatically adds a null — so stdin is also a FILE *?"**
A: Yes! stdin, stdout, stderr are all just FILE * pointers, already open. fgets(buffer, size, stdin) reads from keyboard.

**Q: "1 page = 16384 bytes?"**
A: Yes. vm_stat prints this in its header line: "page size of 16384 bytes". We hardcoded it for now — Month 3 we'll parse it dynamically.

**Q: "We will fix the Unicode alignment eventually, right?"**
A: Yes — Month 3 when we revisit the system monitor. By then the fix will make sense at a deeper level.

---
