
# 服务器API 设计

要让服务器开发灵活，未后期学习和扩展方便可以做一下几个概念的设计：


一期设计

* SocketServer
* Transport
* Protocol
* EventHandle
* ResourceManager
* Connection
* ConnectionManager
* Buffer



二期设计

* SocketServerTimer
* SocketStat
* SocketStateManager
* ThreadSupport
* SocketException
* SocketErrorNo

三期设计

前两期针对功能，后两期针对性能

四期设计

有了性能，针对稳定性和方便调试

五期设计

这对部署、打包、docker或者内存分配等情况优化

## 一期

namespace: mk

文件名： include/mk/socket.h
包含类：

* SocketAddress
* SocketSession
* SocketSessionReadWrite

文件名： include/mk/socket_server.h
包含类：

* SocketServer


文件名： include/mk/transport.h
包含类：

* Transport


文件名： include/mk/transport_tcp.h
包含类：

* TcpTransport

文件名： include/mk/transport_udp.h
包含类：

* UdpTransport

文件名： include/mk/transport_ssl.h
包含类：

* SslTransport

文件名： include/mk/protocol.h
包含类：

* Protocol
* Codec
* ProtobufCodec
* ThriftCodec
* HttpCodec

文件名： include/mk/event.h
包含类：

* IoEventLoop
* IoEventListener
* IoBufferListener

文件名： include/mk/event_libev.h
包含类：

* IoLibev
* IoLibevServer
* IoLibevSession

文件名： include/mk/event_handle.h
包含类：

* EventHandle

文件名： include/mk/buffer.h
包含类：

* Buffer
* BufferResourceManager
* BufferException
* RingBuffer

Session 持有 Transport , Transport 负责解析读写



``` 

   +---------------------
   | SocketServer
   +---------------------
   | - address
   | - transport
   | - connection_manager 
   | - handler
   | + SocketServer(SocketAddress,Transport) 
   | + void Bind() 
   | + void Listen() 
   | + void Listen(int backlog) 
   | + void Close()
   |

   +---------------------
   | SocketAddress
   +---------------------
   | char * host
   | int port
   |
   |
   |
   |


   +---------------------
   | Transport
   +---------------------
   | - address
   | - accept_handle
   | - read_write_handle
   | - close_handle
   | - protocol_handle
   | - stat_handle
   | - exception_handle
   | + void Start()  
   | + void Close()  
   | + void RegisterAcceptHandle();
   | + void RegisterReadWriteHandle();
   | + void RegisterStatHandle();
   | + void RegisterCloseHandle();
   | + void RegisterProtocolHandle();
   | + void RegisterExceptionHandle();
   |
   |
   |

   +---------------------
   | Protocol
   +---------------------
   |
   |
   |
   |
   |
   |

   +---------------------
   | EventHandle
   +---------------------
   |
   |
   |
   |
   |
   |

   +---------------------
   | ResourceManager
   +---------------------
   |
   |
   |
   |
   |
   |

``` 
