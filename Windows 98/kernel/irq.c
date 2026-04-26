#include "irq.h"
#include "idt.h"
#include "isr.h"
#include "kernel.h"
#include "debug.h"

// IRQ handlers
irq_t irq_handlers[16];

// Register IRQ handler
void register_irq_handler(int irq, irq_t handler) {
    // Accept both PIC IRQ numbers (0-15) and IDT vectors (32-47).
    if (irq >= 32) {
        irq -= 32;
    }
    if (irq < 0 || irq >= 16) {
        return;
    }
    irq_handlers[irq] = handler;
}

// IRQ handler
void irq_handler(struct regs *r) {
    // IRQ number is interrupt number - 32
    int irq = r->int_no - 32;
    if (irq < 0 || irq >= 16) {
        // #region agent log
        debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-3\",\"hypothesisId\":\"H10\",\"location\":\"kernel/irq.c:irq_handler\",\"message\":\"irq out of range\",\"data\":{\"irq\":-1},\"timestamp\":0}");
        // #endregion
        return;
    }
    
    if (irq_handlers[irq] != 0) {
        irq_t handler = irq_handlers[irq];
        handler(r);
    }
    
    // Send EOI to PIC
    if (irq >= 8) {
        outb(0xA0, 0x20); // Send EOI to slave PIC
    }
    outb(0x20, 0x20); // Send EOI to master PIC
}

// Remap PIC
void irq_remap(void) {
    outb(0x20, 0x11);
    outb(0xA0, 0x11);
    outb(0x21, 0x20);
    outb(0xA1, 0x28);
    outb(0x21, 0x04);
    outb(0xA1, 0x02);
    outb(0x21, 0x01);
    outb(0xA1, 0x01);
    outb(0x21, 0x0);
    outb(0xA1, 0x0);
}

// Install IRQs
void irq_install(void) {
    irq_remap();
    
    // Install IRQ stubs (32-47)
    extern void irq0(), irq1(), irq2(), irq3(), irq4(), irq5(), irq6(), irq7();
    extern void irq8(), irq9(), irq10(), irq11(), irq12(), irq13(), irq14(), irq15();
    
    idt_set_gate(32, (uint32_t)irq0, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(33, (uint32_t)irq1, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(34, (uint32_t)irq2, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(35, (uint32_t)irq3, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(36, (uint32_t)irq4, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(37, (uint32_t)irq5, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(38, (uint32_t)irq6, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(39, (uint32_t)irq7, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(40, (uint32_t)irq8, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(41, (uint32_t)irq9, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(42, (uint32_t)irq10, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(43, (uint32_t)irq11, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(44, (uint32_t)irq12, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(45, (uint32_t)irq13, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(46, (uint32_t)irq14, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
    idt_set_gate(47, (uint32_t)irq15, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_INTERRUPT_GATE);
}

// I/O port functions
void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}
