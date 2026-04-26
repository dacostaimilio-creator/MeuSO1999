#ifndef IRQ_H
#define IRQ_H

#include <stdint.h>
#include "isr.h"

// IRQ handler type
typedef void (*irq_t)(struct regs *);

#define IRQ0 32
#define IRQ1 33
#define IRQ2 34
#define IRQ3 35
#define IRQ4 36
#define IRQ5 37
#define IRQ6 38
#define IRQ7 39
#define IRQ8 40
#define IRQ9 41
#define IRQ10 42
#define IRQ11 43
#define IRQ12 44
#define IRQ13 45
#define IRQ14 46
#define IRQ15 47

void irq_install(void);
void register_irq_handler(int irq, irq_t handler);
void irq_handler(struct regs *r);
void irq_remap(void);

// I/O port functions
void outb(uint16_t port, uint8_t val);
uint8_t inb(uint16_t port);

#endif
