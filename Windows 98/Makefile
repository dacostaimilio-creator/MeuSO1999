# Makefile para MeuSO 1999
# Cross-compiler para i386/i686

ARCH = i686-elf
CC = $(ARCH)-gcc
AS = $(CC)
ASFLAGS = -c -m32
LD = $(ARCH)-ld
OBJCOPY = $(ARCH)-objcopy

# Diretórios
BOOT_DIR = boot
KERNEL_DIR = kernel
INCLUDE_DIR = include
ISO_DIR = isodir
FSROOT_DIR = fsroot

# Flags
CFLAGS = -ffreestanding -O2 -Wall -Wextra -m32 \
         -fno-builtin -fno-stack-protector -I$(INCLUDE_DIR)
ASFLAGS = -f elf32
LDFLAGS = -T linker.ld -nostdlib -m elf_i386

# Arquivos fonte
KERNEL_SOURCES = $(wildcard $(KERNEL_DIR)/*.c)
BOOT_SOURCES = $(wildcard $(BOOT_DIR)/*.S)

# Arquivos objeto
KERNEL_OBJECTS = $(KERNEL_SOURCES:.c=.o)
BOOT_OBJECTS = $(BOOT_SOURCES:.S=.o)

# Alvo principal
all: kernel.elf

# Kernel binário
kernel.elf: $(BOOT_OBJECTS) $(KERNEL_OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -o kernel.elf $(BOOT_OBJECTS) $(KERNEL_OBJECTS)

# Compilar arquivos C
$(KERNEL_DIR)/%.o: $(KERNEL_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar arquivos assembly
$(BOOT_DIR)/%.o: $(BOOT_DIR)/%.S
	$(AS) $(CFLAGS) -c $< -o $@
# Limpar
clean:
	rm -f $(BOOT_OBJECTS) $(KERNEL_OBJECTS) kernel.elf

# Criar estrutura de diretórios
setup:
	mkdir -p $(ISO_DIR)/boot/grub
	mkdir -p $(FSROOT_DIR)/{bin,sbin,etc,dev,usr,home,proc,sys,tmp}
	mkdir -p $(FSROOT_DIR)/usr/{bin,lib,share}

# Criar ISO
iso: kernel.elf setup
	cp kernel.elf $(ISO_DIR)/boot/kernel.elf
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o meuSO.iso $(ISO_DIR) 2>/dev/null || \
	grub2-mkrescue -o meuSO.iso $(ISO_DIR) 2>/dev/null || \
	xorriso -as mkisofs -R -b boot/grub/i386-pc/eltorito.img -no-emul-boot -boot-load-size 4 -boot-info-table -o meuSO.iso $(ISO_DIR)

# Testar com QEMU
qemu: kernel.bin
qemu: kernel.elf
	qemu-system-i386 -kernel kernel.elf -m 512M

# Testar ISO com QEMU
qemu-iso: iso
	qemu-system-i386 -cdrom meuSO.iso -m 512M

.PHONY: all clean setup iso qemu qemu-iso
