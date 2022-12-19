; Global Descriptor Table [GDT] For Making The Transition Between 16 Bits And 32 Bits
gdt_start:

gdt_null: ; This null descriptor is mandatory
dd 0x0	; dd means define double word (4 bytes)
dd 0x0

gdt_code: ; CODE SEGMENT DESCRIPTOR
dw 0xffff	; Limit (bits 0 - 15)
dw 0x0		; Base (bits 0 - 15)
db 0x0		; Base (bits 16 - 23)
db 10011010b
db 11001111b
db 0x0		; Base (bits 24 - 31

gdt_data: ; DATA SEGMENT DESCRIPTOR
dw 0xffff	; Limit (bits 0 - 15)
dw 0x0		; Base (bits 0 - 15)
db 0x0		; Base (bits 16 - 23)
db 10010010b
db 11001111b
db 0x0		; Base (bits 24 - 31)

gdt_end: ; This label allows the assembler to calculate the size of the GDT



gdt_descriptor:
dw gdt_end - gdt_start - 1		; Size of the GDT
dd gdt_start					; Start address of the GDT



CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start
