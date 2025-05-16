#include "../h/riscv.hpp"
#include "../h/printing.hpp"


void Riscv::handleSupervisorTrap(){

    uint64 volatile scause = r_scause();

    if(scause == 0x0000000000000008UL || scause == 0x0000000000000009UL){
        //interrupt: no; cause code: environment call from U-mode(8) or S-mode(9)

        unsigned volatile num;
        __asm__ volatile ("mv %0, a0" : "=r"(num));
        uint64 volatile arg1;
        __asm__ volatile ("mv %0, a1" : "=r"(arg1));
        uint64 volatile arg2;
        __asm__ volatile ("mv %0, a2" : "=r"(arg2));
        uint64 volatile arg3;
        __asm__ volatile ("mv %0, a3" : "=r"(arg3));

        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();

        uint64 retVal = -1;
        if(num == 0x01){
            //void* mem_alloc (size_t size);
            size_t size = arg1;
            uint64 ptr = reinterpret_cast<uint64>(MemoryAllocator::malloc(size));
            retVal = ptr;

        }else if(num == 0x02){
            //int mem_free (void* ptr);
            void* ptr = reinterpret_cast<void*>(arg1);
            retVal = MemoryAllocator::free(ptr);

        }else if(num == 0x11){
            //int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);
            TCB** handle = reinterpret_cast<TCB**>(arg1);
            TCB::Body body = reinterpret_cast<TCB::Body>(arg2);
            void* arg = reinterpret_cast<void*>(arg3);

            *handle = TCB::createThread(body, arg);

            if(*handle == nullptr){
                retVal = -1;
            }else{
                retVal = 0;
            }

        }else if(num == 0x12){
            //int thread_exit ();
            retVal = TCB::kill_running_thread();

        }else if(num == 0x13){
            //void thread_dispatch ();
            TCB::dispatch();

        }else if(num == 0x21){
            //int sem_open ( sem_t* handle, unsigned init);
            CSemaphore** handle = reinterpret_cast<CSemaphore**>(arg1);
            unsigned init = arg2;
            *handle = CSemaphore::create_sem(init);

            if(*handle == nullptr){
                retVal = -1;
            }else{
                retVal = 0;
            }

        }else if(num == 0x22){
            //int sem_close (sem_t handle);
            CSemaphore* handle = reinterpret_cast<CSemaphore*>(arg1);
            retVal = handle->close_sem();
            if(retVal == 0){
                handle = nullptr;
            }

        }else if(num == 0x23){
            // int sem_wait (sem_t id);
            CSemaphore* id = reinterpret_cast<CSemaphore*>(arg1);
            if(id == nullptr){
                // semafor je zatvoren
                retVal = -1;
            }else{
                retVal = id->wait();

            }

        }else if(num == 0x24){
            //int sem_signal (sem_t id);
            CSemaphore* id = reinterpret_cast<CSemaphore*>(arg1);
            if(id == nullptr){
                // semafor je zatvoren
                retVal = -1;
            }else{
                retVal = id->signal();
            }

        }else if(num == 0x25){
            //int sem_timedwait(sem_t id, time_t timeout);
            retVal = 0;
        }else if(num == 0x26){
            //int sem_trywait(sem_t id);
            CSemaphore* id = reinterpret_cast<CSemaphore*>(arg1);

            if(id == nullptr){
                // semafor je zatvoren
                 retVal = -1;
            }else{
                retVal = id->trywait();
            }

        }else if(num == 0x31){
            //int time_sleep (time_t);

        }else if(num == 0x41){
            //char getc ();
            retVal = (uint64)__getc();

        }else if(num == 0x42){
            //void putc (char);

            __putc(arg1);

        }else if(num == 0x55){
            //switch to U-mode
            Riscv::mc_sstatus(Riscv::SSTATUS_SPP);
            sstatus = Riscv::r_sstatus();
        }else{
            //nepoznat kod
        }

        TCB::running->retVal = retVal;
        sepc += 4;
        w_sstatus(sstatus);
        w_sepc(sepc);
    }else if(scause == 0x8000000000000001UL) {
        //timer
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        mc_sip(SIP_SSIE);
        w_sstatus(sstatus);
        w_sepc(sepc);

    }else if(scause == 0x8000000000000009UL){
        //console
        uint64 volatile sepc = r_sepc();
        uint64 volatile sstatus = r_sstatus();
        console_handler();
        w_sstatus(sstatus);
        w_sepc(sepc);

    }else{
            //unexpected trap cause
            if(scause == 0x0000000000000002UL){
                //Ilegalna instrukcija
                printString("Izuzetak: Ilegalna instrukcija\n");
            }else if(scause == 0x0000000000000005UL){
                //Nedozvoljena adresa čitanja
                printString("Izuzetak: Ilegalna adresa čitanja\n");
            }else if(scause == 0x0000000000000007UL){
                //Nedozvoljena adresa upisa
                printString("Izuzetak: Ilegalna adresa upisa\n");
            }
            while(true){}
    }

}

void Riscv::popSppSpie()
{
    mc_sip(SIP_SSIE);
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}