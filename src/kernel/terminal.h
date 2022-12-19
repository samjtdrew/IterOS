#pragma once

#include "kernelUtils.h"
#include "../drivers/vga.h"
#include "system/processes.h"
#include "system/procMon.h"

char terminalInputBuffer[DATA_BUF_SIZE];

void executeCommand() {
	dequeueFlag((uint32_t)2);
	if (compareString(terminalInputBuffer, "") == 0) {
	} else if (compareString(terminalInputBuffer, "EXIT") == 0) {
		printString("Terminating All Running Processes...\nStopping The CPU\0");
		asm volatile("hlt");
	} else if (compareString(terminalInputBuffer, "HELLO WORLD") == 0) {
		printString("Hello World\n");
	} else if (compareString(terminalInputBuffer, "CREDITS") == 0) {
		clearScreen();
		printString("Itergen Systems Ltd\n");
		printString("IterOS - Developed By Sam Drew CEO\n");
		printString("This operating system was developed by Itergen under the codename Project Calculus\n");
	} else if ((compareString(terminalInputBuffer, "SAM") == 0) || (compareString(terminalInputBuffer, "TOM") == 0) || (compareString(terminalInputBuffer, "LUKE") == 0)) {
		printString("Nerd >:)\n");
	} else if (compareString(terminalInputBuffer, "CLEAR") == 0) {
		clearScreen();
	} else if (compareString(terminalInputBuffer, "REBOOT") == 0) {
		printString("Reboot requested.");
		//Jump back into the bootloader and move to reboot initiater
		//systemReboot();
		printString("\nReboot Failed: SYSSEC-ERROR\nTerminate Power To System Immediately...\n");
	} else if (compareString(terminalInputBuffer, "PROCMON") == 0) {
		startProcessMonitor();
	} else {
		printString("Unknown Command: ");
		printString(terminalInputBuffer);
		printString("\n");
	}
	printString(">> ");
	flag terminalInput = {
		.processID = 2,
		.priority = 1,
		.type = KEYBOARD_FLAG,
		.data = terminalInputBuffer
	};
	queueFlag(terminalInput);
}

void initTerminal() {
	flag terminalInput = {
		.processID = 2,
		.priority = 1,
		.type = KEYBOARD_FLAG,
		.data = terminalInputBuffer
	};
	queueFlag(terminalInput);
}
