/* Copyright 2017 evan */
#ifndef INCLUDE_MK_SOCKET_SERVER_H_
#define INCLUDE_MK_SOCKET_SERVER_H_

#include <mk/socket.h>

#include <mk/transport.h>

#include <memory>


namespace mk {

//  思路是将需要精心内核访问的，抽象在此处
class SocketServer {
 private:
    std::shared_ptr<SocketAddress> socket_address;  // 服务器地址
    std::shared_ptr<Transport> transport;  // 实际的TCP 或者 UDP 服务器

 public:
    explicit SocketServer(std::shared_ptr<SocketAddress> _socket_server,
                           std::shared_ptr<Transport> transport);

    ~SocketServer();

    // 绑定在地址上
    void Bind();

    // 开始监听端口
    void Listen();

    // 开始监听端口
    void Listen(int backlog);

    // 关闭Socket Server
    void Close();

    // 获取文件描述符
    int GetFD() {
        return this->transport->GetFd();
    }
};

typedef std::shared_ptr<SocketServer> SpSocketServer;

}  // namespace mk

#endif  // INCLUDE_MK_SOCKET_SERVER_H_
