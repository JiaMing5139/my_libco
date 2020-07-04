//
// Created by parallels on 6/29/20.
//

#include "co_routine.h"
#include <iostream>
#include <utility>
#include <cassert>
#include "CoRoutineEnv.h"
#include "Channel.h"



void co_swap(co_routine *current, co_routine *next) {
    swapcontext(&current->ctx_, &next->ctx_);
}


void co_routine::resume() {

    assert(cb_);

    co_routine *lpCurrRoutine = env_->pCallStack[env_->iCallStackSize - 1];
    ctx_.uc_link = &lpCurrRoutine->ctx_;

    if(status_ !=kStart){
        makecontext(&ctx_, reinterpret_cast<void (*)()>(co_routine::func), 1, this);
    }


    env_->pCallStack[env_->iCallStackSize++] = this;

    co_swap(lpCurrRoutine, this);


}


co_routine::co_routine(Task cb)
        : cb_(std::move(cb)) {
    getcontext(&ctx_);
    stackMem_ = new char[stackSize];
    ctx_.uc_stack.ss_size = stackSize;
    ctx_.uc_stack.ss_sp = stackMem_;
  //  ctx_.uc_link = &main_thread_->ctx_;
    env_ = CoRoutineEnv::co_get_curr_thread_env();
}


void co_routine::yield() {
//    assert(status_==kEnd);
    co_routine *lpCurrRoutine = env_->pCallStack[env_->iCallStackSize - 2];
    env_->iCallStackSize --;
    co_swap(this, lpCurrRoutine);
}


void co_routine::func(void *this_) {
    assert(this_);
    auto *this_co = static_cast<co_routine *>(this_);
    this_co->status_ = kStart;
    this_co->cb_();
    this_co->status_ = kEnd;
    this_co->yield();
}

co_routine::~co_routine() {
    delete stackMem_;
}

void co_routine::reset(const co_routine::Task& cb) {
    assert(cb);
    cb_ = cb;
}

void co_routine::setChannel(std::shared_ptr<Channel> ptr) {
    channelWeakPtr = ptr;
}

std::weak_ptr<Channel> co_routine::getChannel() {
    return channelWeakPtr;
}


