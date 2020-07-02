cmake_minimum_required(VERSION 3.15)
project(my_libco)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -O1)
aux_source_directory(./log LOG_LIB_SRCS)
aux_source_directory(./Timer TIMER_LIB_SRCS)
include_directories("./")

add_executable(my_libco
        ${LOG_LIB_SRCS}
        ${TIMER_LIB_SRCS}
        main.cpp
        util.h util.cc
        co_routine.cpp
        co_routine.h
        noncopyable.cpp
        noncopyable.h
        InetAddress.cpp
        InetAddress.h
        Socket.cpp
        Socket.h
        SocketsOps.cpp
        SocketsOps.h
        Copyable.cpp Copyable.h
        EventLoop.cpp EventLoop.h
        Epoll.cpp Epoll.h
        Poller.h Poller.cpp
        myeventFd.h myeventFd.cpp
        currentThread.cpp currentThread.h
        CallBack.h
        Channel.h Channel.cpp CoRoutineEnv.cpp CoRoutineEnv.h)