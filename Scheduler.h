//
// Created by parallels on 7/3/20.
//

#ifndef MY_LIBCO_SCHEDULER_H
#define MY_LIBCO_SCHEDULER_H

#include "co_routine.h"
#include "Channel.h"
#include "Socket.h"
#include "InetAddress.h"
#include <map>
#include <memory>
#include <sys/poll.h>
#include <iostream>
#include <stack>
class EventLoop;
class Co_channel{
public:
    typedef std::shared_ptr<Co_channel > Co_channelPtr;
    Co_channel(EventLoop *loop,int fd,co_routine*);
    void onMessage(){
        loop_->cancleTimer(timerId_);
        coRoutine_->resume();
    }
    void ontimeout(){

        coRoutine_->resume();
    }
    void enableWrite(){
        channelptr_->enableWrite();
    }
    void disableWrite(){
        if(channelptr_->isWtriting())
        channelptr_->disableWriting();
    }

    void add_timeout(double seconds);

    void enableRead(){
       if( !(channelptr_->events() & POLLIN))
        channelptr_->enableRead();
    }
    void cancle(){
        assert(this);
        loop_->cancel(channelptr_);
    }


private:
    int fd;
    TimerId timerId_;
    EventLoop * loop_;
    Channel::Channelptr channelptr_;
    co_routine * coRoutine_;
};

class Scheduler :public noncopyable{
public:
    explicit Scheduler(EventLoop*loop):loop_(loop){
    }
enum Wait_operation{
    READ,
    WRITE
};
void register_and_wait(int fd,co_routine* coRoutine,double timeout,Wait_operation);
void onClose(int fd);
void sleep(double timeout,co_routine *coRoutine);
void disablewrite(int fd);


private:
    EventLoop * loop_;
    std::map<int,Co_channel::Co_channelPtr> map_;
};


int co_read_block(int fd,char *buffer,size_t, double timeout);
int co_wirte_block(int fd,char *buffer,size_t,double timeout);
int co_accept_block(int fd,InetAddress & address,double timeout);
int co_close(int fd);
void co_sleep(double timeout);

#endif //MY_LIBCO_SCHEDULER_H
