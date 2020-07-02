//
// Created by parallels on 6/29/20.
//

#ifndef MY_LIBCO_CO_ROUTINE_H
#define MY_LIBCO_CO_ROUTINE_H

#include <ucontext.h>
#include <functional>
#include "noncopyable.h"
#include "CoRoutineEnv.h"
// stCoRoutine_t
extern __thread ::co_routine* main_thread_;
extern __thread ::co_routine* currentRunning;

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
    void reset(Task cb);
    static void func(void* this_);

    void setMain(){
        isMain_ = true;
    }
    bool isMain(){
        return isMain_;
    }
    CoRoutineEnv::CoRoutineEnv_t * env(){
        return env_;
    }

private:
    char * stackMem_;
    ::ucontext_t ctx_{};  // libco stCoRoutineEnv_t
    Task cb_;
    status status_ = kEnd;

    bool isMain_ = false;
    CoRoutineEnv::CoRoutineEnv_t * env_;
};

void init_env();




#endif //MY_LIBCO_CO_ROUTINE_H
