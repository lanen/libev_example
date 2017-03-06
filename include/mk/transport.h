/* Copyright 2017 evan */
#ifndef INCLUDE_MK_TRANSPORT_H_
#define INCLUDE_MK_TRANSPORT_H_

#include <mk/socket.h>
#include <mk/event.h>

#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <resolv.h>

#include <memory>

#include <list>

namespace mk {

typedef std::shared_ptr<TransportAcceptor> SpTransportAcceptor;

// Transport 是实际进行IO 对象
// 提供 TcpTransport
// 提供 UdpTransport
class Transport : public SocketCreator {
 protected:
    SpAddress address = nullptr;
    SpEventLoop event_loop = nullptr;
    int fd = -1;
    IoEventListener *event_listener = nullptr;
    TransportAcceptor *transport_acceptor = nullptr;

 public:
    explicit Transport(SpAddress address);

    ~Transport() {
    }

    // 绑定 socket fd
    virtual void Bind() = 0;

    // 开始监听端口
    virtual void Listen() = 0;

    // 开始监听端口
    virtual void Listen(int backlog) = 0;

    // 启动函数
    virtual void Start() = 0;

    // 关闭
    virtual void Close() = 0;

    void SetFd(int fd) {
        this->fd = fd;
    }
    int GetFd() {
        return this->fd;
    }

    void SetEventLoop(SpEventLoop loop) {
        this->event_loop = loop;
    }

    void SetEventListener(IoEventListener *event_listener) {
        this->event_listener = event_listener;
    }

    SpAddress GetServerAddress() {
        return this->address;
    }

    // 获取绑定在当前Transport 上面的 接收器
    TransportAcceptor *GetTransportAcceptor() {
        if (this->transport_acceptor == nullptr) {
            this->transport_acceptor = this->CreateTransportAcceptor();
        }
        return transport_acceptor;
    }

    virtual inline TransportAcceptor *CreateTransportAcceptor() = 0;


 public:
    // Create Socket
    virtual int CreateSocket() = 0;

    // Create Client Socket
    virtual int CreateClientSocket() = 0;
};

typedef std::shared_ptr<Transport> SpTransport;

// 负责接收
class TransportAcceptor {
 protected:
    Transport *transport = nullptr;
    SpSocketSessionFactory socket_session_factory = nullptr;

 public:
    explicit TransportAcceptor(Transport *transport) : transport(transport) {
    }

    ~TransportAcceptor() {
    }

    void SetSocketSessionFactory(SpSocketSessionFactory factory) {
        this->socket_session_factory = factory;
    }
    // 返回 SocketSession, 有可能存在 nullptr
    virtual SocketSession* DoAccept() = 0;
};

// typedef std::shared_ptr<TransportAcceptor> SpTransportAcceptor;

// 负责读
class TransportReader {
 public:
    TransportReader() {
    }

    ~TransportReader() {
    }

    virtual void DoRead() = 0;
};

// 负责写
class TransportWriter {
 public:
    TransportWriter() {
    }
    ~TransportWriter() {
    }

	virtual bool HasSomethingToSend() = 0;

	virtual void DoWrite() = 0;
};

// 负责关
//class TransportCloser {

//};

// 负责清理
//class TransportCleaner {

//};

// 负责Buffer
//class TransportBuffer {

//};

// 负责线程
//class TransportThread {

//};

}  // namespace mk
#endif  // INCLUDE_MK_TRANSPORT_H_

