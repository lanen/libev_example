/* Copyright 2017 evan */
#include <mk/socket_server.h>
#include <iostream>

namespace mk {

#pragma mark SocketServer
SocketServer::SocketServer(std::shared_ptr<SocketAddress> _socket_address,
                           std::shared_ptr<Transport> transport) :
    socket_address(_socket_address),
    transport(transport) {
    // socket server
    // this->socket_address = _socket_address;
    std::cout << "SocketServer ";
    std::cout << this->socket_address->GetHost();
    std::cout << " Port:" << this->socket_address->GetPort();
    std::cout << std::endl;
}

SocketServer::~SocketServer() {
    // socket server
    this->Close();
}

void SocketServer::Bind() {
    this->transport->Bind();
}

void SocketServer::Listen() {
    this->Listen(5);
}

void SocketServer::Listen(int backlog) {
    this->transport->Listen(backlog);
}

void SocketServer::Close() {
    this->transport->Close();
}


}  // namespace mk
