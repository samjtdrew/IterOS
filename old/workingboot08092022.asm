[BITS 16]
[ORG 0x7C00]


call clear_screen
call start


head: db "ITERGEN SYSTEMS LTD - IterOS", 13, 10, 0
outpass: db "Please Enter Boot Authorisation Code\n>>", 0
inpass: db 0, 0, 0, 0, 0, 0, 0, 0
epass: db 13, 10, "Authorisation Code Denied, Try Again", 13, 10, 0 
bootcode: db "password"
rpass: db 13, 10, "Authorisation Code Accepted, Booting IterOS", 13, 10, 0


start:
mov si, head
call print
takepass:
mov si, outpass
call print
mov bx, inpass
call scan
; check password, if correct, go to right pass
right_pass:
mov si, rpass
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


scan:
mov cx, 0
scanl:
cmp cx, 9
je dead_scan
mov ah, 0
int 0x16
cmp al, 13
je scan_exit
mov [bx], al
inc bx
inc cx
call astrisk
jmp scanl
scan_exit:
call check_pass
dead_scan:
mov ah, 0
int 0x16
cmp al, 13
je wrong_pass
call astrisk
jmp dead_scan


wrong_pass:
mov si, epass
call print
jmp takepass


astrisk:
mov dx, bx
mov al, '*'
mov ah, 0x0E
mov bh, 0x00
mov bl, 0x07
int 0x10
mov bx, dx
ret


check_pass:
mov cx, 0
mov si, bootcode
mov di, inpass
check:
cmp cx, 8
je right_pass
mov al, [si]
mov dl, [di]
inc cx
inc si
inc di
cmp al, dl
je check
jmp wrong_pass


clear_screen:
mov ah, 0
int 0x10
ret


TIMES 510 - ($ - $$) db 0
DW 0xAA55
