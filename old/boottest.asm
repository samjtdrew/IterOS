[BITS 16]
[ORG 0x7C00]


JMP short Test


String: db 'Yall Are Nerds :)', 0


Test:
MOV SI, String
CALL print


print:
MOV AL, [SI]
CMP AL, 0
JZ Hang
MOV AH, 0x0E
INT 0x10
INC SI
LOOP print


Hang:
JMP $


TIMES 510 - ($ - $$) db 0
DW 0xAA55
