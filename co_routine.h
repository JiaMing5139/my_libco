//
// Created by parallels on 6/29/20.
//

#ifndef MY_LIBCO_CO_ROUTINE_H
#define MY_LIBCO_CO_ROUTINE_H

#include <ucontext.h>
#include <functional>
#include "noncopyable.h"
// stCoRoutine_t
class co_routine :public noncopyable{
public:
    enum status{
        kStart,
        kEnd,
        kRunning,
        kSuspending
    };
    typedef std::function<void ()> Task;
    static const size_t stackSize = 2048;
    explicit co_routine(Task cb) ;
~co_routine();
    void resume();
    void yield();
    void release();
    void reset();
    void pfn(void *);
    static void func(void* this_);
private:
    char * stackMem_;
    ::ucontext_t current_{};
    ::ucontext_t ctx_{};  // libco stCoRoutineEnv_t
    Task cb_;
    status status_;


};


#endif //MY_LIBCO_CO_ROUTINE_H
