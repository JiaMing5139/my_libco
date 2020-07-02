//
// Created by parallels on 7/3/20.
//

#include "CoRoutineEnv.h"
namespace CoRoutineEnv {

     __thread CoRoutineEnv_t* gCoEnvPerThread ;

    void init_CoRoutineEnv(){
        gCoEnvPerThread = (CoRoutineEnv_t*)calloc( 1, sizeof(CoRoutineEnv_t) );
        CoRoutineEnv_t *env = gCoEnvPerThread;

        env->iCallStackSize = 0;
        co_routine *self = new co_routine();
        self->setMain();

        env->pending_co = NULL;
        env->occupy_co = NULL;

        env->pCallStack[ env->iCallStackSize++ ] = self;
        auto *ev = new EventLoop;
        env->pEpoll = ev;
    }
}