
#ifndef TCB_HPP
#define TCB_HPP

#include "../lib/hw.h"
#include "../h/scheduler.hpp"
#include "../h/MemoryAllocator.hpp"

// Thread Control Block
class TCB
{
public:

    ~TCB() { if(isFinished()) MemoryAllocator::free(stack); }

    bool isFinished() const { return finished; }

    bool isMain() const {return (body == nullptr);}

    bool hasStarted() const { return started; }

    bool blocked() const { return isBlocked; }

    void setBlocked( bool value) { isBlocked = value; }

    void setRetValue(uint64 v) { retVal = v; }

    void setFinished(bool value) { finished = value; }

    uint64 getRetValue() const {return retVal; }

    uint64 getTimeSlice() const { return timeSlice; }

    using Body = void (*)(void*);

    static TCB *createThread(Body body, void* arg);

    static void yield();

    static TCB *running;

    static int numOfRunningThreads;

private:
    TCB(Body body, void* arg){
        this->body = body;
        this->args = arg;
        this->timeSlice = DEFAULT_TIME_SLICE;
        this->finished = false;
        this->isBlocked = false;
        this->retVal = 0;
        this->stack = (body != nullptr ? ((uint64*)MemoryAllocator::malloc(DEFAULT_STACK_SIZE)) : nullptr);
        (this->context).ra = ((uint64) &threadWrapper);
        (this->context).sp = (this->stack != nullptr ? ((uint64) &((this->stack)[DEFAULT_STACK_SIZE])) : 0);
        this->tHandle = this;
        this->started = false;
        if(body == nullptr && running == nullptr){
            //int main();
            TCB::running = this;
        }else{
            Scheduler::put(this);
        }
    }

    struct Context
    {
        uint64 ra;
        uint64 sp;
    };

    TCB* tHandle;
    Body body;
    void* args;
    uint64 *stack;
    Context context;
    uint64 timeSlice;
    bool started;
    bool finished;
    bool isBlocked;
    uint64 retVal;


    friend class Riscv;

    static void threadWrapper();

    static int kill_running_thread();

    static void contextSwitch(Context *oldContext, Context *runningContext);

    static void dispatch();

    static uint64 timeSliceCounter;
};

#endif
