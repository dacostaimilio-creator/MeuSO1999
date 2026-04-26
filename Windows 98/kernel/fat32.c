#include "fat32.h"
#include "disk.h"

typedef struct {
    uint8_t jump_boot[3];
    uint8_t oem_name[8];
    uint16_t bytes_per_sector;
    uint8_t sectors_per_cluster;
    uint16_t reserved_sector_count;
    uint8_t table_count;
    uint16_t root_entry_count;
    uint16_t total_sectors_16;
    uint8_t media_type;
    uint16_t table_size_16;
    uint16_t sectors_per_track;
    uint16_t head_side_count;
    uint32_t hidden_sector_count;
    uint32_t total_sectors_32;
    uint32_t table_size_32;
    uint16_t extended_flags;
    uint16_t fat_version;
    uint32_t root_cluster;
    uint16_t fat_info;
    uint16_t backup_BS_sector;
    uint8_t reserved_0[12];
    uint8_t drive_number;
    uint8_t reserved_1;
    uint8_t boot_signature;
    uint32_t volume_id;
    uint8_t volume_label[11];
    uint8_t fat_type_label[8];
} __attribute__((packed)) fat32_bpb_t;

static fat32_info_t info;

bool fat32_init(void) {
    uint8_t sector[512];
    info.valid = false;

    if (!disk_is_ready()) {
        return false;
    }

    if (!disk_read_sector_lba28(0, sector)) {
        return false;
    }

    const fat32_bpb_t *bpb = (const fat32_bpb_t *)sector;
    if (bpb->boot_signature != 0x29) {
        return false;
    }

    if (bpb->table_size_32 == 0 || bpb->root_cluster < 2) {
        return false;
    }

    info.bytes_per_sector = bpb->bytes_per_sector;
    info.sectors_per_cluster = bpb->sectors_per_cluster;
    info.reserved_sectors = bpb->reserved_sector_count;
    info.fat_count = bpb->table_count;
    info.sectors_per_fat = bpb->table_size_32;
    info.root_cluster = bpb->root_cluster;
    info.valid = true;
    return true;
}

bool fat32_is_ready(void) {
    return info.valid;
}

const fat32_info_t *fat32_get_info(void) {
    return &info;
}
