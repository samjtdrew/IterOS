# Variables and directories
bootdir=~/OSdev/IterOS/src/bootloader
kerneldir=~/OSdev/IterOS/src/kernel
driverdir=~/OSdev/IterOS/src/drivers
compiledir=~/OSdev/IterOS/release
windir=/mnt/d/IterOS/operating-system

# Copy files from windows working directory...
# bash $compiledir/lincp.sh

cd $bootdir
nasm boot.asm -f bin -o bootloader.bin
nasm blankdsk.asm -f bin -o blankdsk.bin
cd $driverdir
nasm interrupt.asm -f elf -o $kerneldir/interrupt.o
cd $kerneldir
i686-elf-gcc -ffreestanding -c kernel.c -o kernel.o
nasm kernel_entry.asm -f elf -o kernel_entry.o
nasm kernelops.asm -f elf -o kernelops.o
i686-elf-ld -o kernel.bin -Ttext 0x1000 kernel_entry.o interrupt.o kernelops.o kernel.o --oformat binary # Either 0x1000 or 0x7e00
cd $compiledir
cat $bootdir/bootloader.bin $kerneldir/kernel.bin $bootdir/blankdsk.bin > iteros.iso


# Push Project files to GitHub


# Cleanup
# rm $compiledir/iteros.iso
rm $kerneldir/kernel.bin
rm $kerneldir/kernel.o
rm $kerneldir/kernelops.o
rm $bootdir/bootloader.bin
rm $bootdir/blankdsk.bin
rm $kerneldir/kernel_entry.o
rm $kerneldir/interrupt.o
