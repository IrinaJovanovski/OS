#ifndef C_SEM
#define C_SEM


#include "../h/tcb.hpp"
#include "../h/MemoryAllocator.hpp"
#include "../h/list.hpp"

#include "../h/syscall_c.hpp"

class CSemaphore{
public:
    void* operator new (size_t size){
        return MemoryAllocator::malloc(size);
    }

    void operator delete(void *ptr) {
        MemoryAllocator::free(ptr);
    }

    static CSemaphore* create_sem(unsigned init = 1);

    int close_sem();

    int wait();
    int signal();
    int trywait();
    int value() const { return val;}

protected:
    void block();
    int unblock();

private:
    int val;
    List<TCB> blocked;
};

#endif