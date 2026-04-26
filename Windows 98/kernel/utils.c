#include "kernel.h"
#include "utils.h"
#include "vga.h"

// Memset
void *memset(void *s, int c, size_t n) {
    unsigned char *p = (unsigned char *)s;
    for (size_t i = 0; i < n; i++) {
        p[i] = (unsigned char)c;
    }
    return s;
}

// Memcpy
void *memcpy(void *dest, const void *src, size_t n) {
    unsigned char *d = (unsigned char *)dest;
    const unsigned char *s = (const unsigned char *)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

// Strlen
size_t strlen(const char *s) {
    size_t len = 0;
    while (s[len] != '\0') {
        len++;
    }
    return len;
}

// Panic
void panic(const char *message) {
    // Disable interrupts
    asm volatile("cli");
    
    // Print panic message
    extern void vga_puts(const char *);
    vga_puts("\n\n*** KERNEL PANIC ***\n");
    vga_puts(message);
    vga_puts("\n");
    
    // Halt
    for (;;) {
        asm volatile("hlt");
    }
}
