# Socket Programming Assignments

This directory contains 5 socket programming assignments covering TCP and UDP communication, file transfer, and multi-client server handling.

## Project Structure

```
Socket_Programming/
├── Assignment_1/          # Simple TCP "Hello" Communication
│   ├── server.c
│   └── client.c
├── Assignment_2/          # TCP Chat Application
│   ├── server.c
│   └── client.c
├── Assignment_3/          # UDP Message Transfer
│   ├── server.c
│   └── client.c
├── Assignment_4/          # File Transfer Using TCP
│   ├── server.c
│   ├── client.c
│   └── sample.txt
├── Assignment_5/          # Multi-Client TCP Server
│   ├── server.c
│   └── client.c
└── README.md
```

## Assignments Overview

### Assignment 1: Simple TCP "Hello" Communication
- **Concepts**: socket(), bind(), listen(), accept(), connect(), send(), recv()
- **How to run**:
  ```bash
  # Terminal 1 - Server
  cd Assignment_1
  gcc -o server server.c
  ./server

  # Terminal 2 - Client
  cd Assignment_1
  gcc -o client client.c
  ./client
  ```
- **Expected Output**:
  - Server: `Client Connected: 127.0.0.1` and `Received: Hello Server`
  - Client: `Server Reply: Hello Client`

### Assignment 2: TCP Chat Application
- **Concepts**: Full duplex communication, continuous socket communication, string handling with fgets()
- **How to run**:
  ```bash
  # Terminal 1 - Server
  cd Assignment_2
  gcc -o server server.c
  ./server

  # Terminal 2 - Client
  cd Assignment_2
  gcc -o client client.c
  ./client
  ```
- **Expected Output**:
  - Server: `[Client]: message` and prompts `[Server]:` for input
  - Client: `[Client]:` prompts for input and receives `[Server]: reply`
  - Type `exit` on either side to terminate

### Assignment 3: UDP Message Transfer
- **Concepts**: SOCK_DGRAM, sendto(), recvfrom(), Connectionless communication
- **How to run**:
  ```bash
  # Terminal 1 - Server
  cd Assignment_3
  gcc -o server server.c
  ./server

  # Terminal 2 - Client
  cd Assignment_3
  gcc -o client client.c
  ./client
  ```
- **Expected Output**:
  - Client: `Sent: Networking Lab` and `Received: NETWORKING LAB`
  - Server converts message to uppercase and sends back

### Assignment 4: File Transfer Using TCP
- **Concepts**: File handling (fopen, fread, fwrite), Data transfer in chunks, TCP reliability
- **How to run**:
  ```bash
  # Terminal 1 - Server
  cd Assignment_4
  gcc -o server server.c
  ./server

  # Terminal 2 - Client
  cd Assignment_4
  gcc -o client client.c
  ./client
  ```
- **Expected Output**:
  - Server: `File Transfer Successful` and creates `received.txt`
  - Client: `File Transfer Complete`
- **Note**: `sample.txt` is provided for testing

### Assignment 5: Multi-Client TCP Server
- **Concepts**: Concurrent server programming using POSIX Threads (pthread), Process/thread creation, Multiple socket handling
- **How to run**:
  ```bash
  # Terminal 1 - Server
  cd Assignment_5
  gcc -pthread -o server server.c
  ./server

  # Terminal 2, 3, 4... - Clients
  cd Assignment_5
  gcc -o client client.c
  ./client "Arun - 23CS101"
  ./client "Priya - 23CS102"
  ```
- **Expected Output**:
  - Server prints each client's message with socket number
  - Multiple clients can connect simultaneously
  - Example: `Client 4: Arun - 23CS101`

## Compilation Tips

For Assignment 5 (with pthread), use the `-pthread` flag:
```bash
gcc -pthread -o server server.c
```

For other assignments:
```bash
gcc -o server server.c
gcc -o client client.c
```

## Ports Used

- Assignment 1: Port 8080 (TCP)
- Assignment 2: Port 8081 (TCP)
- Assignment 3: Port 8082 (UDP)
- Assignment 4: Port 8083 (TCP)
- Assignment 5: Port 8084 (TCP with Threading)

## Troubleshooting

1. **"Address already in use"**: Wait a few seconds before restarting the server (TIME_WAIT state)
2. **Connection refused**: Make sure the server is running before starting the client
3. **Permission denied**: Make sure you have read/write permissions in the directory
4. **Port in use**: Change the PORT define in the code if needed

## Additional Features Implemented

- Client IP address display
- Error checking with perror()
- Proper socket closing
- Buffer overflow protection
- File transfer with chunk reading/writing
- Multi-threaded concurrent client handling
- String input handling with fgets()

## Evaluation Criteria

- Program Compilation (20 marks)
- Socket API Usage (20 marks)
- Correct Communication (20 marks)
- Code Readability (20 marks)
- Output Demonstration (20 marks)

---

**Total Marks**: 100

All assignments follow proper socket programming practices and include comprehensive error handling.
