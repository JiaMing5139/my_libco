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
using namespace std;

//
//void onMessageCoonection() {
//    cout <<"before read" << endl;
//    currentRunning->yield();
//    cout << "read event start read" << endl;
//}

struct task_t
{
    co_routine *co;
    int fd;
};

static stack<task_t*> g_readwrite;
static int g_listen_fd = -1;

void accpet_co(Socket  *socket,InetAddress & address){
    for(;;){
        int fd = co_accept_block(socket,address);
        LOG_INFO <<"get a new fd" << fd<< endl;
    }
}

void readwrite_co(int fd){

}




int main() {

   CoRoutineEnv:: init_CoRoutineEnv();

   Socket * socket1 = new Socket;
   InetAddress address(2333);
   socket1->setReuseAddr(true);
   socket1->setReusePort(true);
   socket1->setTcpNoDelay(true);
   socket1->bindAddress(address);
   socket1->listen();
   InetAddress peerAddr;
   co_routine co(std::bind(accpet_co,socket1,peerAddr));
   co.resume();

   auto  env = CoRoutineEnv::co_get_curr_thread_env();
   env->pEpoll->loop();
}