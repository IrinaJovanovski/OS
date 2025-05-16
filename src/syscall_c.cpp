#include "../h/syscall_c.hpp"

void* mem_alloc (size_t size){
    // size je u bajtovima
    if(size <= 0) return nullptr;
    // a0 = 0x01

    __asm__ volatile ("mv a1, %0" : : "r"(size));
    __asm__ volatile ("li a0, 0x01");

    __asm__ volatile("ecall");

    return (void*)(TCB::running->getRetValue());
}

int mem_free (void* ptr){
    // a0 = 0x02

    __asm__ volatile ("mv a1, %0" : : "r"(ptr));
    __asm__ volatile ("li a0, 0x02");

    __asm__ volatile("ecall");

    return (int)(TCB::running->getRetValue());
}

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg){
    // a0 = 0x11

    __asm__ volatile ("mv a3, %0" : : "r"(arg));
    __asm__ volatile ("mv a2, %0" : : "r"(start_routine));
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x11");

    __asm__ volatile("ecall");

    if(TCB::running->getRetValue() == 0){
        return 0;
    }else{
        return -1;
    }
}

int thread_exit (){
    // a0 = 0x12
    __asm__ volatile ("li a0, 0x12");

    __asm__ volatile("ecall");

    if(TCB::running->getRetValue() == 0){
        return 0;
    }else{
        return -1;
    }
}

void thread_dispatch (){
    // a0 = 0x13

    __asm__ volatile ("li a0, 0x13");

    __asm__ volatile("ecall");


}

int sem_open ( sem_t* handle, unsigned init){
    // a0 = 0x21
    __asm__ volatile ("mv a2, %0" : : "r"(init));
    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x21");

    __asm__ volatile("ecall");

    if(TCB::running->getRetValue() == 0){
        return 0;
    }else{
        return -1;
    }
}

int sem_close (sem_t handle){
    // a0 = 0x22

    __asm__ volatile ("mv a1, %0" : : "r"(handle));
    __asm__ volatile ("li a0, 0x22");

    __asm__ volatile("ecall");

    if(TCB::running->getRetValue() == 0){
        return 0;
    }else{
        return -1;
    }
}

int sem_wait (sem_t id){
    // a0 = 0x23

    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x23");

    __asm__ volatile("ecall");


    if(TCB::running->getRetValue() == 0){
        return 0;
    }else{
        return -1;
    }
}

int sem_signal (sem_t id){
    // a0 = 0x24

    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x24");

    __asm__ volatile("ecall");

    if(TCB::running->getRetValue() == 0){
        return 0;
    }else{
        return -1;
    }
}

int sem_timedwait (sem_t id, time_t timeout){
    // a0 = 0x25
    /* __asm__ volatile ("li a0, 0x25");
     // a1 = id
     __asm__ volatile("mv a1, %0" : : "r"(((uint64)id)));
     // a2 = timeout
     __asm__ volatile("mv a2, %0" : : "r"(((uint64)timeout)));
     __asm__ volatile("ecall");

     if(Riscv::r_a0() == 0){
         return 0;
     }else{
         return -1;
     }*/
    return 0;
}

int sem_trywait (sem_t id){
    // a0 = 0x26

    __asm__ volatile ("mv a1, %0" : : "r"(id));
    __asm__ volatile ("li a0, 0x26");

    __asm__ volatile("ecall");

    if(TCB::running->getRetValue() == 0){
        return 0;
    }else{
        return -1;
    }
}

int time_sleep (time_t t){
    // a0 = 0x31
    /* __asm__ volatile ("li a0, 0x31");
     // a1 = t
     __asm__ volatile("mv a1, %0" : : "r"(((uint64)t)));
     __asm__ volatile("ecall");

     if(Riscv::r_a0() == 0){
         return 0;
     }else{
         return -1;
     }*/
    return 0;
}

char getc (){
    // a0 = 0x41

    __asm__ volatile ("li a0, 0x41");

    __asm__ volatile("ecall");

    if(TCB::running->getRetValue() == (uint64)EOF){
        return -1;
    }else{
        return (char)(TCB::running->getRetValue());
    }

}

void putc (char c){
    // a0 = 0x42
    __asm__ volatile ("mv a1, %0" : : "r"(c));

    __asm__ volatile ("li a0, 0x42");

    __asm__ volatile("ecall");

}
