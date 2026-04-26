#include "gfx.h"
#include "irq.h"

static volatile uint8_t *const vga_fb = (uint8_t *)0xA0000;
static bool gfx_ready = false;

static void write_regs(const uint8_t *regs) {
    outb(0x3C2, *regs++);

    for (uint8_t i = 0; i < 5; i++) {
        outb(0x3C4, i);
        outb(0x3C5, *regs++);
    }

    uint8_t crtc[25];
    for (uint8_t i = 0; i < 25; i++) {
        crtc[i] = *regs++;
    }
    crtc[0x03] |= 0x80;
    crtc[0x11] &= (uint8_t)~0x80;

    for (uint8_t i = 0; i < 25; i++) {
        outb(0x3D4, i);
        outb(0x3D5, crtc[i]);
    }

    for (uint8_t i = 0; i < 9; i++) {
        outb(0x3CE, i);
        outb(0x3CF, *regs++);
    }

    for (uint8_t i = 0; i < 21; i++) {
        (void)inb(0x3DA);
        outb(0x3C0, i);
        outb(0x3C0, *regs++);
    }

    (void)inb(0x3DA);
    outb(0x3C0, 0x20);
}

bool gfx_init_mode13h(void) {
    static uint8_t regs[] = {
        0x63,
        0x03, 0x01, 0x0F, 0x00, 0x0E,
        0x5F, 0x4F, 0x50, 0x82, 0x54, 0x80, 0xBF, 0x1F, 0x00, 0x41,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x9C, 0x0E, 0x8F, 0x28, 0x40,
        0x96, 0xB9, 0xA3, 0xFF,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x05, 0x0F, 0xFF,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x14, 0x07,
        0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
        0x0C, 0x00, 0x0F, 0x08, 0x00
    };

    write_regs(regs);
    gfx_ready = true;
    gfx_clear(0);
    return true;
}

void gfx_putpixel(int x, int y, uint8_t color) {
    if (!gfx_ready || x < 0 || y < 0 || x >= GFX_WIDTH || y >= GFX_HEIGHT) {
        return;
    }
    vga_fb[y * GFX_WIDTH + x] = color;
}

void gfx_fill_rect(int x, int y, int w, int h, uint8_t color) {
    if (!gfx_ready || w <= 0 || h <= 0) {
        return;
    }

    for (int yy = y; yy < y + h; yy++) {
        for (int xx = x; xx < x + w; xx++) {
            gfx_putpixel(xx, yy, color);
        }
    }
}

void gfx_clear(uint8_t color) {
    if (!gfx_ready) {
        return;
    }
    for (int i = 0; i < (GFX_WIDTH * GFX_HEIGHT); i++) {
        vga_fb[i] = color;
    }
}
