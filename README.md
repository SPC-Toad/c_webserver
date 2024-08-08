# Simple HTTP Server (In C!)

This project implements a simple HTTP server in C. The server listens on a specified port and serves the contents of an `index.html` file to any HTTP clients that connect.

## Files

- `http_server.c`: The main server implementation.
- `Makefile`: A makefile to build, run, and clean the project.

## Requirements

- GCC (GNU Compiler Collection)
- Make (Build automation tool)

## Building the Project

To build the project, simply run:

```sh
make server
```

This will compile `http_server.c` and create an executable named `http_server`.

## Running the Server

To run the server, use:

```sh
make run-server
```

By default, this will start the server on port `8080`. You can specify a different port by editing the `run-server` target in the `Makefile` or by running the executable directly:

```sh
./http_server <port>
```

For example, to run the server on port 8080:

```sh
./http_server 8080
```

## Cleaning Up

To clean up the project (remove the compiled executable), run:

```sh
make clean
```

## How It Works

The server performs the following steps:

1. **Initialize the Socket**: It creates a socket for communication.
2. **Bind the Socket**: It binds the socket to the specified port on the localhost.
3. **Listen for Connections**: It listens for incoming connections.
4. **Accept Connections**: It accepts incoming connections and reads the `index.html` file each time a request is received.
5. **Send Response**: It sends the contents of `index.html` back to the client with appropriate HTTP headers.
6. **Clean Up**: It closes the client socket and continues to listen for new connections.

**Note**

Make sure this file is located in the same directory as your `http_server` executable.