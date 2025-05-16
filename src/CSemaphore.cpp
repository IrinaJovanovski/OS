#include "../h/CSemaphore.hpp"

CSemaphore* CSemaphore::create_sem(unsigned init){
    CSemaphore* sem = (CSemaphore*) MemoryAllocator::malloc(sizeof(CSemaphore));
    if(sem == nullptr) return nullptr;
    sem->val = (int)init;
    sem->blocked.initList();
    return sem;
}

int CSemaphore::close_sem() {
    while(blocked.peekFirst() != nullptr){
        unblock();
    }
    MemoryAllocator::free(this);
    return 0;
}

int CSemaphore::wait(){
    if(--val < 0) block();
    if((TCB::running)->blocked()){
        return -1;
    }
    return 0;
}

int CSemaphore::trywait() {
    if(val >= 1){
        val--;
        return 0;
    }else{
        return -1;
    }
}

int CSemaphore::signal(){
    if(++val <= 0) {
        return unblock();
    }
    return 0;
}

void CSemaphore::block(){
    TCB* old = TCB::running;
    old->setBlocked(true);
    blocked.addLast(old);
    TCB::yield();
}

int CSemaphore::unblock(){
    TCB* t = blocked.removeFirst();
    if(t == nullptr) return -1;
    t->setBlocked(false);
    Scheduler::put(t);
    TCB::yield();
    return 0;
}
