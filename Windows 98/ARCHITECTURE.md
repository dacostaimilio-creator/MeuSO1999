# Arquitetura do MeuSO 1999

## Visão Geral

MeuSO 1999 é um sistema operacional monolítico estilo Windows 98, desenvolvido em C e assembly (i386 32-bit).

## Camadas do Sistema

### 1. Boot Layer

- **GRUB Multiboot**: Bootloader padrão
- **entry.S**: Ponto de entrada em assembly
- **Inicialização**: Setup de segmentos, stack, chamada para kernel_main()

### 2. Kernel Core

#### 2.1 Gerenciamento de Memória

- **GDT (Global Descriptor Table)**: Segmentação flat (32-bit)
- **Paging**: Mapeamento de memória virtual (4KB pages)
- **Alocador físico**: Bitmap de páginas
- **Heap do kernel**: Bump allocator (futuro: free-lists)

#### 2.2 Interrupções

- **IDT (Interrupt Descriptor Table)**: 256 entradas
- **ISR (Interrupt Service Routines)**: Handlers para exceções (0-31)
- **IRQ (Interrupt Requests)**: Handlers para hardware (32-47)
- **PIC (Programmable Interrupt Controller)**: Remapeado para IRQ 32-47

#### 2.3 Drivers

- **VGA**: Modo texto 80x25, suporte a framebuffer (futuro)
- **Teclado PS/2**: IRQ1, scancode para ASCII
- **Mouse PS/2**: IRQ12 (a implementar)
- **Timer PIT**: IRQ0, 100Hz para preempção
- **Disco ATA**: (a implementar)
- **Rede PCnet**: (a implementar)

#### 2.4 Multitarefa

- **Scheduler**: Round-robin preemptivo
- **TCB (Task Control Block)**: Estrutura de tarefa
- **Context Switch**: Troca de contexto em assembly
- **Estados**: RUNNING, READY, BLOCKED, TERMINATED

#### 2.5 Syscalls

- **Int 0x80**: Interface de syscalls (a implementar)
- **ABI**: read, write, open, close, fork, exec, exit

### 3. Userland

#### 3.1 Init

- **PID 1**: Primeiro processo
- **Montagem de FS**: Monta sistema de arquivos
- **Inicialização de serviços**: networkd, uiserver

#### 3.2 Aplicações

- **Shell**: Interface de linha de comando
- **File Manager**: Gerenciador de arquivos estilo Explorer
- **Editor**: Editor de texto básico
- **Browser**: Navegador HTML minimalista

### 4. GUI (Graphical User Interface)

#### 4.1 Window Manager

- **Compositor**: Gerencia janelas e desenho
- **Eventos**: Sistema de eventos (mouse, teclado)
- **Decoração**: Bordas, botões (min/max/close)
- **Taskbar**: Barra de tarefas estilo 1999

#### 4.2 Renderização

- **Framebuffer**: Modo gráfico VESA ou VGA
- **Fontes**: Bitmap fonts (8x16, 12x20)
- **Paleta**: 256 cores (estilo 1999)

### 5. Cursor IA

#### 5.1 Cursor Engine

- **Desenho**: Sprite do cursor (16x16)
- **Animações**: Cursor animado
- **Posicionamento**: Sincronização com mouse

#### 5.2 Cursor Agent

- **Contexto UI**: Observa janela ativa, texto selecionado
- **Análise**: Heurísticas + ML (futuro)
- **Sugestões**: Ações contextuais

#### 5.3 Model Layer

- **Local**: Modelo pequeno quantizado (opcional)
- **Remoto**: API externa (OpenAI, etc.)
- **Protocolo**: JSON via socket local

### 6. Sistema de Arquivos

- **FAT32**: Formato inicial (simples)
- **ext2-lite**: Alternativa (futuro)
- **initrd**: Ramdisk para boot

### 7. Rede

- **Driver**: PCnet-FAST III (am79c973)
- **Stack**: lwIP integrado
- **Protocolos**: ARP, IP, ICMP, TCP (subset)

## Fluxo de Boot

```
1. BIOS/UEFI
   ↓
2. GRUB (multiboot)
   ↓
3. entry.S (_start)
   - Setup stack
   - Setup segmentos
   - Call kernel_main()
   ↓
4. kernel_main()
   - vga_init()
   - gdt_init()
   - idt_init()
   - paging_init()
   - timer_init()
   - keyboard_init()
   - multitask_init()
   - sti (enable interrupts)
   ↓
5. Kernel Loop
   - HLT (aguarda interrupções)
   - Scheduler (a cada 100ms)
```

## Mapa de Memória

```
0x00000000 - 0x000FFFFF: BIOS/ROM (1MB)
0x00100000 - 0x001FFFFF: Kernel (1MB)
0x00200000 - 0x003FFFFF: Heap do kernel (2MB)
0x00400000 - 0x07FFFFFF: Userland (124MB)
0x08000000 - 0xFFFFFFFF: Reservado
```

## Interrupções

| Número | Tipo | Descrição |
|--------|------|-----------|
| 0-31   | ISR  | Exceções do processador |
| 32     | IRQ0 | Timer (PIT) |
| 33     | IRQ1 | Teclado |
| 34     | IRQ2 | Cascata PIC |
| 35-39  | IRQ  | Serial, paralela, etc. |
| 40     | IRQ8 | RTC |
| 44     | IRQ12| Mouse PS/2 |
| 46     | IRQ14| ATA Primary |
| 47     | IRQ15| ATA Secondary |

## Próximas Implementações

1. **Mouse Driver**: Completar driver PS/2
2. **Framebuffer**: Modo gráfico VESA
3. **Window Manager**: Sistema básico de janelas
4. **Sistema de Arquivos**: FAT32 básico
5. **Rede**: Driver PCnet + lwIP
6. **Aplicações**: Shell, file manager, editor
7. **Cursor IA**: Integração completa com ML
