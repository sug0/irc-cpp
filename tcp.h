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

#ifdef _USE_SSL_

#include <openssl/crypto.h>
#include <openssl/x509.h>
#include <openssl/pem.h>
#include <openssl/ssl.h>
#include <openssl/err.h>

#endif

class TCPClient {
    std::string addr;
    char buf[4096];
    int sockfd, portno, n;
    struct hostent *server;
    struct sockaddr_in serv_addr;
#ifdef _USE_SSL_
    SSL_CTX *ctx;
    SSL *ssl;
    const SSL_METHOD *meth;
#endif

public:
    TCPClient(std::string addr, uint16_t portno);
    ~TCPClient();
    void close_connection();
    void send(std::string request);
    std::string receive();
    std::string get(std::string request);
};

#endif
