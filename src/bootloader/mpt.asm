; Master Partition Table
; First Primary Partition
db 0x80 ; Active Partition
db 0x00, 0x00, 0x00 ; CHS Start (Head, Sector, Cylinder)
db 0x0B ; Partition Type - FAT32
db 0x00, 0x00, 0x00 ; CHS End (Read Up On This, Its Complicated)
db 0x3F, 0x00, 0x00, 0x00 ; LBA Starting Sector
db 0xB0, 0x4F, 0x00, 0x00 ; Partition Size
; Second Primary Partition
TIMES 16 db 0
; Third Primary Partition
TIMES 16 db 0
; Fourth Primary Partition
TIMES 16 db 0