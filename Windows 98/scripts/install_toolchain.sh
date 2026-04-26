#!/bin/bash
# Script para instalar cross-compiler i686-elf
# Baseado em: https://wiki.osdev.org/GCC_Cross-Compiler

set -e

PREFIX="$HOME/opt/cross"
TARGET=i686-elf
PATH="$PREFIX/bin:$PATH"

echo "Instalando cross-compiler $TARGET em $PREFIX"

# Criar diretório
mkdir -p "$PREFIX"

# Versões
BINUTILS_VERSION=2.40
GCC_VERSION=13.2.0

# Diretório temporário
BUILD_DIR=$(mktemp -d)
cd "$BUILD_DIR"

echo "Baixando binutils..."
wget -q "https://ftp.gnu.org/gnu/binutils/binutils-${BINUTILS_VERSION}.tar.xz"
tar xf "binutils-${BINUTILS_VERSION}.tar.xz"

echo "Baixando GCC..."
wget -q "https://ftp.gnu.org/gnu/gcc/gcc-${GCC_VERSION}/gcc-${GCC_VERSION}.tar.xz"
tar xf "gcc-${GCC_VERSION}.tar.xz"

# Compilar binutils
echo "Compilando binutils..."
mkdir -p build-binutils
cd build-binutils
../binutils-${BINUTILS_VERSION}/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make -j$(nproc)
make install
cd ..

# Compilar GCC
echo "Compilando GCC..."
mkdir -p build-gcc
cd build-gcc
../gcc-${GCC_VERSION}/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j$(nproc)
make all-target-libgcc -j$(nproc)
make install-gcc
make install-target-libgcc
cd ..

echo "Cross-compiler instalado em $PREFIX/bin"
echo "Adicione ao PATH: export PATH=\"$PREFIX/bin:\$PATH\""

# Limpar
cd /
rm -rf "$BUILD_DIR"
