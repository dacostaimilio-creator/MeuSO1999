#include "debug.h"

static inline void debug_outb(unsigned short port, unsigned char val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

void debug_log_line(const char *line) {
    if (!line) {
        return;
    }

    while (*line) {
        debug_outb(0xE9, (unsigned char)(*line));
        line++;
    }
    debug_outb(0xE9, '\n');
}
