//
// Created by parallels on 7/3/20.
//

#include "CoRoutineEnv.h"
#include "co_routine.h"
namespace CoRoutineEnv {

     __thread CoRoutineEnv_t* gCoEnvPerThread ;

    void init_CoRoutineEnv(){
        gCoEnvPerThread = (CoRoutineEnv_t*)calloc( 1, sizeof(CoRoutineEnv_t) );
        CoRoutineEnv_t *env = gCoEnvPerThread;

        env->iCallStackSize = 0;
        co_routine *self = new co_routine();
        self->setMain();

        env->pending_co = nullptr;
        env->occupy_co = nullptr;

        env->pCallStack[ env->iCallStackSize++ ] = self;
        auto *ev = new EventLoop;
        env->pEpoll = ev;
    }

    CoRoutineEnv_t* co_get_curr_thread_env(){
        return gCoEnvPerThread;
    }

    co_routine *GetCurrCo( CoRoutineEnv_t *env )
    {
        return env->pCallStack[ env->iCallStackSize - 1 ];
    }

    co_routine *GetCurrThreadCo( )
    {
        CoRoutineEnv_t *env = co_get_curr_thread_env();
        if( !env ) return nullptr;
        return GetCurrCo(env);
    }
}