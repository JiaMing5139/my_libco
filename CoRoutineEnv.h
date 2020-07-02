//
// Created by parallels on 7/3/20.
//

#ifndef MY_LIBCO_COROUTINEENV_H
#define MY_LIBCO_COROUTINEENV_H


#include "EventLoop.h"
#include <cassert>
class co_routine;
namespace CoRoutineEnv {
    typedef struct CoRoutineEnv_t {
        co_routine *pCallStack[128];
        int iCallStackSize;
        EventLoop *pEpoll;


        //for copy stack log lastco and nextco
        co_routine *pending_co;
        co_routine *occupy_co;
    } CoRoutineEnv_t;

    extern __thread CoRoutineEnv_t *gCoEnvPerThread;

    CoRoutineEnv_t* co_get_curr_thread_env();

    co_routine *GetCurrCo(CoRoutineEnv_t *env);

    co_routine *GetCurrThreadCo();

    void init_CoRoutineEnv();


}


#endif //MY_LIBCO_COROUTINEENV_H
