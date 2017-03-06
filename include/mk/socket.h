/* Copyright 2017 evan */
#ifndef INCLUDE_MK_SOCKET_H_
#define INCLUDE_MK_SOCKET_H_

#include <mk/event.h>

#include <memory>
#include <list>
#include <string>

namespace mk {

struct Buffer {
    char *data;
    ssize_t len;
    ssize_t pos;
    // Buffer is char array
    Buffer(const char *bytes, ssize_t nbytes) {
        pos = 0;
        len = nbytes;
        data = new char[nbytes];
        memcpy(data, bytes, nbytes);
    }
    virtual ~Buffer() {
        delete [] data;
    }

    char *dpos() {
        return data + pos;
    }

    ssize_t nbytes() {
        return len - pos;
    }
};

class SocketAddress {
 private:
    const char *host;  // IP
    int port;  // 端口
 public:
    SocketAddress(const char *_host, int _port) : host(_host),
        port(_port) {
        // socket address
    }

    ~SocketAddress() {
        delete host;
    }

    const char *GetHost() {
        return this->host;
    }

    int GetPort() {
        return this->port;
    }
};  // SocketAddress

// 类型别名： SocketAddress 的智能指镇
typedef std::shared_ptr<SocketAddress> SpAddress;

class IoEventListener;
class IoEventLoop;

//  连接
class SocketSession : public IoEventListener {
 protected:
    SpAddress server_address;  // 服务器地址
    SpAddress client_address;  // 客户端地址
    int fd;

    IoEventLoop *event_loop;

 public:
    explicit SocketSession(int fd) : fd(fd) {
    }

    ~SocketSession() {
    }

    void SetServerAddress(SpAddress sa) {
        this->server_address = sa;
    }

    SpAddress GetServerAddress() {
        return this->server_address;
    }

    void SetClientAddress(SpAddress ca) {
        this->client_address = ca;
    }

    SpAddress GetClientAddress() {
        return this->client_address;
    }

    void SetEventLoop(IoEventLoop *event_loop) {
        this->event_loop = event_loop;
    }
    int GetFd() {
        return this->fd;
    }

    virtual void Start() = 0;
    virtual void Close() = 0;

    // 来自 IoEventListener
    virtual void OnAccept() = 0;
    virtual void OnRead() = 0;
    virtual void OnWrite() = 0;
    virtual void OnClose() = 0;
};  // SocketSession


//  创建 Socket 的接口
class SocketSessionFactory;
typedef std::shared_ptr<SocketSessionFactory> SpSocketSessionFactory;

class SocketCreator {
 public:
    SocketCreator() {
    }
    ~SocketCreator() {
    }
    // Create Socket
    virtual int CreateSocket() = 0;
    // Create Client Socket
    virtual int CreateClientSocket() = 0;

    SpSocketSessionFactory GetSocketSessionFactory();
};  // SocketCreator


// 会话工厂
class SocketSessionFactory {
 public:
    SocketSessionFactory() {
    }
    ~SocketSessionFactory() {
    }

    virtual SocketSession *CreateSocketSession(int fd) = 0;
};  // SocketSessionFactory

}  // namespace mk

class TransportAcceptor;
#endif  // INCLUDE_MK_SOCKET_H_
