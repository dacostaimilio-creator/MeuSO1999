// Cursor Engine - Componente de desenho e animação do cursor
// Este é o componente que controla a aparência visual do cursor

#include <stdint.h>
#include <stdbool.h>

// Cursor sprite (16x16 pixels, estilo 1999)
static uint8_t cursor_sprite[16][16] = {
    {1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,2,1,0,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,2,2,1,0,0,0,0,0,0,0,0,0,0,0},
    {1,2,2,2,2,1,0,0,0,0,0,0,0,0,0,0},
    {1,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0},
    {1,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0},
    {1,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0},
    {1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0},
    {1,2,2,2,2,2,1,1,1,1,0,0,0,0,0,0},
    {1,2,2,1,2,2,1,0,0,0,0,0,0,0,0,0},
    {1,2,1,0,1,2,2,1,0,0,0,0,0,0,0,0},
    {1,1,0,0,1,2,2,1,0,0,0,0,0,0,0,0},
    {1,0,0,0,0,1,2,2,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,1,2,2,1,0,0,0,0,0,0,0},
    {0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0}
};
// 0 = transparente, 1 = preto, 2 = branco

// Posição do cursor
static int cursor_x = 0;
static int cursor_y = 0;
static bool cursor_visible = true;

// Inicializar cursor engine
void cursor_engine_init(void) {
    cursor_x = 0;
    cursor_y = 0;
    cursor_visible = true;
}

// Atualizar posição do cursor
void cursor_engine_set_position(int x, int y) {
    cursor_x = x;
    cursor_y = y;
}

// Obter posição do cursor
void cursor_engine_get_position(int *x, int *y) {
    *x = cursor_x;
    *y = cursor_y;
}

// Desenhar cursor no framebuffer
void cursor_engine_draw(uint32_t *framebuffer, int width, int height) {
    if (!cursor_visible) {
        return;
    }
    
    // Desenhar sprite do cursor
    for (int py = 0; py < 16; py++) {
        for (int px = 0; px < 16; px++) {
            int fx = cursor_x + px;
            int fy = cursor_y + py;
            
            if (fx >= 0 && fx < width && fy >= 0 && fy < height) {
                uint8_t pixel = cursor_sprite[py][px];
                uint32_t color = 0;
                
                switch (pixel) {
                    case 1: color = 0x000000; break; // Preto
                    case 2: color = 0xFFFFFF; break; // Branco
                    default: continue; // Transparente
                }
                
                framebuffer[fy * width + fx] = color;
            }
        }
    }
}

// Mostrar/ocultar cursor
void cursor_engine_set_visible(bool visible) {
    cursor_visible = visible;
}

bool cursor_engine_is_visible(void) {
    return cursor_visible;
}
