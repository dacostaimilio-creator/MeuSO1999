#ifndef ISR_H
#define ISR_H

#include <stdint.h>

// Registers structure
struct regs {
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, cs, eflags, useresp, ss;
};

// ISR handler type
typedef void (*isr_t)(struct regs *);

void isr_install(void);
void register_interrupt_handler(uint8_t n, isr_t handler);
void isr_handler(struct regs *r);

#endif
