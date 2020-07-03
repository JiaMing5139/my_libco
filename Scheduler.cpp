//
// Created by parallels on 7/3/20.
//

#include "Scheduler.h"
#include "CoRoutineEnv.h"
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <Socket.h>

void Scheduler::register_and_wait(int fd, co_routine *coRoutine, double timeout, Scheduler::Wait_operation operation) {
    auto it = map_.find(fd);
    if(it == map_.end()){
        Co_channel::Co_channelPtr newCo_channel(new Co_channel(loop_, fd, coRoutine));
        switch (operation) {
            case READ:
                newCo_channel->enableRead();
                break;
            case WRITE:
                newCo_channel->enableWrite();
                break;
        }
        //TimerId = runAfter();
        map_[fd] = std::move(newCo_channel);
    }

    coRoutine->yield();
}

void Scheduler::onClose(int fd) {
    auto it = map_.find(fd);
    if (it != map_.end()) {
        map_.erase(it);
    }
}

void Scheduler::disablewrite(int fd) {
    auto it = map_.find(fd);
    assert(it != map_.end());
    it->second->disableWrite();
}


Co_channel::Co_channel(EventLoop *loop, int fd, co_routine *co) :
        channelptr_(new Channel(loop, fd)),
        coRoutine_(co) {
    channelptr_->setReadCallBack(std::bind(&Co_channel::onMessage, this));
    channelptr_->setWriteCallBack(std::bind(&Co_channel::onWrite, this));
}

int co_read_block(int fd, char *buffer, size_t len) {
    auto currenEnv = CoRoutineEnv::co_get_curr_thread_env();
    auto scheduler = CoRoutineEnv::get_Scheduler();
    auto current_co = CoRoutineEnv::GetCurrCo(currenEnv);

    scheduler->register_and_wait(fd, current_co, 1000, Scheduler::READ);
    int n = read(fd, buffer, len);
    return n;
}

int co_wirte_block(int fd, char *buf, size_t nbyte) {
    auto currenEnv = CoRoutineEnv::co_get_curr_thread_env();
    auto scheduler = CoRoutineEnv::get_Scheduler();
    auto current_co = CoRoutineEnv::GetCurrCo(currenEnv);

    size_t wrotelen = 0;

    ssize_t writeret = write( fd,(const char*)buf + wrotelen,nbyte - wrotelen );

    if (writeret == 0)
    {
        return writeret;
    }

    if( writeret > 0 )
    {
        wrotelen += writeret;
    }
    while( wrotelen < nbyte )
    {

        scheduler->register_and_wait(fd, current_co, 1000, Scheduler::WRITE);
        writeret = write( fd,(const char*)buf + wrotelen,nbyte - wrotelen );

        if( writeret <= 0 )
        {
            break;
        }
        wrotelen += writeret ;
    }

    scheduler->disablewrite(fd);
    if (writeret <= 0 && wrotelen == 0)
    {
        return writeret;
    }
    return wrotelen;

}

int co_accept_block( Socket * socket,InetAddress & address) {
    auto currenEnv = CoRoutineEnv::co_get_curr_thread_env();
    auto scheduler = CoRoutineEnv::get_Scheduler();
    auto current_co = CoRoutineEnv::GetCurrCo(currenEnv);

    scheduler->register_and_wait(socket->fd(), current_co, 1000, Scheduler::READ);
    InetAddress addr;
    int fd = socket->accept(&addr);
    return fd;
}


