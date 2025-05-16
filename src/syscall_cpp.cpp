#include "../h/syscall_cpp.hpp"

void* operator new (size_t size){
    return mem_alloc(size);
}

void operator delete (void* obj){
    mem_free(obj);
}

Thread::Thread (void (*body)(void*), void* arg){
    this->body = body;
    this->arg = arg;
    this->myHandle = nullptr;
}

Thread::~Thread (){
    thread_exit();
    (this->myHandle)->~TCB();
}

int Thread::start (){

    thread_create(&(this->myHandle), this->body, this->arg);

    if(this->myHandle != nullptr){
        return 0;
    }else{
        return -1;
    }
}

void Thread::dispatch (){
    thread_dispatch();
}

int Thread::sleep (time_t t){
    //???
    return 0;
}

void Thread::_runWrapper(void* t){
    Thread* thread = (Thread*) t;
    thread->run();
    thread->~Thread();
}

Thread::Thread (){

    this->body = (&_runWrapper);
    this->arg = this;
}

Semaphore::Semaphore (unsigned init){
    sem_open(&(this->myHandle), init);
}

Semaphore::~Semaphore (){
    sem_close(this->myHandle);
    (this->myHandle)->~CSemaphore();
}

int Semaphore::wait (){
    return sem_wait(this->myHandle);
}

int Semaphore::signal (){
    return sem_signal(this->myHandle);
}

int Semaphore::timedWait (time_t t){
    //???
    return 0;
}

int Semaphore::tryWait(){
    return sem_trywait(this->myHandle);
}


char Console::getc (){
    return ::getc();
}

void Console::putc (char c){
    ::putc(c);
}
