/* Copyright 2017 evan */
#include <mk/transport_tcp.h>

//这样设计的目的，尽量在transport 层做网络io，与内核沟通

namespace mk {

TcpTransport::TcpTransport(SpAddress address) : mk::Transport(address) {
}
    
TcpTransport::~TcpTransport()  {
}
    
void TcpTransport::Bind() {
    // bind address
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(this->address->GetPort());
    addr.sin_addr.s_addr = INADDR_ANY;
    
    if (this->fd <= 0) {
        this->CreateSocket();
    }
    
    if (this->fd > 0) {
        if (bind(this->fd, (struct sockaddr *)&addr, sizeof(addr)) != 0) {
            perror("bind");
        }
    }
}


void TcpTransport::Listen() {
	this->Listen(5);
}
    
void TcpTransport::Listen(int backlog) {
    ::listen(this->fd, backlog);
    listening = true;
}

void TcpTransport::Start() {
	// create transport acceptor
    auto acceptor= this->GetTransportAcceptor();
    auto factory = std::make_shared<TcpSocketSessionFactory>();
    acceptor->SetSocketSessionFactory(factory);
    
    
    this->event_loop->RegisterIoEventListener(this->event_listener);
    this->event_loop->SetTransportAcceptor(acceptor);
    
   	this->event_loop->Start();
}

void TcpTransport::Close() {
    if (!listening) return;
    
    shutdown(this->fd, SHUT_RDWR);
    close(this->fd);
    listening = false;
}
    
int TcpTransport::CreateSocket() {
	// create socket
	this->fd = ::socket(PF_INET, SOCK_STREAM, 0);
    
    // 将 socket 标记成非阻塞
    int flags = fcntl(this->fd, F_GETFL, 0);
    fcntl(this->fd, F_SETFL, flags | O_NONBLOCK);
        
	// 设置 Socket Options
    int option=1;
    setsockopt(this->fd,SOL_SOCKET,SO_REUSEADDR,(char *)&option,sizeof(option));
    
    return this->fd;
}
    
int TcpTransport::CreateClientSocket() {
    // Create Client Socket
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    int client_sd = ::accept(this->fd,
                (struct sockaddr *)&client_addr,
                &client_len);
        
    if ( client_sd < 0 ) {
    	perror("accept error");
        return -1;
    }
    fcntl(client_sd, F_SETFL, fcntl(client_sd, F_GETFL, 0)  | O_NONBLOCK);
    return client_sd;
}

TransportAcceptor * TcpTransport::CreateTransportAcceptor() {
	
    return new TcpTransportAcceptor(this);
}

#pragma mark TransportAcceptor

SocketSession* TcpTransportAcceptor::DoAccept() {
    int client_fd = this->transport->CreateClientSocket();
    
    if (nullptr == this->socket_session_factory) {
        return nullptr;
    }
    
    auto ss = this->socket_session_factory->CreateSocketSession(client_fd);
    if (nullptr != ss) {
        ss->SetServerAddress(this->transport->GetServerAddress());
    }
    return ss;
}

#pragma mark TcpSocketSessionFactory 

SocketSession* TcpSocketSessionFactory::CreateSocketSession(int fd) {
	auto ss = new TcpSocketSession(fd);
    return ss;
}
    

}  // namespace mk
