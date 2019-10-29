dd if=/dev/zero of=diskc.img bs=512 count=1000
nasm bootload.asm
dd if=bootload of=diskc.img bs=512 bs=512 count=1 conv=notrunc


bcc -ansi -c -o kernel_c.o kernel.c
as86 kernel.asm -o kernel_asm.o
ld86 -o kernel -d kernel_c.o kernel_asm.o

as86 userlib.asm -o userlib.o



gcc -o loadFile loadfile.c
./loadFile kernel
./loadFile message.txt

