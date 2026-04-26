#ifndef CURSOR_IA_H
#define CURSOR_IA_H

#include <stdint.h>
#include <stdbool.h>

// Cursor Engine API
void cursor_engine_init(void);
void cursor_engine_set_position(int x, int y);
void cursor_engine_get_position(int *x, int *y);
void cursor_engine_draw(uint32_t *framebuffer, int width, int height);
void cursor_engine_set_visible(bool visible);
bool cursor_engine_is_visible(void);

// Cursor Agent API
typedef struct {
    char window_title[256];
    char selected_text[1024];
    int mouse_x, mouse_y;
    bool has_focus;
} ui_context_t;

typedef struct {
    char action_type[64];
    char description[256];
    char target[128];
    int confidence;
} suggested_action_t;

void cursor_agent_init(void);
void cursor_agent_update_context(const ui_context_t *ctx);
suggested_action_t cursor_agent_analyze(void);
suggested_action_t cursor_agent_get_suggestion(void);
void cursor_agent_on_mouse_move(int x, int y);
void cursor_agent_on_keypress(char key);

#endif
