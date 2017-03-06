/* Copyright 2017 evan */
#include <stdcfg.hpp>

#include <netinet/in.h>
#include <sys/socket.h>

#include <stdio.h>
#include <ev++.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdlib.h>
#include <resolv.h>
#include <list>

#include <mk/socket_server.h>
#include <mk/event_libev.h>
#include <mk/transport_tcp.h>
#include <memory>
//
//struct Buffer {
//    char *data;
//    ssize_t len;
//    ssize_t pos;
//    // Buffer is char array
//    Buffer(const char *bytes, ssize_t nbytes) {
//        pos = 0;
//        len = nbytes;
//        data = new char[nbytes];
//        memcpy(data, bytes, nbytes);
//    }
//    virtual ~Buffer() {
//        delete [] data;
//    }
//
//    char *dpos() {
//        return data + pos;
//    }
//
//    ssize_t nbytes() {
//        return len - pos;
//    }
//};

////  客户端连接
//class Connection : public mk::IoEventListener , public mk::IoBufferListener {
//private :
//    int fd;
//    ev::io  io;
//    std::list<Buffer*>  write_queue;
//public:
//    Connection(int fd) : fd(fd){
//
//    }
//    
//    ~Connection(){
//        
//    }
//    
//    void OnAccept(){
//        
//    }
//    
//    void OnRead() {
//        char    buffer[1024];
//        ssize_t nread = recv(this->fd, buffer, sizeof(buffer), 0);
//        
//        if (nread < 0) {
//            perror("read error");
//            return;
//        }
//        
//        if (nread == 0) {
//            delete this;
//        } else {
//            this->write_queue.push_back(new Buffer(buffer, nread));
//        }
//    }
//    
//    void OnWrite() {
//        if (this->write_queue.empty()) {
//            io.set(ev::READ);
//            return;
//        }
//        
//        Buffer *buffer = this->write_queue.front();
//        
//        ssize_t written = write(this->fd,
//                                buffer->dpos(),
//                                buffer->nbytes());
//        if (written < 0) {
//            perror("write error");
//            return;
//        }
//        
//        buffer->pos += written;
//        if ( buffer->nbytes() == 0 ) {
//            this->write_queue.pop_front();
//            delete buffer;
//        }
//    }
//    
//    void OnClose() {
//    }
//    // 来自 IoBufferListener
//    bool HasBufferToWrite() {
//        return this->write_queue.empty();
//    }
//};

//class ServerAcceptor : public mk::IoEventListener {
//private:
//    std::shared_ptr<mk::Transport> socket_createor;
//    mk::SpSocketSessionFactory *session_factory;
//public:
//    
//    ServerAcceptor(std::shared_ptr<mk::Transport> _socket_createor) :
//        socket_createor(_socket_createor){
//    }
//    
//    ~ServerAcceptor() {
//    }
//    
//    void OnAccept() {
//        // TODO: 这里要思考，如何将这个地方变成原子操作
//        int client_fd = this->socket_createor->CreateClientSocket();
//        // 创建一个新连接
//        Connection *connection = new Connection(client_fd);
//        
//        mk::IoEventLoop *event_loop = new mk::IoLibevSession(client_fd, connection);
//        event_loop->Start();
//    }
//    
//    void OnRead() {
//    }
//    
//    void OnWrite() {
//    }
//    
//    void OnClose() {
//    }
//    
//   
//};

int
main(int argc, char** argv) {
    int port = 8192;

    if (argc > 1) {
        port = atoi(argv[1]);
    }
    
    // 服务器地址
    mk::SpAddress address = std::make_shared<mk::SocketAddress>("127.0.0.1", port);
    
    mk::SpTransport transport = std::make_shared<mk::TcpTransport>(address);
    
    mk::SpSocketServer socket_server = std::make_shared<mk::SocketServer>(address, transport);
    socket_server->Bind();
    socket_server->Listen();

	// 具体交给 TransportAcceptor
    //ServerAcceptor *server_acceptor = new ServerAcceptor(transport);
    //transport->SetEventListener(server_acceptor);
    
    // 这个对象具体交给配置来决定
    mk::SpEventLoop event_loop =
        std::make_shared<mk::IoAcceptorLibev>(socket_server->GetFD());
    
    transport->SetEventLoop(event_loop);
    transport->Start();
    
    return 0;
}
