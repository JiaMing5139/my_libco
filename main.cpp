#include <iostream>
#include "util.h"
#include <ucontext.h>
#include "co_routine.h"
#include "CoRoutineEnv.h"
#include "SocketsOps.h"
#include "Socket.h"
#include <stack>
#include "Scheduler.h"
#include "log/logger.h"
#include <unistd.h>

using namespace std;

struct task_t {
    co_routine *co;
    int fd;
};

static stack<task_t *> g_readwrite;
static int g_listen_fd = -1;

void accpet_co(int lisenfd, InetAddress &address) {
    for (;;) {
        if( g_readwrite.empty())
        {
            //     close( fd );
            co_sleep(3);
            cout <<"empty" << endl;
            continue;
        }

        int fd = co_accept_block( lisenfd, address,3);
        LOG_INFO << "accept get" << fd << endl;
        cout <<"accept get" << fd << endl;
        if(fd < 0){
            continue;
        }
        if( g_readwrite.empty() )
        {
            close( fd );
            continue;
        }


        task_t *co = g_readwrite.top();
        co->fd = fd;
        g_readwrite.pop();
        co->co->resume();
    }
}

void readwrite_co(task_t *task) {
    char buf[1024 * 16];
    for (;;) {
        if (-1 == task->fd) {
            g_readwrite.push(task);
            task->co->yield();
            continue;
        }
        int fd = task->fd;
        task->fd = -1;

        for(;;)
        {
            int ret = co_read_block( fd,buf,sizeof(buf),3);
            cout <<"a"<< buf  << endl;
            if( ret > 0 )
            {
                ret = co_wirte_block( fd,buf,ret ,3);
            }
            if( ret > 0 || ( -1 == ret && EAGAIN == errno ) )
            {
                continue;
            }
            co_close( fd );
            break;
        }

    }
}


int main() {

    Jimmy::Logger::setLevel(Jimmy::Logger::CLOSEED);
    CoRoutineEnv::init_CoRoutineEnv();
//    for(int k = 0; k <3 ;k++){

//        pid_t pid = fork();
//        if(pid > 0){
//            continue;
//        } else if(pid < 0){
//            break;
//        }
        Socket *socket1 = new Socket;
        InetAddress address(2334);
        socket1->setReuseAddr(true);
        socket1->setReusePort(true);
        socket1->setTcpNoDelay(true);
        socket1->bindAddress(address);
        socket1->listen();
        InetAddress peerAddr;
        for (int i = 0; i < 10000; i++) {
            task_t *task2 = (task_t *) calloc(1, sizeof(task_t));
            task2->fd = -1;
            task2->co = new co_routine(std::bind(readwrite_co, task2));
            task2->co->resume();
        }
        co_routine co(std::bind(accpet_co, socket1->fd(), peerAddr));
        co.resume();
        auto env = CoRoutineEnv::co_get_curr_thread_env();
        env->pEpoll->loop();
        exit(0);
//   }



}