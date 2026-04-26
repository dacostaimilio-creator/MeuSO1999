#include "idt.h"
#include "isr.h"
#include "irq.h"
#include "kernel.h"

// IDT entries
struct idt_entry idt[256];
struct idt_ptr idtp;

// Set IDT entry
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt[num].base_lo = (base & 0xFFFF);
    idt[num].base_hi = (base >> 16) & 0xFFFF;
    
    idt[num].sel = sel;
    idt[num].always0 = 0;
    idt[num].flags = flags;
}

// Initialize IDT
void idt_init(void) {
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;
    idtp.base = (uint32_t)&idt;
    
    memset(&idt, 0, sizeof(struct idt_entry) * 256);
    
    // Install ISRs
    isr_install();
    
    // Install IRQs
    irq_install();
    
    // Load IDT
    idt_load((uint32_t)&idtp);
}
