#include <iostream>
#include "util.h"
#include <ucontext.h>
#include "co_routine.h"
using namespace std;


static ucontext_t ctx[2];
void main21() {
    cout <<"co in co 1" << endl;
}
int main() {
   init_env();
   co_routine co([] { return main21(); });
   co.resume();
}