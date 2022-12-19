; This assembly program is located at 0x1000 and is called by the bootloader
; It ensures that the kernel is entered at the 'Main()' function
[BITS 32]
[extern main]
; First probe the A20 Line to check if enabled
checkA20:
pushad
mov edi, 0x112345
mov esi, 0x012345
mov [esi], esi
mov [edi], edi
cmpsd
popad
jne A20_on	; A20 Line is set
jmp fast_enable_A20		; A20 Line is cleared
jmp checkA20	; Check again that the A20 Line is enabled


fast_enable_A20:
in al, 0x92
or al, 2
out 0x92, al
ret


; Start kernel
A20_on:
call main
jmp $
