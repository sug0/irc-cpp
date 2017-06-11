#ifndef _TCP_CONN_H_
#define _TCP_CONN_H_

#include <stdexcept>
#include <string>
#include <cstdlib>
#include <cstring>
#include <stdint.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

class TCPClient {
    std::string addr;
    char buf[4096];
    int sockfd, portno, n;
    struct hostent *server;
    struct sockaddr_in serv_addr;

public:
    TCPClient(std::string addr, uint16_t portno);
    ~TCPClient();
    void close_connection();
    void send(std::string request);
    std::string receive();
    std::string get(std::string request);
};

#endif
