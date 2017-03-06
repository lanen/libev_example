/* Copyright 2017 evan */
#ifndef INCLUDE_MK_EVENT_H_
#define INCLUDE_MK_EVENT_H_

#include <memory>

namespace mk {

// 声明
class TransportAcceptor;
class TransportReader;
class TransportWriter;

// Io 事件监听器
class IoEventListener {
 public:
    IoEventListener();

    virtual ~IoEventListener();
    virtual void OnAccept();
    virtual void OnRead();
    virtual void OnWrite();
    virtual void OnClose();
};  // IoEventListener

// Io Buffer 监听器
class IoBufferListener {
 public:
    IoBufferListener() {
       // Nothing
    }

    ~IoBufferListener() {
       // Nothing
    }

    virtual bool HasBufferToWrite() = 0;
};  // IoBufferListener

// Io 事件循环
class IoEventLoop {
 protected:
    int fd;  // 文件描述符
    IoEventListener *listener;  // 事件监听器
    TransportAcceptor *acceptor;

 public:
    explicit IoEventLoop(int fd);

    virtual ~IoEventLoop();

    void RegisterIoEventListener(IoEventListener *_listener) {
        this->listener = _listener;
    }

    void SetTransportAcceptor(TransportAcceptor *acceptor) {
        this->acceptor = acceptor;
    }

    virtual void SetTransportReader(TransportReader *reader) {
    }

    virtual void SetTransportWriter(TransportWriter *writer) {
    }

    virtual void Start();
};  // IoEventLoop

typedef std::shared_ptr<IoEventLoop> SpEventLoop;

}  // namespace mk
#endif  // INCLUDE_MK_EVENT_H_
