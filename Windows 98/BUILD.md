# Guia de Build - MeuSO 1999

## Pré-requisitos

### Linux (Ubuntu/Debian)

```bash
# Instalar ferramentas básicas
sudo apt-get update
sudo apt-get install build-essential nasm xorriso grub-pc-bin qemu-system-i386

# Instalar cross-compiler i686-elf
# Opção 1: Tentar pacote (pode não estar disponível)
sudo apt-get install gcc-i686-elf binutils-i686-elf

# Opção 2: Compilar cross-compiler manualmente (recomendado)
# Siga: https://wiki.osdev.org/GCC_Cross-Compiler
```

### Windows (WSL ou MSYS2)

Se estiver no Windows, use WSL2 ou MSYS2:

```bash
# No WSL2
sudo apt-get update
sudo apt-get install build-essential nasm xorriso grub-pc-bin qemu-system-i386
```

## Compilação

### 1. Configurar ambiente

```bash
# Criar estrutura de diretórios
make setup
```

### 2. Compilar kernel

```bash
# Compilar tudo
make

# Ou passo a passo:
make clean
make kernel.bin
```

### 3. Testar com QEMU

```bash
# Testar kernel diretamente
make qemu

# Ou criar ISO e testar
make iso
make qemu-iso
```

## Estrutura de Build

```
kernel.bin          # Kernel compilado (binário)
isodir/             # Arquivos para ISO
  boot/
    kernel.bin
    grub/
      grub.cfg
meuSO.iso           # ISO final (após make iso)
```

## Troubleshooting

### Erro: "i686-elf-gcc: command not found"

Você precisa instalar o cross-compiler. Veja seção de pré-requisitos.

### Erro: "grub-mkrescue: command not found"

Instale grub-pc-bin:
```bash
sudo apt-get install grub-pc-bin
```

### Erro: "qemu-system-i386: command not found"

Instale QEMU:
```bash
sudo apt-get install qemu-system-i386
```

### Kernel não boota

1. Verifique se o kernel.bin foi criado corretamente
2. Teste primeiro com QEMU antes do VirtualBox
3. Verifique os logs do GRUB

## Próximos Passos

Após compilar com sucesso:

1. Teste no QEMU: `make qemu`
2. Crie ISO: `make iso`
3. Teste ISO: `make qemu-iso`
4. Configure VM no VirtualBox (veja README.md)
