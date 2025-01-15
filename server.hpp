#ifndef SERVER_HPP
#define SERVER_HPP
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <cstdio>
#include <cstdlib>




#include <arpa/inet.h>
#include <netdb.h>

#define BACKLOG 5
#define MAXCLIENT 2
#define BUFFER_SIZE 1024
#endif