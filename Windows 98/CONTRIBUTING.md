# Guia de Contribuição - MeuSO 1999

## Estrutura do Projeto

```
/
├── boot/              # Bootloader e código assembly
│   ├── entry.S        # Ponto de entrada multiboot
│   ├── gdt_flush.S    # Função de flush GDT
│   ├── idt_load.S     # Função de load IDT
│   ├── isr_stubs.S    # Stubs de interrupções
│   └── task_switch.S  # Troca de contexto
├── kernel/            # Código do kernel
│   ├── main.c         # Função principal
│   ├── gdt.c          # Gerenciamento de segmentos
│   ├── idt.c          # Gerenciamento de interrupções
│   ├── isr.c          # ISR handlers
│   ├── irq.c          # IRQ handlers
│   ├── paging.c       # Gerenciamento de memória
│   ├── timer.c        # Timer PIT
│   ├── keyboard.c     # Driver de teclado
│   ├── vga.c          # Driver VGA
│   ├── multitask.c    # Multitarefa
│   └── utils.c        # Funções utilitárias
├── include/           # Headers
├── userland/          # Aplicações userland
│   └── cursor_ia/     # Componente Cursor IA
└── fsroot/           # Sistema de arquivos raiz
```

## Convenções de Código

### Estilo

- Use tabs para indentação (4 espaços equivalentes)
- Nomes de funções em snake_case
- Nomes de constantes em UPPER_CASE
- Comentários em português
- Headers em inglês (padrão)

### Estrutura de Arquivos

- Um arquivo `.c` por módulo
- Header correspondente em `include/`
- Documentação inline para funções complexas

## Áreas para Contribuição

### Prioridade Alta

1. **Drivers Completos**
   - Mouse PS/2 completo
   - Driver de disco ATA
   - Driver de rede (PCnet)

2. **Sistema de Arquivos**
   - Implementação FAT32
   - Suporte a initrd

3. **GUI Básica**
   - Window manager
   - Framebuffer driver
   - Sistema de eventos

### Prioridade Média

1. **Aplicações Userland**
   - Shell completo
   - File manager
   - Editor de texto

2. **Rede**
   - Integração lwIP
   - Stack TCP/IP básico

3. **Cursor IA**
   - Integração com ML local
   - Sugestões contextuais avançadas

### Prioridade Baixa

1. **Otimizações**
   - Melhorar scheduler
   - Otimizar gerenciamento de memória

2. **Documentação**
   - Mais exemplos
   - Tutoriais

## Como Contribuir

1. Fork o repositório
2. Crie uma branch para sua feature
3. Faça suas mudanças
4. Teste com `make qemu`
5. Envie um pull request

## Testes

Sempre teste suas mudanças:

```bash
make clean
make
make qemu
```

## Perguntas?

Abra uma issue no repositório para discussão.
