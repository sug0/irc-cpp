#include "tcp.h"

TCPClient::TCPClient(std::string addr, uint16_t portno)
{
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
}

TCPClient::~TCPClient()
{
    TCPClient::close_connection();
}

void TCPClient::close_connection()
{
    close(sockfd);
}

void TCPClient::send(std::string request)
{
    bzero(buf, sizeof(buf));
    int n = write(sockfd, request.c_str(), request.length());

    if (n < 0) 
        throw std::runtime_error("error writing to socket " + sockfd);
}

std::string TCPClient::receive()
{
    bzero(buf, sizeof(buf));
    int n = read(sockfd, buf, sizeof(buf));

    if (n < 0) 
        throw std::runtime_error("error reading from socket " + sockfd);

    return std::string {buf, sizeof(buf)};
}

std::string TCPClient::get(std::string request)
{
    TCPClient::send(request);
    return TCPClient::receive();
}
