#include "connections.h"

/* Listen for connections on port PORT, backlog BACKLOG requests */
bool rcv_sock::bnd(int port, int backlog)
{
    // Create UDP socket
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == -1)
        die("socket");

    // Zero out before use
    memset((char *)&si_self, 0, sizeof(si_self));

    si_self.sin_family = AF_INET;
    si_self.sin_port = htons(port);
    si_self.sin_addr.s_addr = htonl(INADDR_ANY); // From whoom to accept connections

    // Bind to port
    if (bind(sockfd, (sockaddr *)&si_self, sizeof(si_self)) == -1)
        die("bind");
    listen(sockfd, backlog);
    return true;
}


/* return no. of bytes recieved, save up to LENGTH bytes to BUF*/
int rcv_sock::recieve(char *buf, int length)
{
    int rcv_len;
    if ((rcv_len = recv(sockfd, buf, length, 0)) == -1)
        die("recieve");
    return rcv_len;
}


/* Recieve up to LENGTH bytes */
std::string rcv_sock::recieve(int length)
{
    char buf[length];

    // Try to recieve data
    recieve(buf, length);

    std::string result(buf); // Auto find length of string recieved
    return result;
}


rcv_sock::~rcv_sock()
{
    // Close socket
    close(sockfd);
}

/*
int main(int argc, char *argv[])
{
    rcv_sock r;
    r.bnd(2020);
#define LENGTH 100
    char buf[LENGTH];
    while (1)
    {
        //r.recieve(LENGTH, buf);
        std::cout << "Got : " << r.recieve(100) << '\n';
    }
    return 0;
}
*/