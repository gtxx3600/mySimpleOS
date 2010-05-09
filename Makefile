ASMBFLAGS	= -I boot/include/  -I include/
CFLAGS		= -I include/ -c -fno-builtin -fno-stack-protector -m32
ASMKFLAGS   = -I include/
DEPEN		=   include/type.h include/const.h include/protect.h include/string.h include/proc.h include/tty.h \
				include/console.h include/global.h include/keyboard.h include/proto.h include/proc.h include/fs.h
OBJS		=   kernel/kernel.o kernel/global.o lib/kliba.o lib/string.o kernel/start.o kernel/i8259A.o \
				kernel/protect.o kernel/keyboard.o lib/klib.o kernel/main.o kernel/clock.o kernel/syscall.o \
				kernel/proc.o kernel/tty.o kernel/console.o kernel/printf.o kernel/vsprintf.o kernel/systask.o \
				lib/misc.o kernel/hd.o fs/main.o
boot/LOADER.BIN:boot/LOADER.asm
	nasm $(ASMBFLAGS) boot/LOADER.asm -o boot/LOADER.BIN
boot/boot.img: boot/boot.asm
	nasm $(ASMBFLAGS) boot/boot.asm -o boot/boot.bin
	@dd if=boot/boot.bin of=boot/boot.img bs=512 count=1
	@dd if=/dev/zero of=boot/boot.img skip=1 seek=1 bs=512 count=2879
lib/kliba.o:lib/kliba.asm include/sconst.inc
	nasm -f elf -o lib/kliba.o lib/kliba.asm $(ASMBFLAGS)
lib/string.o:lib/string.asm
	nasm -f elf -o lib/string.o lib/string.asm $(ASMBFLAGS)
kernel/main.o:kernel/main.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)
kernel/keyboard.o:kernel/keyboard.c $(DEPEN) include/keymap.h
	gcc -c -o $@ $< $(CFLAGS)
kernel/start.o:kernel/start.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)
kernel/global.o:kernel/global.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)
kernel/i8259A.o:kernel/i8259A.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)
kernel/protect.o:kernel/protect.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)
kernel/clock.o:kernel/clock.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)
kernel/proc.o:kernel/proc.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS) 
kernel/tty.o:kernel/tty.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS) 
kernel/console.o:kernel/console.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS) 
kernel/printf.o:kernel/printf.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS) 
kernel/vsprintf.o:kernel/vsprintf.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS) 
kernel/syscall.o:kernel/syscall.asm
	nasm -f elf -o  $@ $<  $(ASMBFLAGS)
kernel/systask.o:kernel/systask.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)
kernel/hd.o:kernel/hd.c $(DEPEN) include/hd.h
	gcc -c -o $@ $< $(CFLAGS)
fs/main.o:fs/main.c $(DEPEN) include/hd.h include/fs.h
	gcc -c -o $@ $< $(CFLAGS)
lib/klib.o:lib/klib.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)
lib/misc.o:lib/misc.c $(DEPEN)
	gcc -c -o $@ $< $(CFLAGS)	
kernel/kernel.o:kernel/kernel.asm include/sconst.inc
	nasm -f elf -o kernel/kernel.o kernel/kernel.asm $(ASMKFLAGS)
kernel/KERNEL.BIN:$(OBJS)
	ld  -m elf_i386 -Ttext 0x30400 -o kernel/KERNEL.BIN $(OBJS)
copy: boot/boot.img boot/LOADER.BIN kernel/KERNEL.BIN
	sudo mount -o loop boot/boot.img /mnt/floppy/ -o fat=12;\
	sudo cp boot/LOADER.BIN /mnt/floppy/;\
	sudo cp kernel/KERNEL.BIN /mnt/floppy/;\
	sudo umount /mnt/floppy/;\
	
clean:
	rm -f boot/*.BIN
	rm -f boot/*.bin
	rm -f boot/*.img
	rm -f kernel/*.o
	rm -f kernel/*.BIN
	rm -f lib/*.o
	rm -f fs/*.o
	
	
