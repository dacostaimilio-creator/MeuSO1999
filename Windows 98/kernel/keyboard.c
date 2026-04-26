#include "keyboard.h"
#include "irq.h"
#include "kernel.h"
#include "vga.h"

static char key_buffer[256];
static int key_buffer_pos = 0;

// Scancode to ASCII mapping (US layout)
static char scancode_to_ascii[] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Keyboard interrupt handler
static void keyboard_callback(struct regs *r) {
    uint8_t scancode = inb(0x60);
    
    if (scancode < sizeof(scancode_to_ascii)) {
        char c = scancode_to_ascii[scancode];
        if (c != 0) {
            // Add to buffer
            if (key_buffer_pos < sizeof(key_buffer) - 1) {
                key_buffer[key_buffer_pos++] = c;
                key_buffer[key_buffer_pos] = '\0';
                
                // Echo to screen
                vga_putchar(c);
            }
        }
    }
}

// Initialize keyboard
void keyboard_init(void) {
    register_irq_handler(IRQ1, keyboard_callback);
}

// Get key from buffer
char keyboard_getchar(void) {
    if (key_buffer_pos > 0) {
        char c = key_buffer[0];
        // Shift buffer
        for (int i = 0; i < key_buffer_pos; i++) {
            key_buffer[i] = key_buffer[i + 1];
        }
        key_buffer_pos--;
        return c;
    }
    return 0;
}
