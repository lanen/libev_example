/* Copyright 2017 evan */
#ifndef INCLUDE_MK_TRANSPORT_TCP_H_
#define INCLUDE_MK_TRANSPORT_TCP_H_

#include <mk/transport.h>
#include <mk/socket.h>
#include <memory>
#include <iostream>

namespace mk {

class TcpTransport : public Transport {
 private:
    bool listening;  // 标记服务器正在监听

 public:
    explicit TcpTransport(SpAddress address) ;

    ~TcpTransport();

    void Bind();

    void Listen();

    void Listen(int backlog);

    void Start();

    void Close();

    // Create Socket
    int CreateSocket();

    // Create Client Socket
    int CreateClientSocket();

    // create transport acceptor for tcp
    inline TransportAcceptor *CreateTransportAcceptor();

};

// for tcp
class TcpTransportAcceptor : public TransportAcceptor {
 public:
    explicit TcpTransportAcceptor(Transport *transport) : TransportAcceptor(transport) {
    }
    ~TcpTransportAcceptor() {
    }

    virtual SocketSession* DoAccept() ;

};

// 此类同时实现了 IoEventListener，
// 此类实现了读写接口，注册到事件循环中，可从内核读写
class TcpSocketSession : public SocketSession , public TransportReader, public TransportWriter {
 protected:
     std::list<Buffer*>  write_queue;
 public:
    TcpSocketSession(int fd) : SocketSession(fd) {
    }

    ~TcpSocketSession(){
        std::cout << "destruct TcpSocketSession" << std::endl;
    }

    // 来自 IoEventListener
    void OnAccept() {

        if (nullptr != this->event_loop) {
            //this->event_loop->RegisterIoEventListener(this);
            this->event_loop->SetTransportReader(this);
            this->event_loop->SetTransportWriter(this);
            this->event_loop->Start();
        }

    }
    void Start(){

    }
    void Close(){

    }
    void OnRead() {

    }
    void OnWrite() {

    }
    void OnClose() {

    }
    void DoWrite() {


        Buffer *buffer = this->write_queue.front();

        ssize_t written = ::write(this->fd,
                                buffer->dpos(),
                                buffer->nbytes());
        if (written < 0) {
            perror("write error");
            return;
        }

        buffer->pos += written;
        if ( buffer->nbytes() == 0 ) {
            this->write_queue.pop_front();
            delete buffer;
        }
    }

    bool HasSomethingToSend() {
        return write_queue.empty();
    }
    void DoRead() {
        char    buffer[1024];
        ssize_t nread = ::recv(fd, buffer, sizeof(buffer), 0);

        if (nread < 0) {
            perror("read error");
            return;
        }

        if (nread == 0) {
            delete this;
        } else {
            this->write_queue.push_back(new Buffer(buffer, nread));
        }
    }
};

class TcpSocketSessionFactory : public SocketSessionFactory {
 public:
    TcpSocketSessionFactory() {
    }
    ~TcpSocketSessionFactory() {
    }

    SocketSession *CreateSocketSession(int fd);

};

}  // namespace mk
#endif  // INCLUDE_MK_TRANSPORT_TCP_H_


