#include "memory.h"
#include "OS.h"

#define MEMORY_SIZE 4096

struct memoryBlock {
    uint16_t size;
    memoryBlock * next;
    memoryBlock * prev;

    memoryBlock(){
        size = 0;
        next = 0;
        prev = 0;
    }

    
};

int blockCount = 0;

uint16_t address(memoryBlock * block){ 
    return (uint16_t)((uint16_t)block + (uint16_t)sizeof(memoryBlock)); 
}

int freeMemory = 0;

// TODO :
// MemoryBlocks are at the start of a block in memory
// Remove address from struct

memoryBlock * firstBlock = 0;
memoryBlock * lastBlock = 0;

uint16_t getFreeRam() {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void * malloc(uint16_t size){
    if(size == 0) return NULL;
    if(!firstBlock) {
        uint16_t freeRam = getFreeRam();                    
        if(freeRam < size) return NULL;                     
        firstBlock = (memoryBlock *)(MEMORY_SIZE - freeRam);     
        firstBlock->size = size;                            
        firstBlock->next = 0;                               
        firstBlock->prev = 0;                               
        lastBlock = firstBlock;      
        freeMemory = freeRam - size;              
        blockCount++;          
        print(" 1-- Allocating memory at address: ");
        println(itoa((int)address(firstBlock)));
        print("Block count: "); 
        println(itoa(blockCount));
        return (void *)(address(firstBlock));               
    }
    memoryBlock * block = firstBlock;
    while(block){
        if(block->next){
            if(address(block->next) - (address(block) + block->size) >= size){
                memoryBlock * newBlock = (memoryBlock *)(address(block) + block->size);
                newBlock->size = size;
                newBlock->next = block->next;
                newBlock->prev = block;
                block->next->prev = newBlock;
                block->next = newBlock;
                freeMemory -= size;
                blockCount++;
                print(" 2 --- Allocating memory at address: ");
                println(itoa((int)address(newBlock)));
                print("Block count: "); 
                println(itoa(blockCount));
                return (void *)address(newBlock);
            }
        } else {
            if(MEMORY_SIZE - (address(block) + block->size) >= size){
                memoryBlock * newBlock = (memoryBlock *)(address(block) + block->size);
                newBlock->size = size;
                newBlock->next = 0;
                newBlock->prev = block;
                block->next = newBlock;
                lastBlock = newBlock;
                freeMemory -= size;
                blockCount++;
                print(" 3 - --- Allocating memory at address: ");
                println(itoa((int)address(newBlock)));
                print("Block count: "); 
                println(itoa(blockCount));
                return (void *)address(newBlock);
            }
        }
        block = block->next;
    }
    return NULL;
}

void free(void * ptr){
    if(!ptr) ThrowException(EXCEPTION_ARGUMENT_ERROR, __LINE__, __FILE__, "trying to free a null pointer", 0);
    memoryBlock * block = firstBlock;
    while(block){
        if(address(block) == (uint16_t)ptr){
            if(block->prev) block->prev->next = block->next;
            if(block->next) block->next->prev = block->prev;
            if(block == firstBlock) firstBlock = block->next;
            if(block == lastBlock) lastBlock = block->prev;
            blockCount--;
            return;
        }
        block = block->next;
    }
}

void memset(void * ptr, uint8_t value, uint16_t size){
    if(!ptr) ThrowException(EXCEPTION_ARGUMENT_ERROR, __LINE__, __FILE__, "ptr is null", 0);
    uint8_t * p = (uint8_t *)ptr;
    for(int i = 0; i < size; i++){
        *p++ = value;
    }
}

void memcpy(void * dest, void * src, uint16_t size){
    if(!dest){
        ThrowException(EXCEPTION_ARGUMENT_ERROR, __LINE__, __FILE__, "dest is null", 0);
        return;
    } 
    if(!src){
        ThrowException(EXCEPTION_ARGUMENT_ERROR, __LINE__, __FILE__, "src is null", 0);
        return;
    } 
    uint8_t * d = (uint8_t *)dest;
    uint8_t * s = (uint8_t *)src;
    for(int i = 0; i < size; i++){
        *d++ = *s++;
    }
}