/* Copyright 2017 evan */
#ifndef INCLUDE_MK_EVENT_LIBEV_H_
#define INCLUDE_MK_EVENT_LIBEV_H_

#include <mk/event.h>
#include <ev++.h>

namespace mk {

// 封装 libev C++ API
class IoLibev : public IoEventLoop {
 protected:
    ev::io  io;  // io watcher

 public:
    explicit IoLibev(int fd);

    virtual ~IoLibev();

    virtual void Start() = 0;

 protected:
    virtual void OnAccept(ev::io &watcher, int revents) = 0;

    virtual void OnRead(ev::io &watcher) = 0;

    virtual void OnWrite(ev::io &watcher) = 0;
};

// 针对服务器循环
class IoAcceptorLibev : public IoLibev {
 private:
    ev::default_loop loop;  // io default event loop
 public:
    explicit IoAcceptorLibev(int fd) : IoLibev(fd) {
        // None
    }

    ~IoAcceptorLibev() {
        // None
    }

    virtual void Start();

 protected:
    virtual void OnAccept(ev::io &watcher, int revents);

    virtual void OnRead(ev::io &watcher);

    virtual void OnWrite(ev::io &watcher);
};

//  针对连接的循环
class IoSessionLibev : public IoLibev {
 private:
    TransportReader *transport_reader;
    TransportWriter *transport_writer;

 public:
    explicit IoSessionLibev(int fd) : IoLibev(fd) {
        // None
    }

    ~IoSessionLibev() {
        // None
    }

    void SetTransportReader(TransportReader *transport_reader) {
        this->transport_reader = transport_reader;
    }

    void SetTransportWriter(TransportWriter *transport_writer) {
        this->transport_writer = transport_writer;
    }

    virtual void Start();

 protected:
    virtual void OnAccept(ev::io &watcher, int revents);

    virtual void OnRead(ev::io &watcher);

    virtual void OnWrite(ev::io &watcher);
};


}  // namespace mk
#endif  // INCLUDE_MK_EVENT_LIBEV_H_


