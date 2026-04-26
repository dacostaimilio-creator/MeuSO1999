#ifndef VGA_H
#define VGA_H

void vga_init(void);
void vga_clear(void);
void vga_putchar(char c);
void vga_puts(const char *str);
void vga_set_cursor(int x, int y);

#endif
