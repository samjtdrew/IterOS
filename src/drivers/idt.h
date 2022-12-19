#pragma once

#include <stdint.h>
#include "vga.h"
#include "../kernel/kernelUtils.h"
#include "IOports.h"

#define IDT_ENTRIES 256

typedef struct {
		uint16_t lowOffset;
		uint16_t selector;
		uint8_t always0;
		uint8_t flags;
		uint16_t highOffset;
} __attribute__((packed)) idtGate_t;

typedef struct {
		uint16_t limit;
		uint16_t base;
} __attribute__((packed)) idtRegister_t;

typedef struct {
		uint32_t ds;
		uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
		uint32_t int_no, err_code;
		uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (*isr_t)(registers_t *);

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

char *exceptionMessages[] = {
		"Division By Zero",
		"Debug",
		"Non Maskable Interrupt",
		"Breakpoint",
		"Into Detected Overflow",
		"Out Of Bounds",
		"Invalid Opcode",
		"No Coprocessor",

		"Double Fault",
		"Coprocessor Segment Overrun",
		"Bad TSS",
		"Segment Not Present",
		"Stack Fault",
		"General Protection Fault",
		"Page Fault",
		"Unknown Interrupt",

		"Coprocessor Fault",
		"Alignment Check",
		"Machine Check",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",

		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved",
		"Reserved"
};

#define low16(address) (uint16_t)((address) & 0xFFFF)
#define high16(address) (uint16_t)(((address) >> 16) & 0xFFFF)

idtGate_t idt[IDT_ENTRIES];
isr_t interruptHandlers[256];
idtRegister_t idtReg;

void setIdtGate(int n, uint32_t handler) {
		idt[n].lowOffset = low16(handler);
		idt[n].selector = 0x08;
		idt[n].always0 = 0;
		idt[n].flags = 0x8E;
		idt[n].highOffset = high16(handler);
}

void isrHandler(registers_t *r) {
		printString("Recieved Interrupt: ");
		char s[3];
		intToString(r->int_no, s);
		printString(s);
		printString("\n");
		printString(exceptionMessages[r->int_no]);
		printString("\n");
		asm volatile("hlt");
}

void registerInterruptHandler(uint8_t n, isr_t handler) {
		interruptHandlers[n] = handler;
}

void irqHandler(registers_t *r) {
		if (interruptHandlers[r->int_no] != 0) {
				isr_t handler = interruptHandlers[r->int_no];
				handler(r);
		}
		if (r->int_no >= 40) {
				portByteOut(0xA0, 0x20);
		}
		portByteOut(0x20, 0x20);
}

void loadIdt() {
		idtReg.base = (uint32_t) &idt;
		idtReg.limit = IDT_ENTRIES * sizeof(idtGate_t) - 1;
		asm volatile("lidt (%0)" : : "r" (&idtReg));
}

void isrInstall() {
		setIdtGate(0, (uint32_t)isr0);
		setIdtGate(1, (uint32_t)isr1);
		setIdtGate(2, (uint32_t)isr2);
		setIdtGate(3, (uint32_t)isr3);
		setIdtGate(4, (uint32_t)isr4);
		setIdtGate(5, (uint32_t)isr5);
		setIdtGate(6, (uint32_t)isr6);
		setIdtGate(7, (uint32_t)isr7);
		setIdtGate(8, (uint32_t)isr8);
		setIdtGate(9, (uint32_t)isr9);
		setIdtGate(10, (uint32_t)isr10);
		setIdtGate(11, (uint32_t)isr11);
		setIdtGate(12, (uint32_t)isr12);
		setIdtGate(13, (uint32_t)isr13);
		setIdtGate(14, (uint32_t)isr14);
		setIdtGate(15, (uint32_t)isr15);
		setIdtGate(16, (uint32_t)isr16);
		setIdtGate(17, (uint32_t)isr17);
		setIdtGate(18, (uint32_t)isr18);
		setIdtGate(19, (uint32_t)isr19);
		setIdtGate(20, (uint32_t)isr20);
		setIdtGate(21, (uint32_t)isr21);
		setIdtGate(22, (uint32_t)isr22);
		setIdtGate(23, (uint32_t)isr23);
		setIdtGate(24, (uint32_t)isr24);
		setIdtGate(25, (uint32_t)isr25);
		setIdtGate(26, (uint32_t)isr26);
		setIdtGate(27, (uint32_t)isr27);
		setIdtGate(28, (uint32_t)isr28);
		setIdtGate(29, (uint32_t)isr29);
		setIdtGate(30, (uint32_t)isr30);
		setIdtGate(31, (uint32_t)isr31);
		
		portByteOut(0x20, 0x11);
		portByteOut(0xA0, 0x11);
		portByteOut(0x21, 0x20);
		portByteOut(0xA1, 0x28);
		portByteOut(0x21, 0x04); 
		portByteOut(0xA1, 0x02);
		portByteOut(0x21, 0x01);
		portByteOut(0xA1, 0x01);
		portByteOut(0x21, 0x0);
		portByteOut(0xA1, 0x0);

		setIdtGate(32, (uint32_t)irq0);
		setIdtGate(33, (uint32_t)irq1);
		setIdtGate(34, (uint32_t)irq2);
		setIdtGate(35, (uint32_t)irq3);
		setIdtGate(36, (uint32_t)irq4);
		setIdtGate(37, (uint32_t)irq5);
		setIdtGate(38, (uint32_t)irq6);
		setIdtGate(39, (uint32_t)irq7);
		setIdtGate(40, (uint32_t)irq8);
		setIdtGate(41, (uint32_t)irq9);
		setIdtGate(42, (uint32_t)irq10);
		setIdtGate(43, (uint32_t)irq11);
		setIdtGate(44, (uint32_t)irq12);
		setIdtGate(45, (uint32_t)irq13);
		setIdtGate(46, (uint32_t)irq14);
		setIdtGate(47, (uint32_t)irq15);

		loadIdt();
}
