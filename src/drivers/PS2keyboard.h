#pragma once

#include <stdint.h>
#include "vga.h"
#include "../kernel/terminal.h"
#include "keycodes.h"
#include "../kernel/system/processes.h"
#include "../kernel/kernelUtils.h"

#define KEYBOARD_KEYS 256
#define SC_MAX 57

static char keyBuffer[KEYBOARD_KEYS];

const char scancodeChar[] = {
	'*', '*', '1', '2', '3', '4', '5',
	'6', '7', '8', '9', '0', '-', '=',
	'*', '*', 'Q', 'W', 'E', 'R', 'T',
	'Y', 'U', 'I', 'O', 'P', '[', ']',
	'*', '*', 'A', 'S', 'D', 'F', 'G',
	'H', 'J', 'K', 'L', ';', '\\', '`',
	'*', '\\', 'Z', 'X', 'C', 'V', 'B',
	'N', 'M', ',', '.', '/', '*', '*',
	'*', ' '
};

int backspace(char buf[]) {
	int len = stringLength(buf);
	if (len > 0) {
		buf[len - 1] = '\0';
		return 0;
	} else {
		return 1;
	}
}

void printBackspace() {
	int newCursor = getCursor() - 2;
	setCharAtVideoMemory(' ', newCursor);
	setCursor(newCursor);
}

void keyboardCallback(registers_t *regs) {
	uint8_t scancode = portByteIn(0x60);	
	if (scancode > SC_MAX) return;
	if (scancode == BACKSPACE) {
		if (backspace(keyBuffer) == 0) {
			printBackspace();
		}
	} else if (scancode == ENTER) {
		printString("\n"); //Jump to next line (What enter is supposed to do)
		flag processes[MAX_FLAGS]; //Set up array of flags
		uint32_t flagCount;
		flagCount = enumFlagsByType(processes, KEYBOARD_FLAG); //Find all set flags for keyboard interrupts
		/*
		printString("Flag Count: ");
		printString(itoa(flagCount, 10)); //Display how many flags were found
		printString("\n");
		*/
		flag monitors[MAX_FLAGS];
		uint32_t monCount;
		monCount = enumFlagsByType(monitors, MONITOR_FLAG);
		for (size_t i = 0; i < flagCount; i++) {
			//For each flag found, copy the keybuffer to the specified memory
			uint32_t dataBuffer = DATA_BUF_SIZE;
			if (dataBuffer < (sizeof(keyBuffer) / sizeof(char))) { //Check that the data buffer is large enough to hold the input
				printString("Buffer Overflow\n");
			} else {
				size_t j;
				for (j = 0; j < dataBuffer; j++) {
					processes[i].data[j] = keyBuffer[j]; //Copy each character to the local data buffer
				}
				processes[i].data[j] = '\0';
			}
			//Check the processIDs for individual executions
			switch (processes[i].processID) {
				case (uint32_t)1:
					break;
				case (uint32_t)2:
					executeCommand();
					break;
				default:
					printString("ProcessID:");
					printString(itoa(processes[i].processID, 10));
					printString(" Unknown, Please Update System\n");
			}
			//If there are monitors, feed them data
			char dataToFeed[DATA_BUF_SIZE] = "KEYBOARD_FLAG Process Updated...\n"; //Displays PROCMON >> KEYBOARD_FLAG Process Updated...
			for (size_t k = 0; k < monCount; k++) { //For each monitor
				for (size_t i = 0; i < DATA_BUF_SIZE; i++) {
					monitors[k].data[i] = dataToFeed[i];
				}
			}
		}
		keyBuffer[0] = '\0';
	} else if (scancode == ESCAPE) {
		clearScreen();
		printString("\nShutting Down...\n");
		asm volatile("hlt");
	} else {
		char letter = scancodeChar[(int)scancode];
		append(keyBuffer, letter);
		char str[2] = {letter, '\0'};
		printString(str);
	}
}

void initPS2keyboard() {
	registerInterruptHandler(IRQ1, keyboardCallback);
}