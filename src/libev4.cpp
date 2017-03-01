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

// 服务实例
class EchoInstance {
 private:
    ev::io  io;
    static int total_clients;
    int     sfd;

    std::list<Buffer*>  write_queue;

 public:
        void callback(ev::io &wather, int revents) {
            if (EV_ERROR & revents) {
                perror("got invalid event");
            }

            // 处理读事件
            if (revents & EV_READ) {
                this->read_cb(wather);
            }

            // 处理写事件
            if (revents & EV_WRITE) {
                this->write_cb(wather);
            }

            if (this->write_queue.empty()) {
                io.set(ev::READ);
            } else {
                io.set(ev::READ | ev::WRITE);
            }
        }

        void read_cb(const ev::io &watcher) {
            char    buffer[1024];
            ssize_t nread = recv(watcher.fd, buffer, sizeof(buffer), 0);

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

        void write_cb(const ev::io &watcher) {
            if (this->write_queue.empty()) {
                io.set(ev::READ);
                return;
            }

            Buffer *buffer = this->write_queue.front();

            ssize_t written = write(watcher.fd,
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

    virtual ~EchoInstance() {
        // stop and free watcher if client socket is closing
        io.stop();
        close(this->sfd);

        printf("%d clients(s) connected. \n", --this->total_clients);
    }

    void init() {
        printf("get connection \n");
        this->total_clients++;

        this->io.set<EchoInstance, &EchoInstance::callback>(this);
        // this->io.set(this);
        this->io.start(this->sfd, ev::READ);
    }

 public:
    explicit EchoInstance(int s) : sfd(s) {
        fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0)  | O_NONBLOCK);
        this->init();
    }
};

class EchoServer {
 private:
    ev::io  io;
    ev::sig sio;
    int     s;

 public:
    explicit EchoServer(int port) {
        printf("Listening on port %d\n", port);
        struct sockaddr_in addr;

        s = socket(PF_INET, SOCK_STREAM, 0);

        addr.sin_family = AF_INET;
        addr.sin_port   = htons(port);
        addr.sin_addr.s_addr = INADDR_ANY;

        if (bind(s, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
            perror("bind");
        }

        fcntl(s, F_SETFL, fcntl(s, F_GETFL, 0) | O_NONBLOCK);
        listen(s, 5);

        io.set<EchoServer, &EchoServer::io_accept>(this);
        io.start(s, ev::READ);

        sio.set<&EchoServer::signal_cb>();
        sio.start(SIGINT);
    }

    ~EchoServer() {
        shutdown(s, SHUT_RDWR);
        close(s);
    }

    void io_accept(ev::io &watcher, int revents) {
        if ( EV_ERROR & revents ) {
            perror("got invalid event");
            return;
        }

        struct sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);

        int client_sd = accept(watcher.fd,
                (struct sockaddr *)&client_addr,
                &client_len);

        if ( client_sd < 0 ) {
            perror("accept error");
            return;
        }

        EchoInstance *client = new EchoInstance(client_sd);
    }

    static void signal_cb(ev::sig &signal, int revents) {
        signal.loop.break_loop();
    }
};

int EchoInstance::total_clients = 0;

int
main(int argc, char** argv) {
    int port = 8192;

    if (argc > 1) {
        port = atoi(argv[1]);
    }

    ev::default_loop loop;
    EchoServer  echo(port);

    // run
    loop.run(0);
    return 0;
}
