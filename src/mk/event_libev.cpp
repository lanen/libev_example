/* Copyright 2017 evan */
#include <mk/event_libev.h>
#include <iostream>

#include <mk/transport.h>

namespace mk {
    #pragma mark IoLibev
    IoLibev::IoLibev(int fd) : mk::IoEventLoop(fd) {
    }

    IoLibev::~IoLibev() {
    }

    #pragma mark IoAcceptorLibev

    void IoAcceptorLibev::Start() {
        std::cout << "set OnAccept" << std::endl;

        io.set<IoAcceptorLibev, &IoAcceptorLibev::OnAccept>(this);
        io.start(this->fd, ev::READ);

        std::cout << "Run loop" << std::endl;
        loop.run(0);
    }

    void IoAcceptorLibev::OnAccept(ev::io &watcher, int revents) {
        if ( EV_ERROR & revents ) {
            perror("got invalid event");
            return;
        }
        
        if (this->acceptor != nullptr) {
            auto ss = this->acceptor->DoAccept();
            if(nullptr != ss){
            	// 建立 Session 之后，给Session 绑定事件循环
                auto io = new IoSessionLibev(ss->GetFd());
                             
                ss->SetEventLoop(io);
                // 启动循环
                ss->OnAccept();
            }
        } else {
            perror("acceptor is null");
        }
    }

    void IoAcceptorLibev::OnWrite(ev::io &watcher) {
        std::cout << "OnWrite" << std::endl;
        // 负责事件，不复杂 IO 写
//        if (nullptr != this->reader) {
//            this->reader->Write(watcher.fd, nullptr);
//        }
    }

    void IoAcceptorLibev::OnRead(ev::io &watcher) {
        std::cout << "OnRead" << std::endl;
        // 负责事件，不复杂 IO 读
//        if (nullptr != this->reader) {
//            this->reader->Read(watcher.fd, nullptr);
//        }
        
    }

    #pragma mark IoLibevSession

    void IoSessionLibev::Start() {
        std::cout << "Session set OnAccept, fd=" << this->fd << std::endl;

        io.set<IoSessionLibev, &IoSessionLibev::OnAccept>(this);
        io.start(this->fd, ev::READ);

//        std::cout << "Session Run loop" << std::endl;

    }

    void IoSessionLibev::OnAccept(ev::io &watcher, int revents) {
        if ( EV_ERROR & revents ) {
            perror("got invalid event");
            return;
        }
        // 处理读事件
        if (revents & EV_READ) {
            this->OnRead(watcher);
        }

        // 处理写事件
        if (revents & EV_WRITE) {
            this->OnWrite(watcher);
        }
        
        if (this->transport_writer) {
            if (!this->transport_writer->HasSomethingToSend()) {
                io.set(ev::READ);
            } else {
                io.set(ev::READ | ev::WRITE);
            }
        }
    }

    void IoSessionLibev::OnWrite(ev::io &watcher) {
        std::cout << "Session-OnWrite, fd=" << this->fd << std::endl;

        if (this->transport_writer) {
            if (!this->transport_writer->HasSomethingToSend()) {
                io.set(ev::READ);
                return;
            }
        }

        // 负责事件，不复杂 IO 写
        if (this->transport_writer) {
            this->transport_writer->DoWrite();
        }

    }

    void IoSessionLibev::OnRead(ev::io &watcher) {
        std::cout << "Session-OnRead, fd=" << this->fd << std::endl;
        // 负责事件，不复杂 IO 读
        if (this->transport_reader != nullptr) {
            this->transport_reader->DoRead();
        }

    }

}  // namespace mk

