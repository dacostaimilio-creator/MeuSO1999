#include "mouse.h"
#include "irq.h"

static volatile int mouse_x = 40;
static volatile int mouse_y = 12;
static volatile bool mouse_ready = false;
static uint8_t mouse_cycle = 0;
static int8_t mouse_bytes[3];

static void mouse_wait_write(void) {
    for (int i = 0; i < 100000; i++) {
        if ((inb(0x64) & 0x02) == 0) {
            return;
        }
    }
}

static void mouse_wait_read(void) {
    for (int i = 0; i < 100000; i++) {
        if (inb(0x64) & 0x01) {
            return;
        }
    }
}

static void mouse_write(uint8_t data) {
    mouse_wait_write();
    outb(0x64, 0xD4);
    mouse_wait_write();
    outb(0x60, data);
}

static uint8_t mouse_read(void) {
    mouse_wait_read();
    return inb(0x60);
}

static void mouse_callback(struct regs *r) {
    (void)r;
    mouse_bytes[mouse_cycle++] = (int8_t)inb(0x60);

    if (mouse_cycle >= 3) {
        mouse_cycle = 0;

        mouse_x += mouse_bytes[1];
        mouse_y -= mouse_bytes[2];

        if (mouse_x < 0) mouse_x = 0;
        if (mouse_y < 0) mouse_y = 0;
        if (mouse_x > 79) mouse_x = 79;
        if (mouse_y > 24) mouse_y = 24;
    }
}

void mouse_init(void) {
    register_irq_handler(IRQ12, mouse_callback);

    mouse_wait_write();
    outb(0x64, 0xA8);

    mouse_wait_write();
    outb(0x64, 0x20);
    mouse_wait_read();
    uint8_t status = inb(0x60) | 0x02;

    mouse_wait_write();
    outb(0x64, 0x60);
    mouse_wait_write();
    outb(0x60, status);

    mouse_write(0xF6);
    (void)mouse_read();
    mouse_write(0xF4);
    (void)mouse_read();

    mouse_ready = true;
}

bool mouse_is_ready(void) {
    return mouse_ready;
}

int mouse_get_x(void) {
    return mouse_x;
}

int mouse_get_y(void) {
    return mouse_y;
}
