#include "connections.h"

in_addr snd_sock::str2addr(const char *hostname)
{
        in_addr result;
        if (inet_addr(hostname) == -1)
        { // Need resolving
            hostent *host;
            in_addr **addr_list;

            if ((host = gethostbyname(hostname)) == NULL)
                die("Failed to resolve hostname");

            //Cast the h_addr_list to in_addr , since h_addr_list also has the ip hostname in long format only
            addr_list = (in_addr **)host->h_addr_list;
            result = *addr_list[0];
        }
        else
        { // Plain IP address
            result.s_addr = inet_addr(hostname);
        }
        return result;
}


snd_sock::snd_sock()
{
    sockfd = -1;
}

/* Connect to address ADDRESS on port PORT using udp */
bool snd_sock::conn(in_addr address, int port)
{
    if (sockfd == -1)
    {
        sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
        if (sockfd == -1)
            die("socket");
    }

    // Zero before use
    memset((char *)&si_other, 0, sizeof(si_other));

    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(port);
    si_other.sin_addr = address;

    if (connect(sockfd, (sockaddr *)&si_other, sizeof(si_other)) == -1)
        die("connect");

    return true;
}

/* Connect to server at human readable HOSTNAME */
bool snd_sock::conn(const char *hostname, int port)
{
    in_addr address;
    address = str2addr(hostname);
    return conn(address, port);
}

/* send string S of length LEN, return number of bytes sent */
int snd_sock::send_data(const char *s, int len)
{
    std::printf("Sending %d Bytes\n", len);
    int n = send(sockfd, s, len, 0);
    return n;
}

/* send string S, return number of bytes sent */
int snd_sock::send_data(const char *s)
{
    int len = strlen(s);
    return send_data(s, len);
}

/*
int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        std::printf("Usage: %s server port\n", argv[0]);
        exit(1);
    }
    int port = atoi(argv[2]);
    snd_sock s;
    s.conn(argv[1], port);
    
    std::string msg;
    //memset(msg,0,10);

    std::printf("------------------------------\ntype what to send below :\n");
    while (1)
    {
        std::cin >> msg;
//        msg[0] = 'e';
        s.send_data(msg.c_str());
       // msg[0] = 'd';
       // s.send_data(msg);
    }
    return 0;
}
*/