#include <iostream>
#include "util.h"
#include <ucontext.h>
#include "co_routine.h"
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
   init_env();
   auto * co = new co_routine([] { return onMessageCoonection(); });
   co->resume();
   cout << "read event re running co" << endl;
   co->resume();
}