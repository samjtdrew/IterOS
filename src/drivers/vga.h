#pragma once

#include "IOports.h"

#define VGA_BLACK 0
#define	VGA_BLUE 1
#define VGA_GREEN 2
#define VGA_CYAN 3
#define VGA_RED 4
#define VGA_MAGENTA 5
#define VGA_BROWN 6
#define VGA_LIGHT_GREY 7
#define VGA_DARK_GREY 8
#define VGA_LIGHT_BLUE 9
#define VGA_LIGHT_GREEN 10
#define VGA_LIGHT_CYAN 11
#define VGA_LIGHT_RED 12
#define VGA_LIGHT_MAGENTA 13
#define VGA_LIGHT_BROWN 14
#define VGA_WHITE 15

#define VGA_CTRL_REGISTER 0x3d4
#define VGA_DATA_REGISTER 0x3d5
#define VGA_OFFSET_LOW 0x0f
#define VGA_OFFSET_HIGH 0x0e

#define VIDEO_ADDRESS 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define WHITE_ON_BLACK 0x0f

void setCursor(int offset) {
	offset /= 2;
	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	portByteOut(VGA_DATA_REGISTER, (unsigned char) (offset >> 8));
	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	portByteOut(VGA_DATA_REGISTER, (unsigned char) (offset & 0xff));
}

int getCursor() {
	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_HIGH);
	int offset = portByteIn(VGA_DATA_REGISTER) << 8;
	portByteOut(VGA_CTRL_REGISTER, VGA_OFFSET_LOW);
	offset += portByteIn(VGA_DATA_REGISTER);
	return offset * 2;
}

void setCharAtVideoMemory(char character, int offset) {
	unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;
	vidmem[offset] = character;
	vidmem[offset + 1] = WHITE_ON_BLACK;
}

int getRowFromOffset(int offset) {
	return offset / (2 * MAX_COLS);
}

int getOffset(int col, int row) {
	return 2 * (row * MAX_COLS + col);
}

int moveOffsetToNewLine(int offset) {
	return getOffset(0, getRowFromOffset(offset) + 1);
}

void vidMemCpy(char *source, char *dest, int nbytes) {
	int i;
	for (i = 0; i < nbytes; i++) {
		*(dest + i) = *(source + i);
	}
}

int scrollVgaScreen(int offset) {
	vidMemCpy((char *) (getOffset(0, 1) + VIDEO_ADDRESS), (char *) (getOffset(0, 0) + VIDEO_ADDRESS), MAX_COLS * (MAX_ROWS - 1) * 2);
	for (int col = 0; col < MAX_COLS; col++) {
		setCharAtVideoMemory(' ', getOffset(col, MAX_ROWS - 1));
	}
	return offset - 2 * MAX_COLS;
}

void printString(char *String) {
	int offset = getCursor();
	int i = 0;
	while (String[i] != '\0') {
		if (offset >= MAX_ROWS * MAX_COLS * 2) {
			offset = scrollVgaScreen(offset);
		}
		if (String[i] == '\n') {
			offset = moveOffsetToNewLine(offset);
		} else {
			setCharAtVideoMemory(String[i], offset);
			offset += 2;
		}
		i++;
	}
	setCursor(offset);
}

void clearScreen() {
	for (int i = 0; i < MAX_COLS * MAX_ROWS; i++) {
		setCharAtVideoMemory(' ', i * 2);
	}
	setCursor(getOffset(0, 0));
}