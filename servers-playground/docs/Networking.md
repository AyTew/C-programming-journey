# UDP and TCP Network Programming in C - Complete Guide

## Day 1 - Building Network Servers from Scratch

### What We Built
1. **UDP Echo Server** - Connectionless, unreliable, fast
2. **TCP Echo Server** - Connection-based, reliable, slower
3. **Packet Analysis** - Used tcpdump to see actual network packets

---

## UDP Server - Complete Code
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>

struct sockaddr_in server_addr;
const int BUFFER_SIZE = 1024;

int main() {
    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // Create UDP socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    // Bind to port 8080
    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    while(1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char data[BUFFER_SIZE];
        
        // Receive data
        int bytes = recvfrom(sock, data, BUFFER_SIZE, 0, 
                            (struct sockaddr*)&client_addr, &client_len);
        
        printf("Received %d bytes from %s:%d\n", 
               bytes,
               inet_ntoa(client_addr.sin_addr),
               ntohs(client_addr.sin_port));
        
        // Echo back
        sendto(sock, data, bytes, 0, 
              (struct sockaddr*)&client_addr, client_len);
    }
    
    return 0;
}
```

**Compile & Run:**
```bash
gcc udp.c -o udp
./udp
```

**Test:**
```bash
echo "hello" | nc -4u -w1 localhost 8080
```

---

## TCP Server - Complete Code
```c
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <unistd.h>

struct sockaddr_in server_addr;
const int BUFFER_SIZE = 1024;

int main() {
    // Setup server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // Create TCP socket
    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    // Bind to port 8080
    bind(sock, (struct sockaddr*)&server_addr, sizeof(server_addr));
    
    // Listen for connections (max 5 pending)
    listen(sock, 5);
    
    while(1) {
        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        char data[BUFFER_SIZE];
        
        // Accept connection
        int client_sock = accept(sock, (struct sockaddr*)&client_addr, &client_len);
        
        // Receive data
        int bytes = recv(client_sock, data, BUFFER_SIZE, 0);
        printf("Received %d bytes: %s\n", bytes, data);
        
        // Echo back
        send(client_sock, data, bytes, 0);
        
        // Close connection
        close(client_sock);
    }
    
    return 0;
}
```

**Compile & Run:**
```bash
gcc tcp.c -o tcp
./tcp
```

**Test:**
```bash
echo "hello" | nc localhost 8080
```

---

## Key Concepts Learned

### What is a Socket?
**A socket is an endpoint for network communication.**

Think of it like a phone jack:
- You create a socket (plug in phone)
- You bind to a port (assign a phone number)
- You send/receive data (talk on the phone)

### File Descriptors (Handlers)
Every program automatically gets:
- **0 = stdin** (keyboard input)
- **1 = stdout** (screen output)
- **2 = stderr** (error output)

First socket/file you open gets **3**, next gets **4**, etc.
```c
int sock = socket(...);  // Returns 3 (first available)
printf("%d", sock);       // Prints: 3
```

### Socket Address Structures

**Generic (any protocol):**
```c
struct sockaddr {
    // Works for any protocol
};
```

**IPv4 specific:**
```c
struct sockaddr_in {
    sa_family_t sin_family;     // AF_INET
    in_port_t sin_port;         // Port (e.g., 8080)
    struct in_addr sin_addr;    // IP address (e.g., 127.0.0.1)
};
```

**Why cast?**
```c
bind(sock, (struct sockaddr*)&server_addr, ...);
```
Functions take generic `sockaddr*` but we use specific `sockaddr_in` for IPv4.

### Client Address Discovery
```c
struct sockaddr_in client_addr;  // Empty initially!
recvfrom(..., &client_addr, ...);  // OS fills it in!
```

**The magic:** `recvfrom()` tells you WHO sent the data!
- Client's IP address
- Client's random port (ephemeral port)

Then you use that SAME address to send back:
```c
sendto(sock, data, bytes, 0, &client_addr, ...);
```

---

## UDP vs TCP - Deep Comparison

### UDP Characteristics

| Property | Description | Why |
|----------|-------------|-----|
| **No Acknowledgement** | Client sends, gets no confirmation | No ACK packets |
| **No Guaranteed Delivery** | Packets can get lost | No retransmission |
| **Connectionless** | No setup/teardown | Just send anytime |
| **No Congestion Control** | Keeps sending at full speed | Doesn't slow down |
| **Stateless** | Server doesn't remember clients | Each packet independent |
| **Smaller Packets** | 8-byte header | Minimal overhead |
| **Faster** | Less processing, fewer packets | Speed over reliability |

### TCP Characteristics

| Property | Description | Why |
|----------|-------------|-----|
| **Acknowledgement** | Every packet confirmed | ACK for everything |
| **Guaranteed Delivery** | Retransmits lost packets | Reliability built-in |
| **Connection-based** | 3-way handshake to start | SYN, SYN-ACK, ACK |
| **Congestion Control** | Slows down when busy | Prevents overload |
| **Stateful** | Tracks connection state | Sequence numbers |
| **Larger Packets** | 20-60 byte header | More overhead |
| **Slower** | More processing, more packets | Reliability over speed |

---

## Packet Analysis with tcpdump

### Basic Usage
```bash
# Monitor localhost traffic on port 8080
sudo tcpdump -i lo0 -n -vv -X port 8080
```

**Flags:**
- `-i lo0` = Interface (loopback for localhost)
- `-n` = Don't resolve hostnames
- `-vv` = Very verbose (show headers)
- `-X` = Show packet contents in HEX and ASCII
- `port 8080` = Filter by port

### UDP Packet Example
```
IP 127.0.0.1.56834 > 127.0.0.1.8080: UDP, length 9
```

**Breakdown:**
```
IP Header:    20 bytes
UDP Header:    8 bytes
Data:          9 bytes ("Terminal\n")
─────────────────────
TOTAL:        37 bytes
```

**Only 2 packets total:**
1. Client → Server (request)
2. Server → Client (response)

### TCP Packet Example
```
1. [S]    SYN
2. [S.]   SYN-ACK
3. [.]    ACK
4. [.]    ACK
5. [P.]   PUSH (data)
6. [F.]   FIN
7. [.]    ACK
8. [.]    ACK
9. [F.]   FIN
10. [.]   ACK
```

**10 packets to send 4 bytes of data!**

**TCP Packet Sizes:**
```
IP Header:    20 bytes
TCP Header:   20-60 bytes (with options: 32-44 typical)
Data:         4 bytes ("TCP\n")
─────────────────────
TOTAL:        56 bytes (for data packet)
Setup:        64 + 64 + 52 bytes
```

---

## Header Breakdown

### IP Header (20 bytes)
```
┌────────────────────────────────────┐
│ Version: IPv4 or IPv6              │
│ Header Length                      │
│ Type of Service (Priority)         │
│ Total Length (packet size)         │
│ Identification (packet ID)         │
│ Flags (fragmentation)              │
│ TTL (Time To Live - max hops)      │
│ Protocol: TCP=6, UDP=17            │
│ Checksum (error detection)         │
│ Source IP: 127.0.0.1               │
│ Destination IP: 127.0.0.1          │
└────────────────────────────────────┘
```

**Purpose:** Route packet from source to destination

### UDP Header (8 bytes)
```
┌─────────────────────────────────┐
│ Source Port: 56834              │
│ Destination Port: 8080          │
│ Length (total size)             │
│ Checksum (error check)          │
└─────────────────────────────────┘
```

**Simple!** Just "from where, to where, how big, is it corrupt?"

### TCP Header (20-60 bytes)
```
┌──────────────────────────────────────────┐
│ Source Port: 57899                       │
│ Destination Port: 8080                   │
│ Sequence Number (order packets)          │
│ Acknowledgment Number (confirm receipt)  │
│ Header Length                            │
│ Flags: SYN, ACK, FIN, PSH, RST, URG     │
│ Window Size (flow control)               │
│ Checksum                                 │
│ Urgent Pointer                           │
│ Options (timestamps, scaling, etc)       │
└──────────────────────────────────────────┘
```

**Complex!** Manages connection state, ordering, reliability

### TCP Flags
- **SYN** = Start connection
- **ACK** = Acknowledge received data
- **FIN** = Finish connection
- **PSH** = Push data immediately
- **RST** = Reset connection
- **URG** = Urgent data

**Common combinations:**
- `[S]` = SYN (connection request)
- `[S.]` = SYN+ACK (connection accepted)
- `[.]` = ACK (acknowledgment)
- `[P.]` = PSH+ACK (data + ack)
- `[F.]` = FIN+ACK (closing)

---

## Ephemeral Ports

**Server:** Fixed port (8080)
**Client:** Random port assigned by OS
```
Client 1: 127.0.0.1:54321 → Server: 127.0.0.1:8080
Client 2: 127.0.0.1:54322 → Server: 127.0.0.1:8080
Client 3: 127.0.0.1:52891 → Server: 127.0.0.1:8080
```

**Each nc call gets a different random port!**

**Range:** Typically 49152-65535 (dynamic/private ports)

---

## Socket API Functions

### Creating Sockets
```c
int socket(int domain, int type, int protocol);
```

**Examples:**
```c
// UDP over IPv4
socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
socket(AF_INET, SOCK_DGRAM, 0);  // 0 = auto-detect

// TCP over IPv4
socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
socket(AF_INET, SOCK_STREAM, 0);

// IPv6
socket(AF_INET6, SOCK_DGRAM, 0);

// Bluetooth
socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
```

### Binding to Port
```c
int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);
```

**Attaches socket to a specific port and IP**

### UDP Functions
```c
// Receive from anyone
recvfrom(sock, buffer, size, flags, &client_addr, &client_len);

// Send to specific address
sendto(sock, data, size, flags, &client_addr, client_len);
```

### TCP Functions
```c
// Listen for connections
listen(sock, backlog);  // backlog = max pending connections

// Accept a connection
int client_sock = accept(sock, &client_addr, &client_len);

// Receive from connected client
recv(client_sock, buffer, size, flags);

// Send to connected client
send(client_sock, data, size, flags);

// Close connection
close(client_sock);
```

---

## Use Cases

### When to Use UDP
- **Video/Audio streaming** - Don't care about occasional dropped frames
- **Online gaming** - Latest position matters, not old packets
- **DNS queries** - Small, fast, can retry if needed
- **VoIP** - Real-time, latency-sensitive
- **IoT sensors** - Simple data broadcasts
- **Network discovery** - Broadcast to find devices

### When to Use TCP
- **File downloads** - Need every byte intact
- **Web pages (HTTP/HTTPS)** - Complete HTML required
- **Databases** - Can't lose transactions
- **Email** - Messages must arrive completely
- **SSH/Remote terminals** - Every keystroke matters
- **API calls** - Reliable request/response

---

## Testing Commands

### Send UDP message
```bash
echo "test" | nc -4u -w1 localhost 8080
echo -n "test" | nc -4u -w1 localhost 8080  # No newline
```

### Send TCP message
```bash
echo "test" | nc localhost 8080
```

### Interactive mode
```bash
nc -u localhost 8080  # UDP
nc localhost 8080      # TCP
# Type messages, press Enter
# Ctrl+C to exit
```

### Flood test
```bash
# Send 1000 messages
for i in {1..1000}; do
    echo "Message $i" | nc -u -w0 localhost 8080 &
done
wait
```

### Check if server is listening
```bash
lsof -i :8080
netstat -an | grep 8080
```

---

## Common Issues & Fixes

### Port already in use
```
bind: Address already in use
```

**Fix:** Kill process using port or use different port
```bash
lsof -i :8080
kill <PID>
```

### Permission denied
```
bind: Permission denied
```

**Fix:** Use port > 1024 (ports < 1024 need root)

### Connection refused
**UDP:** Won't get this error (connectionless!)
**TCP:** Server not running or firewall blocking

### nc not echoing
**Check:**
- Server running?
- Correct port?
- Using `-w` timeout flag?
- Try different nc flags: `-4u -w1`

---

## Key Takeaways

1. **Sockets are universal** - Same API for different protocols
2. **UDP trades reliability for speed** - Fire and forget
3. **TCP guarantees delivery** - But costs overhead
4. **Headers matter** - UDP=8 bytes, TCP=20-60 bytes
5. **Connection state** - TCP tracks, UDP doesn't
6. **File descriptors** - 0,1,2 reserved, sockets start at 3
7. **Address discovery** - `recvfrom()` tells you WHO sent data
8. **Ephemeral ports** - Clients get random ports automatically

---

## Next Steps

- Build a UDP chat server (multiple clients)
- Implement TCP with keep-alive connections
- Build a file transfer protocol
- Explore IPv6 (`AF_INET6`)
- Try Unix domain sockets (`AF_UNIX`)
- Experiment with Bluetooth sockets

---

**You built real network servers from scratch!** 🔥💪🚀
