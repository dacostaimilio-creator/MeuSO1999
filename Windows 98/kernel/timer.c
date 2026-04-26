#include "timer.h"
#include "irq.h"
#include "multitask.h"
#include "kernel.h"

volatile uint32_t tick = 0;

// Timer interrupt handler
static void timer_callback(struct regs *r) {
    tick++;
    
    // Call scheduler every 10 ticks (100ms at 100Hz)
    if (tick % 10 == 0) {
        schedule();
    }
}

// Initialize timer (PIT)
void timer_init(uint32_t frequency) {
    // Register timer interrupt handler
    register_irq_handler(IRQ0, timer_callback);
    
    // Calculate divisor
    uint32_t divisor = 1193180 / frequency;
    
    // Send command byte
    outb(0x43, 0x36);
    
    // Send divisor (low and high bytes)
    outb(0x40, (uint8_t)(divisor & 0xFF));
    outb(0x40, (uint8_t)((divisor >> 8) & 0xFF));
}

uint32_t timer_get_tick(void) {
    return tick;
}
