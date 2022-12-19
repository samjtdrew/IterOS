#pragma once

#include "../../drivers/vga.h"
#include "processes.h"
#include "../kernelUtils.h"
#include "sysError.h"

int startProcessMonitor() {
    clearScreen();
    #ifdef DISABLED_PROCMON
        printString("Error: ");
        printSysError(ERROR_PROCESS_DISABLED);
        printString("\n");
        return 0;
    #endif
	printString("---Starting Procmon---\nPress C to exit...");
    char keyInputDataBuf[DATA_BUF_SIZE];
    flag procMonKeyInput = {
        .processID = 4,
        .priority = 1,
        .data = keyInputDataBuf,
        .type = KEYBOARD_FLAG
    };
    int queued;
    queued = queueFlag(procMonKeyInput);
    if (queued != 0) {
        printString("Failed To Queue procMonKeyInput");
    }
    do {
        char processDataBuf[DATA_BUF_SIZE];
        processDataBuf[0] = '\0';
        flag procMon = {
            .processID = 3,
            .data = processDataBuf,
            .priority = 1,
            .type = MONITOR_FLAG
        };
        queueFlag(procMon);
        while (processDataBuf[0] == '\0') {};
        printString("PROCMON >> ");
        printString(processDataBuf);
        printString("\n");
        dequeueFlag((uint32_t)3);
    } while (compareString(keyInputDataBuf, "C") != 0);
    dequeueFlag((uint32_t)4);
    dequeueFlag((uint32_t)3);
    printString("Exiting PROCMON...\n");
    return 0;
}