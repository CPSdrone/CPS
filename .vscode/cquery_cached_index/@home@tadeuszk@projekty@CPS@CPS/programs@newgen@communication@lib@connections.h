#ifndef CONNECTIONS_H
#define CONNECTIONS_H

#include <cstdio>  //printf
#include <string.h> //memset
#include <stdlib.h> //exit(0);
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream> //std::string

#include "useful.h"

#define DEFAULT_PORT 2020

/* UDP socket for sending data */
class snd_sock
{
private:
    sockaddr_in si_other;
    int sockfd;

    /* return address corresponding to hostname */
    in_addr str2addr(const char *hostname);

public:
    snd_sock();
    /* Connect to server before sending data */

    /* Connect to address ADDRESS on port PORT using udp */
    bool conn(in_addr address, int port );

    /* Connect to server at human readable HOSTNAME */
    bool conn(const char *hostname, int port = DEFAULT_PORT);


    /* Send data to previosly connected port */

    int send_data(const char *s, int len);
    int send_data(const char *s);
};


/* UDP socket for recieving data */
class rcv_sock
{
private:
    sockaddr_in si_self;
    int sockfd;

public:
    /* Bind to a port allows connections to it */

    /* Listen for connections on port PORT, backlog BACKLOG requests */
    bool bnd(int port = DEFAULT_PORT, int backlog = 5);

    int recieve(char *buf, int length);
    std::string recieve(int length); // The easier recieve
    ~rcv_sock();
};


#endif