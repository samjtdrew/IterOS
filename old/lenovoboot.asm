[BITS 16]
[ORG 0x7C00]

call start

head: db "ITERGEN SYSTEMS LTD - IterOS", 13, 10, 0


start:
mov si, head
call print
jmp $


print:
mov al, [si]
mov ah, 0x0E
mov bh, 0X00
mov bl, 0x07
cmp al, 0
je print_exit
int 0x10
inc si
jmp print
print_exit:
ret

TIMES 510 - ($ - $$) db 0
DW 0xAA55