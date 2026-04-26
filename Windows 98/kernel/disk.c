#include "disk.h"
#include "irq.h"

static bool disk_ready = false;

static void ata_wait_bsy(void) {
    for (int i = 0; i < 100000; i++) {
        if ((inb(0x1F7) & 0x80) == 0) {
            return;
        }
    }
}

static bool ata_wait_drq(void) {
    for (int i = 0; i < 100000; i++) {
        uint8_t status = inb(0x1F7);
        if (status & 0x01) {
            return false;
        }
        if (status & 0x08) {
            return true;
        }
    }
    return false;
}

void disk_init(void) {
    ata_wait_bsy();
    disk_ready = true;
}

bool disk_is_ready(void) {
    return disk_ready;
}

bool disk_read_sector_lba28(uint32_t lba, uint8_t *buffer) {
    if (!disk_ready || !buffer || (lba >> 28) != 0) {
        return false;
    }

    ata_wait_bsy();
    outb(0x1F6, 0xE0 | ((lba >> 24) & 0x0F));
    outb(0x1F2, 1);
    outb(0x1F3, (uint8_t)(lba & 0xFF));
    outb(0x1F4, (uint8_t)((lba >> 8) & 0xFF));
    outb(0x1F5, (uint8_t)((lba >> 16) & 0xFF));
    outb(0x1F7, 0x20);

    if (!ata_wait_drq()) {
        return false;
    }

    for (int i = 0; i < 256; i++) {
        uint16_t word;
        asm volatile("inw %1, %0" : "=a"(word) : "Nd"((uint16_t)0x1F0));
        buffer[i * 2] = (uint8_t)(word & 0xFF);
        buffer[i * 2 + 1] = (uint8_t)((word >> 8) & 0xFF);
    }

    return true;
}
