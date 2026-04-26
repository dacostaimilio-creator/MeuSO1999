#ifndef MOUSE_H
#define MOUSE_H

#include <stdbool.h>
#include <stdint.h>

void mouse_init(void);
bool mouse_is_ready(void);
int mouse_get_x(void);
int mouse_get_y(void);

#endif
