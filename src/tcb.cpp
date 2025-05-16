
#include "../h/tcb.hpp"
#include "../h/riscv.hpp"
#include "../h/printing.hpp"

TCB *TCB::running = nullptr;

int TCB::numOfRunningThreads = 0;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body, void* arg)
{
    TCB* tcb = (TCB*) MemoryAllocator::malloc(sizeof(TCB));
    if(tcb == nullptr) return tcb;
    tcb->body = body;
    tcb->started = false;
    tcb->args = arg;
    tcb->timeSlice = DEFAULT_TIME_SLICE;
    tcb->finished = false;
    tcb->isBlocked = false;
    tcb->retVal = 0;
    tcb->stack = (body != nullptr ? ((uint64*)MemoryAllocator::malloc(DEFAULT_STACK_SIZE)) : nullptr);
    (tcb->context).ra = ((uint64) &threadWrapper);
    (tcb->context).sp = (tcb->stack != nullptr ? ((uint64) &(tcb->stack[DEFAULT_STACK_SIZE/sizeof(uint64)]) ) : 0);
    tcb->tHandle = tcb;
    if(body == nullptr && running == nullptr){
        //int main();
        TCB::running = tcb;
    }else{
        Scheduler::put(tcb);
    }
    numOfRunningThreads++;
    return tcb;
}

void TCB::yield()
{
   __asm__ volatile("li a0,0x13");
    __asm__("ecall");

}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked) Scheduler::put(old);
    running = Scheduler::get();

    TCB::contextSwitch(&(old->context), &(running->context));
}

void TCB::threadWrapper()
{
    Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
    Riscv::popSppSpie();

    running->started = true;

    if(!running->isMain()){
        running->body(running->args);
        running->setFinished(true);
    }
    if(--numOfRunningThreads == 0) return;

    TCB::yield();
}

int TCB::kill_running_thread(){
    if(running->finished){
        return -1;
    }
    running->setFinished(true);
    return 0;
}
