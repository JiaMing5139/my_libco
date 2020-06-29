#include <iostream>
#include "util.h"
#include <ucontext.h>
#include "co_routine.h"
using namespace std;



static void main2() {
    cout<<"main2"<< endl;
    cout<<"main2"<< endl;
    cout<<"main2"<< endl;
}
int main() {
    co_routine coRoutine([] { return main2(); });
    coRoutine.resume();
}