#include <iostream>
#include "util.h"
#include <ucontext.h>
#include "co_routine.h"
using namespace std;


static ucontext_t ctx[2];
void onMessageCoonection() {
    cout <<"before read" << endl;
    currentRunning->yield();
    cout << "read event start read" << endl;

}
int main() {
   init_env();
   auto * co = new co_routine([] { return onMessageCoonection(); });
   co->resume();
   cout << "read event re running co" << endl;
   co->resume();
}