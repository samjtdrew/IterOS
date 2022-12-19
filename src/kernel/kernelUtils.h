#pragma once

#include <stddef.h>

//Functions from kernelops.asm
extern void jumpToRealMode_ASM(void);
extern void setRM_ASM(void);


//Funcs
void systemReboot() {
	jumpToRealMode_ASM();
}

int stringLength(char s[]) {
		int i = 0;
		while (s[i] != '\0') ++i;
		return i;
}

void append(char s[], char n) {
		int len = stringLength(s);
		s[len] = n;
		s[len + 1] = '\0';
}

void reverse(char s[]) {
		int c, i, j;
		for (i = 0, j = stringLength(s)-1; i < j; i++, j--) {
				c = s[i];
				s[i] = s[j];
				s[j] = c;
		}
}

void intToString(int n, char str[]) {
		int i, sign;
		if ((sign = n) < 0) n = -n;
		i = 0;
		do {
				str[i++] = n % 10 + '0';
		} while ((n /= 10) > 0);
		if (sign < 0) str[i++] = '-';
		str[i] = '\0';
		reverse(str);
}

int compareString(char s1[], char s2[]) {
		int i;
		for (i = 0; s1[i] == s2[i]; i++) {
				if (s1[i] == '\0') return 0;
		}
		return s1[i] - s2[i];
}

char* itoa(int val, int base) {
	if (val == 0) {
		return "0";
	} else {
		static char buf[32] = {0};
		int i = 30;
		for(; val && i ; --i, val /= base) {
			buf[i] = "0123456789abcdef"[val % base];
		}
		return &buf[i+1];
	}
}

int stringCpy(char* dst, char* src, int dstSize) {
	for (size_t i = 0; i < dstSize; i++) {
		dst[i] = src[i];
	}
	return 0;
}