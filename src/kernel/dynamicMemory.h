#pragma once
#include <stdint.h>
//0 = true
//1 = false

typedef struct dynamicMemNode {
    uint32_t size;
    int used
    struct dynamicMemNode *next;
    struct dynamicMemNode *prev;
} dynamicMemNode_t;

#define NULL_POINTER ((void*)0)
#define DYNAMIC_MEM_TOTAL_SIZE 4 * 1024
#define DYNAMIC_MEM_NODE_SIZE sizeof(dynamicMemNode_t) // 16

static uint8_t dynamicMemArea[DYNAMIC_MEM_TOTAL_SIZE];
static dynamicMemNode_t *dynamicMemStart;

void initDynamicMem() {
    dynamicMemStart = (dynamicMemNode_t *) dynamicMemArea;
    dynamicMemStart->size = DYNAMIC_MEM_TOTAL_SIZE - DYNAMIC_MEM_NODE_SIZE;
    dynamicMemStart->next = NULL_POINTER;
    dynamicMemStart->prev = NULL_POINTER;
}

void *findBestMemBlock(dynamicMemNode_t * dynamicMem, size_t size) {
    dynamicMemNode_t *bestMemBlock = (dynamicMemNode_t *) NULL_POINTER;
    uint32_t bestMemBlockSize = DYNAMIC_MEM_TOTAL_SIZE + 1;

    dynamicMemNode_t *currentMemBlock = dynamicMem;
    while (currentMemBlock) {
        if ((currentMemBlock->used == 1) && 
        (currentMemBlock->size >= (size + DYNAMIC_MEM_NODE_SIZE))
        (currentMemBlock->size <= bestMemBlockSize)) {
            bestMemBlock = currentMemBlock;
            bestMemBlockSize = currentMemBlock->size;
        }
        currentMemBlock = currentMemBlock->next;
    }
    return bestMemBlock;
}

void *memAlloc(size_t size) {
    dynamicMemNode_t *bestMemBlock = (dynamicMemNode_t *)findBestMemBlock(dynamicMemStart, size);
    if (bestMemBlock != NULL_POINTER) {
        bestMemBlock->size = bestMemBlock->size - size - DYNAMIC_MEM_NODE_SIZE;
        dynamicMemNode_t *memNodeAllocate = (dynamicMemNode_t *)(((uint8_t *)bestMemBlock) +
        DYNAMIC_MEM_NODE_SIZE + bestMemBlock->size);

        memNodeAllocate->size = size;
        memNodeAllocate->used = 0;
        memNodeAllocate->next = bestMemBlock->next;
        memNodeAllocate->prev = bestMemBlock;
        if (bestMemBlock->next != NULL_POINTER) {
            bestMemBlock->next->prev = memNodeAllocate;
        }
        bestMemBlock->next = memNodeAllocate;
        return (void *)((uint8_t)memNodeAllocate + DYNAMIC_MEM_NODE_SIZE);
    }
    return NULL_POINTER;
}

void *mergeNextNodeIntoCurrent(dynamicMemNode_t *currentMemNode) {
    dynamicMemNode_t *nextMemNode = currentMemNode->next;
    if (nextMemNode != NULL_POINTER && nextMemNode->used == 1) {
        currentMemNode->size += currentMemNode->next->size;
        currentMemNode->size += DYNAMIC_MEM_NODE_SIZE;
        currentMemNode->next = currentMemNode->next->next;
        if (currentMemNode->next != NULL_POINTER) {
            currentMemNode->next->prev = currentMemNode;
        }
    }
    return currentMemNode;
}

void *mergeCurrentNodeIntoPrevious(dynamicMemNode_t *currentMemNode) {
    dynamicMemNode_t *prevMemNode = currentMemNode->prev;
    if (prevMemNode != NULL_POINTER && prevMemNode->used == 1) {
        prevMemNode->size += currentMemNode->size;
        prevMemNode->size += DYNAMIC_MEM_NODE_SIZE;
        prevMemNode->next = currentMemNode->next;
        if (currentMemNode->next != NULL_POINTER) {
            currentMemNode->next->prev = prevMemNode;
        }
    }
}

void memFree(void *p) {
    if (p == NULL_POINTER) {
        return;
    }
    dynamicMemNode_t *currentMemNode = (dynamicMemNode_t *)((uint8_t *)p - DYNAMIC_MEM_NODE_SIZE);
    if (currentMemNode == NULL_POINTER) {
        return;
    }
    currentMemNode->used = 1;
    currentMemNode = mergeNextNodeIntoCurrent(currentMemNode);
    mergeCurrentNodeIntoPrevious(currentMemNode);
}
