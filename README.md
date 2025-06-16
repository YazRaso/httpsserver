# C HTTPS Server

Simple HTTPS server implemented in C using OpenSSL and BSD sockets.

## Features

- Creates a TCP server socket bound to a specified IP and port.
- Accepts SSL/TLS connections using OpenSSL.
- Handles basic HTTP requests (GET and POST).
- Parses POST requests with Content-Length and reads the full body.
- Sends simple HTTP responses.
- Proper error handling and socket cleanup.

## Requirements

- OpenSSL library installed
- C compiler (gcc or clang)
- Linux or Unix-like OS (for BSD sockets API)

## Build

```sh
gcc -o https_server main.c -lssl -lcrypto
