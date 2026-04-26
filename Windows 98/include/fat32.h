#ifndef FAT32_H
#define FAT32_H

#include <stdbool.h>
#include <stdint.h>

typedef struct {
    bool valid;
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sectors;
    uint8_t fat_count;
    uint32_t sectors_per_fat;
    uint32_t root_cluster;
} fat32_info_t;

bool fat32_init(void);
bool fat32_is_ready(void);
const fat32_info_t *fat32_get_info(void);

#endif
