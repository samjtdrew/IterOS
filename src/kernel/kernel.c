#include <stdint.h>
#include "../drivers/vga.h"
#include "../drivers/idt.h"
#include "../drivers/PS2keyboard.h"
#include "kernelUtils.h"
#include "system/security.h"
#include "system/processes.h"

//Globals
flagQueue processFlags;

int main() {
		clearScreen();
		printString("---ITERGEN SYSTEMS LTD---\n\nIterOS\n\nThis operating system was developed by Itergen Systems as a proto-kernel intended to run programs developed by Itergen Systems, in the most efficient way possible.\n");
		printString("Installing interrupt service routines (ISRs)\n");
		isrInstall();
		printString("Enabling external interrupts\n");
		asm volatile("sti");
		printString("Initializing PS/2 keyboard driver\n");
		initPS2keyboard();
		printString("Initializing Process Manager\n");
		processFlags.flagCount = (uint32_t)0;
		printString("Initializing Login Daemon\n");
		int loginStatus;
		do {
			loginStatus = initLoginDaemon();
		} while (loginStatus != 0);
		char dataBuf[DATA_BUF_SIZE];
		flag sys = {
			.processID = 21,
			.type = SYSTEM_FLAG,
			.priority = 1,
			.data = dataBuf
		};
		queueFlag(sys); //System Flag For System Calls
		printString("ITERGEN SYSTEMS LTD\n");
		printString(">> \0");
		initTerminal();
		return 0;
}
