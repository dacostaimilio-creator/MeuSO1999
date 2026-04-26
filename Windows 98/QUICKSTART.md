# Quick Start - MeuSO 1999

## Início Rápido

### 1. Instalar Dependências

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential nasm xorriso grub-pc-bin qemu-system-i386

# Instalar cross-compiler (escolha uma opção):

# Opção A: Script automático (recomendado)
chmod +x scripts/install_toolchain.sh
./scripts/install_toolchain.sh
export PATH="$HOME/opt/cross/bin:$PATH"

# Opção B: Pacote (se disponível)
sudo apt-get install gcc-i686-elf binutils-i686-elf
```

### 2. Compilar

```bash
# Criar estrutura
make setup

# Compilar kernel
make

# Testar no QEMU
make qemu
```

### 3. Criar ISO

```bash
# Criar ISO bootável
make iso

# Testar ISO
make qemu-iso
```

### 4. VirtualBox

1. Criar nova VM:
   - Tipo: Other / Other/Unknown (32-bit)
   - Memória: 512MB
   - Disco: 8GB VDI dinâmico

2. Configurar:
   - Sistema → Placa-mãe: PIIX3, desmarque EFI
   - Display: 16-64MB VRAM
   - Armazenamento: Adicione `meuSO.iso` como CD
   - Rede: PCnet-FAST III (NAT)

3. Iniciar VM

## Estrutura Criada

```
MeuSO 1999/
├── boot/                    # Bootloader
│   ├── entry.S              # Ponto de entrada multiboot
│   ├── gdt_flush.S          # Flush GDT
│   ├── idt_load.S           # Load IDT
│   ├── isr_stubs.S          # Stubs de interrupções
│   └── task_switch.S        # Troca de contexto
├── kernel/                   # Kernel
│   ├── main.c               # Função principal
│   ├── gdt.c                # GDT
│   ├── idt.c                # IDT
│   ├── isr.c                # ISR handlers
│   ├── irq.c                # IRQ handlers
│   ├── paging.c             # Paginação
│   ├── timer.c              # Timer PIT
│   ├── keyboard.c           # Driver teclado
│   ├── vga.c                # Driver VGA
│   ├── multitask.c          # Multitarefa
│   └── utils.c              # Utilitários
├── include/                  # Headers
├── userland/                 # Userland
│   └── cursor_ia/           # Cursor IA
│       ├── cursor_engine.c  # Engine de desenho
│       ├── cursor_agent.c   # Agente IA
│       └── cursor_ia.h      # API
├── scripts/                  # Scripts auxiliares
├── linker.ld                 # Script de linkagem
├── Makefile                  # Build system
└── README.md                 # Documentação
```

## Funcionalidades Implementadas

✅ **Kernel Básico**
- Bootloader multiboot
- GDT/IDT configurados
- Paginação básica
- Timer PIT (100Hz)
- Driver de teclado PS/2
- Driver VGA (modo texto)
- Multitarefa básica
- Sistema de interrupções

✅ **Estrutura Base**
- Makefile completo
- Scripts de build
- Documentação
- Componente Cursor IA (estrutura)

## Próximos Passos

### Curto Prazo

1. **Mouse Driver**
   - Completar driver PS/2
   - Eventos de mouse

2. **Framebuffer**
   - Modo gráfico VESA
   - Desenho básico

3. **Window Manager**
   - Sistema de janelas
   - Eventos UI

### Médio Prazo

1. **Sistema de Arquivos**
   - FAT32 básico
   - Operações de I/O

2. **Rede**
   - Driver PCnet
   - Stack TCP/IP (lwIP)

3. **Aplicações**
   - Shell completo
   - File manager
   - Editor de texto

### Longo Prazo

1. **Cursor IA Completo**
   - Integração ML
   - Sugestões contextuais

2. **Browser**
   - Parser HTML básico
   - Renderização

3. **Sistema de Pacotes**
   - Instalador .tar.gz
   - Gerenciador de apps

## Troubleshooting

### Kernel não compila

```bash
# Verificar toolchain
i686-elf-gcc --version

# Se não encontrado, instalar:
./scripts/install_toolchain.sh
export PATH="$HOME/opt/cross/bin:$PATH"
```

### QEMU não inicia

```bash
# Verificar se QEMU está instalado
qemu-system-i386 --version

# Instalar se necessário
sudo apt-get install qemu-system-i386
```

### ISO não boota

1. Verificar se `kernel.bin` foi criado
2. Verificar `grub.cfg` em `isodir/boot/grub/`
3. Testar com QEMU primeiro

## Recursos

- [OSDev Wiki](https://wiki.osdev.org/)
- [JamesM's Tutorial](https://web.archive.org/web/20160412174753/http://www.jamesmolloy.co.uk/tutorial_html/)
- [lwIP](https://savannah.nongnu.org/projects/lwip/)

## Licença

Projeto educacional de código aberto.
