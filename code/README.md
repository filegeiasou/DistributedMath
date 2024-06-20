# Distributed Maths Project

## Overview

This project implements a distributed mathematics service using UNIX RPC. It includes server and client programs for performing statistical calculations over the network. The project consists of three main components:
1. `stats_server`: The server that performs the statistical calculations.
2. `stats_client`: The client that communicates with the server using RPC.
3. `socket_clnt`: An additional client that connects using a raw socket.

## Prerequisites

Ensure you have the following installed on your system:
- GNU Make
- GCC (GNU Compiler Collection)
- RPCGEN (RPC protocol compiler)

## Compilation

To compile the project, follow these steps:

1. Open a terminal.
2. Navigate to the project directory.
3. Run the following command:
   ```sh
   make
### To start the server, use the following command:
./stats_server

### To run the RPC client, use the following command:
./stats_client <port> localhost

### Running the Socket Client
To run the socket client, use the following command:
./socket_clnt <port> localhost
