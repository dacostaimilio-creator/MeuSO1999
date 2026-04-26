#ifndef GFX_H
#define GFX_H

#include <stdbool.h>
#include <stdint.h>

#define GFX_WIDTH 320
#define GFX_HEIGHT 200

bool gfx_init_mode13h(void);
void gfx_putpixel(int x, int y, uint8_t color);
void gfx_fill_rect(int x, int y, int w, int h, uint8_t color);
void gfx_clear(uint8_t color);

#endif
