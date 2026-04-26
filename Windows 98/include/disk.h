#ifndef DISK_H
#define DISK_H

#include <stdbool.h>
#include <stdint.h>

void disk_init(void);
bool disk_is_ready(void);
bool disk_read_sector_lba28(uint32_t lba, uint8_t *buffer);

#endif
