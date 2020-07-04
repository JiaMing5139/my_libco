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
    if (it == map_.end()) {
        Co_channel::Co_channelPtr newCo_channel(new Co_channel(loop_, fd, coRoutine));
        map_[fd] = std::move(newCo_channel);
    }
    auto channel = map_[fd];
    switch (operation) {
        case READ:
            channel->enableRead();
            break;
        case WRITE:
            channel->enableWrite();
            break;
    }
    if (timeout > 0) {
        channel->add_timeout(timeout);
    }
    coRoutine->yield();

   auto i = coRoutine->getChannel().lock();
   if(i){
       i->disableAll();
   }
}


void Scheduler::onClose(int fd) {
    auto it = map_.find(fd);
    if (it != map_.end()) {
        it->second->cancle();
        map_.erase(it);
    }
}

void Scheduler::disablewrite(int fd) {
    auto it = map_.find(fd);
    assert(it != map_.end());
    it->second->disableWrite();
}

void wakeUpfromSleep(co_routine *coRoutine) {

    coRoutine->resume();
}

void Scheduler::sleep(double timeout, co_routine *coRoutine) {


    if (timeout > 0)
    {
//        auto i = coRoutine->getChannel().lock();
//        if(i){
//            i->disableAll();
//        }
        loop_->runAfter(timeout, std::bind(wakeUpfromSleep, coRoutine));
        coRoutine->yield();
    }

}


Co_channel::Co_channel(EventLoop *loop, int fd, co_routine *co) :
        channelptr_(new Channel(loop, fd)),
        loop_(loop),
        coRoutine_(co),
        fd(fd) {
    //   timerId_ = loop_->runAfter(timeout,std::bind(&Co_channel::onMessage, this));
    co->setChannel(channelptr_);
    channelptr_->setReadCallBack(std::bind(&Co_channel::onMessage, this));
    channelptr_->setWriteCallBack(std::bind(&Co_channel::onMessage, this));
    channelptr_->setErrorCallBack(std::bind(&Co_channel::onMessage, this));
}

void Co_channel::add_timeout(double seconds) {
    timerId_ = loop_->runAfter(seconds, std::bind(&Co_channel::ontimeout, this));
}

int co_read_block(int fd, char *buffer, size_t len, double timetout) {
    auto currenEnv = CoRoutineEnv::co_get_curr_thread_env();
    auto scheduler = CoRoutineEnv::get_Scheduler();
    auto current_co = CoRoutineEnv::GetCurrCo(currenEnv);

    scheduler->register_and_wait(fd, current_co, timetout, Scheduler::READ);
    int n = read(fd, buffer, len);
    return n;
}

int co_wirte_block(int fd, char *buf, size_t nbyte, double timetout) {
    auto currenEnv = CoRoutineEnv::co_get_curr_thread_env();
    auto scheduler = CoRoutineEnv::get_Scheduler();
    auto current_co = CoRoutineEnv::GetCurrCo(currenEnv);

    size_t wrotelen = 0;

    ssize_t writeret = write(fd, (const char *) buf + wrotelen, nbyte - wrotelen);

    if (writeret == 0) {
        return writeret;
    }

    if (writeret > 0) {
        wrotelen += writeret;
    }
    while (wrotelen < nbyte) {

        scheduler->register_and_wait(fd, current_co, timetout, Scheduler::WRITE);
        writeret = write(fd, (const char *) buf + wrotelen, nbyte - wrotelen);

        if (writeret <= 0) {
            break;
        }
        wrotelen += writeret;

    }

    scheduler->disablewrite(fd);
    if (writeret <= 0 && wrotelen == 0) {
        return writeret;
    }
    return wrotelen;

}

int co_accept_block(int fd, InetAddress &address, double timetout) {
    auto currenEnv = CoRoutineEnv::co_get_curr_thread_env();
    auto scheduler = CoRoutineEnv::get_Scheduler();
    auto current_co = CoRoutineEnv::GetCurrCo(currenEnv);
    scheduler->register_and_wait(fd, current_co, timetout, Scheduler::READ);
    InetAddress addr;
    struct sockaddr_in6 peerAddr{};
    socklen_t len;
    int newfd = accept(fd, (struct sockaddr *) &peerAddr, &len);
    address.setSockAddrInet6(peerAddr);
    return newfd;
}

int co_close(int fd) {
    auto scheduler = CoRoutineEnv::get_Scheduler();
    scheduler->onClose(fd);
    close(fd);
}

void co_sleep(double timeout) {
    auto currenEnv = CoRoutineEnv::co_get_curr_thread_env();
    auto scheduler = CoRoutineEnv::get_Scheduler();
    auto current_co = CoRoutineEnv::GetCurrCo(currenEnv);

    scheduler->sleep(timeout, current_co);
}


