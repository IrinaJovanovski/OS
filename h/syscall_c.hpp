#ifndef SYSCALL_C_HPP
#define SYSCALL_C_HPP

#include "../lib/hw.h"
#include "../h/riscv.hpp"

typedef unsigned long time_t;
const int EOF = -1;

class TCB;
typedef TCB* thread_t;

class CSemaphore;
typedef CSemaphore* sem_t;

void* mem_alloc (size_t size);

int mem_free (void* ptr);

int thread_create (thread_t* handle, void(*start_routine)(void*), void* arg);

int thread_exit ();

void thread_dispatch ();

int sem_open ( sem_t* handle, unsigned init);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);

int sem_timedwait(sem_t id, time_t timeout);

int sem_trywait(sem_t id);

int time_sleep (time_t);

char getc ();

void putc (char);

#endif