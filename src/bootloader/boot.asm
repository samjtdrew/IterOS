[BITS 16]
[ORG 0x7C00]

jmp short start
nop

; BIOS Parameter Block
%include "bpb.asm"

start:
    xor ax, ax ; Setting offset for segment registers
    mov ds, ax
    mov es, ax
    mov [BOOT_DRIVE], dl ; Save bios drive number
    ; Primary prerequisites are now set
    mov bp, 0x7C00 ; Setting the stack
    mov sp, bp
    ; Load the kernel from memory
    mov bx, 0x1000
    mov dh, 15 ; Sectors to load
    mov dl, [BOOT_DRIVE] ; Drive to load from
    call dskload ; Disk Load function
    call switch_to_pm	; Set CPU to 32-bit Protected Mode


; Functions
pressKeyToReboot:
; ADDR: 0x00000078  DATA: b4 00 cd
    mov ah, 0
    int 16h ; Waits for a key to be pressed
    jmp 0FFFFh:0 ; Jumps to the start of BIOS
    ret

%include "disk_load.asm"
%include "gdt.asm"
%include "switch_to_pm.asm"


[BITS 32]
BEGIN_PM:	; Protected mode is now activated and initialised
call 0x1000 ; Running The Kernel

jmp $ ; Hanging if the Kernel Returns

BOOT_DRIVE: db 0

TIMES 446-($-$$) db 0 ; With MPT
; TIMES 510-($-$$) db 0 ; Without MPT
; Master Partition Table
%include "mpt.asm"

DW 0xAA55
