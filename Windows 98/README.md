# MeuSO 1999 - Sistema Operacional Estilo Windows 98

Sistema operacional completo estilo Windows 98/1999 com kernel monolítico, GUI, aplicações e componente de Cursor IA.

## Características

- **Kernel monolítico** em C + assembly (i386 32-bit)
- **Gerenciamento de memória** com paging
- **Multitarefa** preemptiva
- **Drivers essenciais**: teclado, mouse, VGA
- **GUI estilo 1999** com window manager
- **Aplicações builtin**: shell, file manager, editor, browser
- **Cursor IA**: agente inteligente integrado

## Requisitos

### Toolchain

```bash
# Ubuntu/Debian
sudo apt-get install build-essential nasm xorriso grub-pc-bin qemu-system-i386

# Instalar cross-compiler i686-elf
# Opção 1: Usar pacote (se disponível)
sudo apt-get install gcc-i686-elf binutils-i686-elf

# Opção 2: Compilar do zero (recomendado)
# Siga instruções em: https://wiki.osdev.org/GCC_Cross-Compiler
```

## Compilação

```bash
# Criar estrutura de diretórios
make setup

# Compilar kernel
make

# Criar ISO
make iso

# Testar com QEMU
make qemu
# ou
make qemu-iso
```

## Estrutura do Projeto

```
/
├── boot/           # Bootloader e código assembly
├── kernel/         # Código do kernel
├── include/        # Headers
├── userland/       # Aplicações userland
├── fsroot/         # Sistema de arquivos raiz
├── iso/            # Arquivos para ISO
├── linker.ld       # Script de linkagem
├── Makefile        # Build system
└── README.md       # Este arquivo
```

## Arquitetura

### Boot
- GRUB Multiboot → `entry.S` → `kernel_main()`

### Kernel
- **GDT/IDT**: Gerenciamento de segmentos e interrupções
- **Paging**: Mapeamento de memória virtual
- **Timer**: PIT para preempção
- **Drivers**: Teclado PS/2, Mouse PS/2, VGA
- **Multitarefa**: Scheduler round-robin

### Userland
- **init**: Processo PID 1
- **Shell**: Interface de linha de comando
- **GUI**: Window manager estilo 1999
- **Apps**: File manager, editor, browser

### Cursor IA
- **Cursor Engine**: Desenho e animação do cursor
- **Cursor Agent**: Daemon que observa contexto UI
- **Model Layer**: Inferência local ou remota

## Próximos Passos

1. ✅ Estrutura básica do kernel
2. ✅ Drivers base implementados (mouse PS/2, disco ATA PIO)
3. ✅ Detecção inicial de sistema de arquivos FAT32
4. ✅ Interface boot estilo 1999 (desktop em VGA texto)
5. ⏳ Aplicações userland
6. ⏳ Componente Cursor IA
7. ⏳ Pilha de rede TCP/IP (lwIP)
8. ⏳ Browser HTML básico

## Licença

Este projeto é educacional e de código aberto.

## Referências

- [OSDev Wiki](https://wiki.osdev.org/)
- [JamesM's Kernel Tutorial](https://web.archive.org/web/20160412174753/http://www.jamesmolloy.co.uk/tutorial_html/)
- [lwIP](https://savannah.nongnu.org/projects/lwip/)
