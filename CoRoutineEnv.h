//
// Created by parallels on 7/3/20.
//

#ifndef MY_LIBCO_COROUTINEENV_H
#define MY_LIBCO_COROUTINEENV_H

#include "co_routine.h"
#include "EventLoop.h"
namespace CoRoutineEnv {
    typedef  struct  CoRoutineEnv{
        co_routine *pCallStack[ 128 ];
        int iCallStackSize;
        EventLoop *pEpoll;


        //for copy stack log lastco and nextco
        co_routine* pending_co;
        co_routine* occupy_co;
    } CoRoutineEnv_t;


    void init_CoRoutineEnv();
    inline CoRoutineEnv_t * CoRoutineEnv();



    extern __thread CoRoutineEnv_t* gCoEnvPerThread;

};


#endif //MY_LIBCO_COROUTINEENV_H
