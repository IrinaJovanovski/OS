#ifndef MEMORY_ALLOCATOR_HPP
#define MEMORY_ALLOCATOR_HPP

#include "../lib/hw.h"
#include "../lib/mem.h"

struct Block{
    Block* next = nullptr;
    Block* prev = nullptr;
    size_t size;
};

class MemoryAllocator{
public:

    static void initMem();

    static void* malloc(size_t size);
    static int free(void* ptr);

private:

    static Block* freeMemHead;
    static Block* allocated;
    static bool init;
    static size_t ROUNDED_HBLOCK_SIZE;
};

#endif