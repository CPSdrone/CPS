#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <memory.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <errno.h>
#include <stdlib.h>
#include <iostream>

#define DEFAULT_PORT 2020

int mode;
int die(char *msg)
{
    perror(msg);
    exit(1);
}

/* UDP socket for sending data */
class send_sock
{
public:
    send_sock()
    {
        sockfd = -1;
        port = 0;
        address = "";
    }

    bool conn(const char *, int);
    bool send_data(const char *, int);
    std::string receive(int);

private:
    int sockfd;
    std::string address;
    int port;
    sockaddr_in server;
};

bool send_sock::conn(const char *address, int port = DEFAULT_PORT)
{
    // Create socket if not connected
    if (sockfd == -1)
    {
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd == -1)
            die("Could not create socket");
        std::printf("Socket created\n");
    }

    // Setup adress
    if (inet_addr(address) == -1)
    { // Need resolving
        hostent *host;
        in_addr **addr_list;

        if ((host = gethostbyname(address)) == NULL)
        {
            perror("Failed to resolve hostname");
            return false;
        }

        //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
        addr_list = (struct in_addr **)host->h_addr_list;
        server.sin_addr = *addr_list[0];
    }
    else
    { // Plain ip address
        server.sin_addr.s_addr = inet_addr(address);
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to server
    if (connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Connection failed");
        return false;
    }
    std::printf("Connection succesfull\n");
    return true;
}

/* Send data to connected host */
bool send_sock::send_data(const char *data, int data_len)
{
    int n = send(sockfd, data, data_len, 0);
    if (n < 0)
    {
        perror("Send failed");
        return false;
    }
    printf("Data sent : %d Bytes to port %d\n", n, ntohs(server.sin_port));
    return true;
}

// Buggy and ment for deletion
std::string send_sock::receive(int size = 64)
{
    char buf[size];
    std::string reply;
    // Get socket info
    sockaddr_in sin;
    socklen_t len;
    if (getsockname(sockfd, (sockaddr *)&sin, &len) == -1)
        perror("Get info failed");
    else
        std::printf("Listeninig on port : %d\n", sin.sin_port);

    listen(sockfd, 5);
    std::cout << "Starting recieve" << '\n';
    // Recieve
    if (recv(sockfd, buf, size, 0) == -1)
    {
        printf("Recieve fail\n");
    }
    std::cout << "Recieve successful" << '\n';
    reply = buf;
    return reply;
}

int main(int argc, char *argv[])
{
    /*
    send_sock s("T440");
    int i = s.send_msg("test");
    std::cout << i << " bytes sent" << '\n' ;
    i = s.send_msg("test");
    std::cout << i << " bytes sent" << '\n' ;
    i = s.send_msg("test");
    std::cout << i << " bytes sent" << '\n' ;
    //*/
    //*
    if (argc < 2)
        return 1;
    mode = atoi(argv[1]);
    //*/
    //int mode = 0;

    send_sock s;
    if (mode == 1)
    {
        s.conn("T440");
        s.send_data("Whatever", 9);
    }
    else
    {
        s.conn("T440");
        std::string reply = s.receive();
        std::cout << reply << std::endl;
    }

    return 0;
}