#include "../h/MemoryAllocator.hpp"
#include "../h/syscall_c.hpp"

Block* MemoryAllocator::freeMemHead = nullptr;
Block* MemoryAllocator::allocated = nullptr;
size_t MemoryAllocator::ROUNDED_HBLOCK_SIZE = 0;
bool MemoryAllocator::init = false;

void MemoryAllocator::initMem(){

    freeMemHead = (Block*)HEAP_START_ADDR;
    size_t heap = (size_t)HEAP_END_ADDR - (size_t)HEAP_START_ADDR;
    freeMemHead->size = (heap % MEM_BLOCK_SIZE == 0) ? heap : (heap - heap % MEM_BLOCK_SIZE);
    ROUNDED_HBLOCK_SIZE = sizeof(Block) + MEM_BLOCK_SIZE - sizeof(Block) % MEM_BLOCK_SIZE;
    freeMemHead->size -= ROUNDED_HBLOCK_SIZE; // umanjen za velicinu zaglavlja (Block) zaokruzenu na cele blokove
    freeMemHead->prev = nullptr;
    freeMemHead->next = nullptr;
    init = true;

}


void* MemoryAllocator::malloc(size_t size){
    if(freeMemHead == nullptr){
        if(!init){
            initMem();
        }else{
            return nullptr;
        }
    }
    Block* blk = freeMemHead;
    Block* p = nullptr;
    size_t sz = (size % MEM_BLOCK_SIZE == 0) ? size : (size + MEM_BLOCK_SIZE - size % MEM_BLOCK_SIZE);
    // trazimo blok odgovarajuce velicine medju praznima
    for(;blk != nullptr; blk = blk->next){
        if(blk->size >= sz) break;
    }
    if(blk == nullptr) return nullptr;
    // updatujemo listu oslobodjenih
    if(blk->size == sz){
        // ako uzimamo ceo blok
        if(freeMemHead == blk){
            freeMemHead = blk->next;
            freeMemHead->prev = nullptr;
        }else{
            p = blk->prev;
            p->next = blk->next;
            if(blk->next != nullptr) blk->next->prev = p;
        }
    }else if(blk->size > sz && blk->size >= (sz + ROUNDED_HBLOCK_SIZE)){
        // ako uzimamo deo bloka, treba prelomiti i ostaviti ostatak u vidu bloka
        size_t remainingSize = blk->size - sz - ROUNDED_HBLOCK_SIZE;
        Block* remainingBlock = (Block*)((size_t)blk + ROUNDED_HBLOCK_SIZE + sz);
        if(!blk->prev){
            blk->size = sz;
            remainingBlock->next = freeMemHead->next;
            remainingBlock->prev = nullptr;
            remainingBlock->size = remainingSize;
            freeMemHead = remainingBlock;
            if(blk->next != nullptr) blk->next->prev = freeMemHead;
        }else{
            blk->size = sz;
            p = blk->prev;
            p->next = remainingBlock;
            remainingBlock->prev = p;
            if(blk->next != nullptr){
                blk->next->prev = remainingBlock;
                remainingBlock->next = blk->next;
            }
            remainingBlock->size = remainingSize;
        }
    }else{
        return nullptr;
    }
    blk->prev = blk->next = nullptr;
    //update liste alociranih
    if(allocated == nullptr){ // nijedan blok nije alociran
        allocated = blk;
        return (void*)((size_t)blk + ROUNDED_HBLOCK_SIZE);
    }
    p = nullptr;
    Block* curr = allocated;
    for(; curr != nullptr; curr = curr->next){
        if(blk < curr) break;
        p = curr;
    }
    if(curr == nullptr){ // bice poslednji u nizu alociranih blokova
        if(p != nullptr) p->next = blk;
        blk->prev = p;
    }else{
        if(p != nullptr) p->next = blk;
        blk->prev = p;
        blk->next = curr;
        curr->prev = blk;
    }
    return (void*)((size_t)blk + ROUNDED_HBLOCK_SIZE);

}

int MemoryAllocator::free(void* ptr){
    if(allocated == nullptr || !init || (size_t)ptr < (size_t)HEAP_START_ADDR || (size_t)ptr > (size_t)HEAP_END_ADDR) return -1;
    Block* freeBlocks = (Block*) ((size_t)ptr - ROUNDED_HBLOCK_SIZE); // blok koji treba osloboditi ako postoji u listi alociranih
    //izbacivanje iz allocated liste
    Block* curr = allocated;
    Block* p = nullptr;
    for(; freeBlocks != curr && (curr != nullptr); curr = curr->next){}
    if(curr == freeBlocks && curr != nullptr){
        if(curr->prev != nullptr) curr->prev->next = curr->next;
        if(curr->next != nullptr) curr->next->prev = curr->prev;
        curr->next = curr->prev = nullptr;
    }else{
        // ne postoji takav alociran blok
        return -1;
    }
    //ubacivanje u freeMemHead listu
    if(freeMemHead == nullptr) {
        freeMemHead = freeBlocks;
        freeMemHead->prev = freeMemHead->next = nullptr;
        return 0;
    }
    freeBlocks = curr;
    curr = freeMemHead;
    p = nullptr;
    for(; freeBlocks < curr && (curr != nullptr); curr = curr->next){ p = curr; }

    if(curr == freeMemHead){
        freeBlocks->next = freeMemHead;
        freeMemHead->prev = freeBlocks;
        freeMemHead = freeBlocks;
    }else if(curr == nullptr){
        p->next = freeBlocks;
        freeBlocks->prev = p;
    }else{
        p->next = freeBlocks;
        freeBlocks->prev = p;
        freeBlocks->next = curr;
        curr->prev = freeBlocks;
    }
    //try to join blocks in free list
    if(freeBlocks->prev != nullptr){
        p = freeBlocks->prev;
        if(((size_t)p + p->size + ROUNDED_HBLOCK_SIZE) == (size_t)freeBlocks){
            p->next = freeBlocks->next;
            p->next->prev = p;
            p->size = p->size + ROUNDED_HBLOCK_SIZE + freeBlocks->size;
            freeBlocks = p;
        }
    }
    if(freeBlocks->next != nullptr){
        p = freeBlocks->next;
        if(((size_t)freeBlocks + freeBlocks->size + ROUNDED_HBLOCK_SIZE) == (size_t)p){
            freeBlocks->next = p->next;
            freeBlocks->next->prev = freeBlocks;
            freeBlocks->size = freeBlocks->size + ROUNDED_HBLOCK_SIZE + p->size;
        }
    }
    return 0;
}

