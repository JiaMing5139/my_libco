#include <iostream>
#include "util.h"
#include <ucontext.h>
#include "co_routine.h"
#include "CoRoutineEnv.h"
using namespace std;


void onMessageCoonection() {
    cout <<"before read" << endl;
    currentRunning->yield();
    cout << "read event start read" << endl;
}

void accpet_co(int fd){

}

void readwrite_co(int fd){

}




int main() {
   CoRoutineEnv:: init_CoRoutineEnv();
   auto * co = new co_routine([] { return onMessageCoonection(); });
   co->resume();
   cout << "read event re running co" << endl;
   co->resume();
   delete co;
}