#pragma once

#include "../kernelUtils.h"

//Define The Error Codes
#define ERROR_THREADING 0x01
#define ERROR_PROCESS_DISABLED 0x02


//To lookup error, supply buffer of 128
int getSysError(unsigned char errorCode, char* dataBuffer) {
    #define BUFFER 128
    switch (errorCode) {
        case ERROR_THREADING:
            stringCpy(dataBuffer, "System Threading Error", BUFFER);
            return 0;
        case ERROR_PROCESS_DISABLED:
            stringCpy(dataBuffer, "Process Is Currently Disabled", BUFFER);
            return 0;
        default:
            stringCpy(dataBuffer, "Unknown Error, Visit https://github.com/samjtdrew/IterOS For More Info", BUFFER);
            return -1;
    }
}

int printSysError(unsigned char errorCode) {
    char stringBuffer[128];
    getSysError(errorCode, stringBuffer);
    printString(stringBuffer);
    return 0;
}