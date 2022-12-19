#pragma once

#include <stdint.h>
#include <stddef.h>
#include "../../drivers/vga.h"
#include "../../drivers/idt.h"
#include "../../drivers/PS2keyboard.h"

#define MAX_FLAGS 1024
#define DATA_BUF_SIZE 512

//Flag Types
#define SYSTEM_FLAG 0x00
#define KEYBOARD_FLAG 0x01
#define MONITOR_FLAG 0x02
#define ERROR_FLAG 0xFF

//Disabled Processes
#define DISABLED_PROCMON
#define DISABLED_SECURE_LOGIN

typedef struct flag {
    uint32_t processID;
	uint32_t priority;
    uint32_t type;
    char* data;
} flag;

typedef struct flagQueue {
    flag flagData[MAX_FLAGS];
    uint32_t flagCount;
} flagQueue;

extern flagQueue processFlags;

int queueFlag(flag pending) {
    if (processFlags.flagCount < MAX_FLAGS) { //If the flag count has not maxed out
        processFlags.flagData[processFlags.flagCount] = pending; //Add the flag
        processFlags.flagCount++;
        return 0; //Success
    } else {
        return -1; //Failure
    }
}

int dequeueFlag(uint32_t processID) {
    for (size_t i = 0; i < processFlags.flagCount; i++) {
        if (processID == processFlags.flagData[i].processID) {
            while (i < (processFlags.flagCount - 1)) {
                processFlags.flagData[i] = processFlags.flagData[i + 1];
                i++;
            }
            processFlags.flagData[i] = (flag) {0, 0, 0, 0};
            processFlags.flagCount--;
            /*
            printString("Number Of Queued Processes: ");
            printString(itoa(processFlags.flagCount, 10));
            printString("\n");*/
        }
    }
}

flag findFlag(uint32_t process) {
    for (size_t i = 0; i < processFlags.flagCount; i++) {
        if (process == processFlags.flagData[i].processID) {
            return processFlags.flagData[i];
        }
    }
    return (flag) {0, 0, 0, 0};
}

uint32_t enumFlagsByType(flag* flagList, uint32_t type) {
    uint32_t flagTypeCount = 0;
    for (size_t i = 0; i < processFlags.flagCount; i++) {
        if (type == processFlags.flagData[i].type) {
            flagList[flagTypeCount] = processFlags.flagData[i];
            flagTypeCount++;
        }
    }
    return flagTypeCount;
}