global jumpToRealMode_ASM
global setRM_ASM

[bits 16] ; Sets 16 bit PM

IDT_RM: ; Real mode IDT
    dw 0x3FF ; 256 Entries, 4bytes each = 1Kb
    dd 0 ; Real mode IVT @memaddr 0x0000

cr0state: dd 0

jumpToRealMode_ASM:
    ; Move from protected mode (16 bit) to real mode (16 bit)
    cli ; Clear interupts
    mov eax, 0 ; 16-Bit PM GDT entries
    mov ds, eax
    mov es, eax
    mov fs, eax
    mov gs, eax
    mov ss, eax
    mov eax, cr0 ; Disabling paging
    mov [cr0state], eax ; Save cr0 state
    and eax, 0x7FFFFFFE ; Disable paging & disable 16-bit PM
    mov cr0, eax
    call 0:setRM_ASM
    ret

setRM_ASM:
    mov sp, 0x8000 ; Moving the stack pointer
    mov ax, 0 ; Resetting the segment registers to 0
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    lidt [IDT_RM]
    sti
    ret