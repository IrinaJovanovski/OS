#include "../h/syscall_cpp.hpp"
#include "../lib/console.h"
#include "../lib/hw.h"

extern void userMain();

void _userMain(void*){
    userMain();
}

void main(){

    Riscv::w_stvec((uint64)&Riscv::supervisorTrap);

    TCB* mainThread;
    int val = thread_create(&mainThread, nullptr, nullptr);
    //printInt(val);
    if(val) return;

    TCB* threadOne;
    int val1 = thread_create(&threadOne, &(_userMain), nullptr);
    //printInt(val1);
    if(val1) return;

    //TCB* mainThread = TCB::createThread( nullptr, nullptr);
    //TCB* threadOne = TCB::createThread( &(_userMain), nullptr);
    //Riscv::mc_sip(Riscv::SIP_SSIE);
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE);

    while(!threadOne->isFinished()){
            thread_dispatch();
    }
    mainThread->setFinished(true);
}
