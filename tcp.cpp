#include "tcp.h"

TCPClient::TCPClient(bool use_ssl, std::string addr, uint16_t portno)
{
    this->use_ssl = use_ssl;

    server = gethostbyname(addr.c_str());

    if (server == nullptr)
        throw std::invalid_argument("nonexistant host " + addr);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        throw std::runtime_error("error opening socket");

    this->addr   = addr;
    this->portno = portno;

    bzero((char *)&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr, 
          (char *)&serv_addr.sin_addr.s_addr,
          server->h_length);

    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
        throw std::runtime_error("error connecting to host " + addr);

    if (use_ssl) {
        SSL_library_init();

        meth = TLSv1_2_client_method();
        ctx  = SSL_CTX_new(meth);

        if (ctx == nullptr)
            throw std::runtime_error("error negotiating ssl");

        ssl = SSL_new(ctx);

        if (ssl == nullptr)
            throw std::runtime_error("error creating ssl");

        SSL_set_fd(ssl, sockfd);
        SSL_set_connect_state(ssl);

        if (SSL_connect(ssl) < 0)
            throw std::runtime_error("error connecting ssl");
    }
}

TCPClient::~TCPClient()
{
    TCPClient::close_connection();

    if (use_ssl) {
        SSL_shutdown(ssl);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
    }
}

void TCPClient::close_connection()
{
    close(sockfd);
}

void TCPClient::send(std::string request)
{
    bzero(buf, sizeof(buf));

    if (use_ssl)
        n = SSL_write(ssl, request.c_str(), request.length());
    else
        n = write(sockfd, request.c_str(), request.length());

    if (n < 0)
        throw std::runtime_error("error writing to socket");
}

std::string TCPClient::receive()
{
    bzero(buf, sizeof(buf));

    if (use_ssl)
        n = SSL_read(ssl, buf, sizeof(buf));
    else
        n = read(sockfd, buf, sizeof(buf));

    if (n < 0)
        throw std::runtime_error("error reading from socket");

    return std::string {buf, strlen(buf)};
}

std::string TCPClient::get(std::string request)
{
    TCPClient::send(request);
    return TCPClient::receive();
}
