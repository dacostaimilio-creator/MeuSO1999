#include "gui.h"
#include "keyboard.h"
#include "mouse.h"
#include "timer.h"
#include "debug.h"
#include "kernel.h"

static int gui_initialized = 0;
static int win_x = 18;
static int win_y = 5;
static int boot_phase = 0;
static uint32_t boot_end_tick = 0;
static uint32_t last_boot_anim_tick = 0;
static int boot_bar_pos = 0;

static volatile uint16_t *const text_mem = (uint16_t *)VGA_MEMORY;

static inline uint16_t vga_cell(char c, uint8_t fg, uint8_t bg) {
    return (uint16_t)c | (uint16_t)((fg | (bg << 4)) << 8);
}

static void put_cell(int x, int y, char c, uint8_t fg, uint8_t bg) {
    if (x < 0 || y < 0 || x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
    text_mem[y * VGA_WIDTH + x] = vga_cell(c, fg, bg);
}

static void fill_rect_chars(int x, int y, int w, int h, char c, uint8_t fg, uint8_t bg) {
    for (int yy = y; yy < y + h; yy++) {
        for (int xx = x; xx < x + w; xx++) {
            put_cell(xx, yy, c, fg, bg);
        }
    }
}

static void draw_text(int x, int y, const char *s, uint8_t fg, uint8_t bg) {
    for (int i = 0; s[i] != '\0'; i++) {
        put_cell(x + i, y, s[i], fg, bg);
    }
}

static void draw_xp_boot_screen_text(void) {
    fill_rect_chars(0, 0, VGA_WIDTH, VGA_HEIGHT, ' ', 15, 1);
    draw_text(28, 8, "Microsoft Windows XP", 15, 1);
    draw_text(22, 11, "Iniciando sistema classico...", 7, 1);
    fill_rect_chars(20, 15, 40, 1, ' ', 7, 8);
    // moving chunk animation inside the boot bar
    for (int i = 0; i < 8; i++) {
        int p = 22 + ((boot_bar_pos + i) % 36);
        put_cell(p, 15, ' ', 15, 11);
    }
    draw_text(24, 18, "Tema retro carregando drivers...", 7, 1);
}

static void draw_desktop(void) {
    fill_rect_chars(0, 0, VGA_WIDTH, VGA_HEIGHT, ' ', 15, 3);
    fill_rect_chars(0, VGA_HEIGHT - 1, VGA_WIDTH, 1, ' ', 0, 8);
    fill_rect_chars(0, VGA_HEIGHT - 1, 10, 1, ' ', 0, 2);
    draw_text(1, VGA_HEIGHT - 1, "Iniciar", 15, 2);

    fill_rect_chars(2, 2, 13, 1, ' ', 0, 3);
    draw_text(2, 2, "[Meu Computador]", 15, 3);
    fill_rect_chars(2, 4, 10, 1, ' ', 0, 3);
    draw_text(2, 4, "[Lixeira]", 15, 3);

    fill_rect_chars(win_x, win_y, 38, 11, ' ', 0, 7);
    fill_rect_chars(win_x, win_y, 38, 1, ' ', 15, 1);
    draw_text(win_x + 1, win_y, "MeuSO Explorer", 15, 1);
    draw_text(win_x + 2, win_y + 2, "Tema: classico antigo", 0, 7);
    draw_text(win_x + 2, win_y + 3, "Mover janela: W A S D", 0, 7);
}

static void draw_cursor(int x, int y) {
    put_cell(x, y, 'X', 15, 4);
}

void gui_init(void) {
    // #region agent log
    debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-2\",\"hypothesisId\":\"H6\",\"location\":\"kernel/gui.c:gui_init:start\",\"message\":\"gui init text mode compatibility path\",\"data\":{\"mode\":\"text\"},\"timestamp\":0}");
    // #endregion
    gui_initialized = 1;
    // #region agent log
    debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-2\",\"hypothesisId\":\"H6\",\"location\":\"kernel/gui.c:gui_init:ok\",\"message\":\"text mode gui initialized\",\"data\":{\"gui\":1},\"timestamp\":0}");
    // #endregion
}

void gui_render_boot_screen(void) {
    if (!gui_initialized) {
        // #region agent log
        debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-2\",\"hypothesisId\":\"H6\",\"location\":\"kernel/gui.c:render:skip\",\"message\":\"render skipped because gui not initialized\",\"data\":{\"gui\":0},\"timestamp\":0}");
        // #endregion
        return;
    }
    // #region agent log
    debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-2\",\"hypothesisId\":\"H6\",\"location\":\"kernel/gui.c:render:start\",\"message\":\"render boot and desktop in text mode\",\"data\":{\"gui\":1},\"timestamp\":0}");
    // #endregion
    boot_phase = 1;
    boot_bar_pos = 0;
    last_boot_anim_tick = timer_get_tick();
    boot_end_tick = last_boot_anim_tick + 350; // ~3.5s at 100Hz
    draw_xp_boot_screen_text();
}

void gui_update(void) {
    if (!gui_initialized) {
        return;
    }

    if (boot_phase) {
        uint32_t now = timer_get_tick();
        if (now >= boot_end_tick) {
            boot_phase = 0;
            draw_desktop();
            draw_cursor(mouse_get_x(), mouse_get_y());
            return;
        }

        if (now != last_boot_anim_tick) {
            last_boot_anim_tick = now;
            boot_bar_pos = (boot_bar_pos + 1) % 36;
            draw_xp_boot_screen_text();
        }
        return;
    }

    char c = keyboard_getchar();
    if (c == 'a') win_x -= 1;
    if (c == 'd') win_x += 1;
    if (c == 'w') win_y -= 1;
    if (c == 's') win_y += 1;

    if (win_x < 0) win_x = 0;
    if (win_y < 0) win_y = 0;
    if (win_x > VGA_WIDTH - 38) win_x = VGA_WIDTH - 38;
    if (win_y > VGA_HEIGHT - 12) win_y = VGA_HEIGHT - 12;

    draw_desktop();
    draw_cursor(mouse_get_x(), mouse_get_y());
}
