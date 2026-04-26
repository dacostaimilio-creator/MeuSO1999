#ifndef KERNEL_H
#define KERNEL_H

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

// Kernel version
#define KERNEL_VERSION "0.1.0"
#define KERNEL_NAME "MeuSO 1999"

// Memory layout
#define KERNEL_BASE 0x100000
#define KERNEL_STACK_SIZE 16384
#define PAGE_SIZE 4096

// VGA
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// Helper functions
void itoa(int value, char *str, int base);
void *memset(void *s, int c, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
size_t strlen(const char *s);

// Panic function
void panic(const char *message);

#endif
