#pragma once

//System Security Header
#include "../../drivers/vga.h"
#include "../../drivers/idt.h"
#include "../../drivers/PS2keyboard.h"
#include "processes.h"
#include "../kernelUtils.h"

#define TEST_PASSCODE "PASSWORD"

int initLoginDaemon() {
    #ifdef DISABLED_SECURE_LOGIN
        printString("Error: ");
        printSysError(ERROR_PROCESS_DISABLED);
        printString("\n");
        return 0;
    #endif
    char username[DATA_BUF_SIZE]; //Setting a memory location to store flag data
    username[0] = '\0';
    flag awaitLogin = { //Flag to tell keyboard that it is awaiting input
        .priority = 1,
        .processID = 1,
        .type = KEYBOARD_FLAG,
        .data = username
    };
    int query = queueFlag(awaitLogin); //Add the prepared flag to the queue
    if (query != 0) { //Check that the flag was queued
        printString("Queued awaitLogin Failed To Be Queued\n");
    }
    printString("User: ");
    int timeout = 0; //Timeout var to record how many cycles it took to recieve input
    while (username[0] == '\0') { //Wait until data has been input to the flag buffer
        //timeout++;
    }
    dequeueFlag((uint32_t)1);
    if (compareString(username, "SAM") == 0) {
        char password[DATA_BUF_SIZE];
        password[0] = '\0';
        flag awaitPassword = {
            .priority = 1,
            .processID = 1,
            .type = KEYBOARD_FLAG,
            .data = password
        };
        int query = queueFlag(awaitPassword);
        if (query != 0) {
            printString("Queued awaitPassword Failed To Be Queued\n");
        }
        printString("Passcode: ");
        while (password[0] == '\0') {}
        dequeueFlag((uint32_t)1);
        if (compareString(password, TEST_PASSCODE) == 0) {
            clearScreen();
            printString("Welcome ");
            printString(username); //Once input is recieved, print the input
            printString("\n");
            return 0;
        } else {
            printString("Incorrect Password...\n");
            return -1;
        }
    } else {
        printString("User Does Not Exist...\n");
        return -1;
    }
    return -1;
    //printString("Time Till Interrupt: ");
    //printString((char*) timeout);
    //printString("\n");
}