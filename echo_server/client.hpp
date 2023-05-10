#ifndef CLIENT_HPP
# define CLIENT_HPP

#include <iostream>
#include <exception>
#include <cstdlib> // for atoi
#include <cstring> // for memset
#include <unistd.h>
#include <arpa/inet.h> // for htnol, htons, INADDR_ANY, socketaddr_in, ...

/* socket, connect, bind, listen, accept, ... */
#include <sys/types.h>
#include <sys/socket.h> 

#define PACKET_SIZE 1024

#endif