//
// Created by parallels on 6/29/20.
//

#ifndef MY_LIBCO_CO_ROUTINE_H
#define MY_LIBCO_CO_ROUTINE_H

#include <ucontext.h>
#include <functional>
#include "noncopyable.h"
// stCoRoutine_t
class co_routine;
extern __thread ::co_routine* main_thread_;
void co_swap(co_routine * current, co_routine * next);

class co_routine :public noncopyable{
public:
    friend void co_swap(co_routine * current, co_routine * next);
public:
    enum status{
        kStart,
        kEnd,
    };
    typedef std::function<void ()> Task;
    static const size_t stackSize = 1024*1024;
    explicit co_routine(Task cb = nullptr) ;
    ~co_routine();
    void resume();
    void yield();
    static void func(void* this_);
private:
    char * stackMem_;
    ::ucontext_t ctx_{};  // libco stCoRoutineEnv_t
    Task cb_;
    status status_ = kEnd;
};

void init_env();




#endif //MY_LIBCO_CO_ROUTINE_H
