cmake_minimum_required (VERSION 2.8)

# 查找当前目录下的所有源文件
set (exe agent_server)
set (SRCS main.cpp)

configure_file( ../etc/log.properties etc/log.properties COPYONLY)


add_executable (${exe} ${SRCS})

#target_link_libraries(${exe} framework netutil protobuf log4cplusU ev)
target_link_libraries(${exe} ev)

install (TARGETS ${exe} RUNTIME DESTINATION bin)

add_executable (ev1 libev1.cpp)
target_link_libraries(ev1 ev)

add_executable (ev2 libev2.cpp)
target_link_libraries(ev2 ev)

add_executable (ev3 libev3.cpp)
target_link_libraries(ev3 ev)

add_executable (ev4 libev4.cpp)
target_link_libraries(ev4 ev pthread)

add_executable (ev5 libev5.cpp
    ${MAINFOLDER}/include/mk/socket_server.h
    ${MAINFOLDER}/src/mk/socket_server.cpp

    ${MAINFOLDER}/include/mk/socket.h

    ${MAINFOLDER}/include/mk/transport.h
    ${MAINFOLDER}/include/mk/transport_tcp.h
    ${MAINFOLDER}/src/mk/transport.cpp
    ${MAINFOLDER}/src/mk/transport_tcp.cpp

    ${MAINFOLDER}/include/mk/event.h
    ${MAINFOLDER}/include/mk/event_libev.h
    ${MAINFOLDER}/src/mk/event.cpp
    ${MAINFOLDER}/src/mk/event_libev.cpp
)
target_link_libraries(ev5 ev pthread)


add_executable (f f.cpp)
target_link_libraries(f ev pthread)



