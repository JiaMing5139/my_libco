//
// Created by parallels on 6/29/20.
//

#include "co_routine.h"
#include <iostream>
#include <utility>
#include <cassert>

void co_routine::resume() {
    if(status_ ==kEnd){

        makecontext(&ctx_, reinterpret_cast<void (*)()>(co_routine::func), 1, this);
        status_=kStart;
    }
    swapcontext(&current_,&ctx_);

}

co_routine::co_routine(Task cb):cb_(std::move(cb)) {
    getcontext(&ctx_);
    stackMem_ = new char[co_routine::stackSize];
    ctx_.uc_stack.ss_size= co_routine::stackSize;
    ctx_.uc_stack.ss_sp = stackMem_;
    ctx_.uc_link = &current_;
    status_=kEnd;
}

void co_routine::yield() {
    swapcontext(&ctx_,&current_);
}

void co_routine::release() {

}

void co_routine::reset( ) {

}

void co_routine::func(void * this_) {
        assert(this_);
        auto* co = static_cast<co_routine *>(this_);
        co->cb_();
        co->status_ = kRunning;

}

co_routine::~co_routine() {
    delete stackMem_;
}

void co_routine::pfn(void *) {
}


