#include <stdint.h>
#include <stddef.h>
#include "kernel.h"
#include "gdt.h"
#include "idt.h"
#include "paging.h"
#include "timer.h"
#include "keyboard.h"
#include "mouse.h"
#include "disk.h"
#include "fat32.h"
#include "vga.h"
#include "gui.h"
#include "multitask.h"
#include "utils.h"
#include "debug.h"

// Multiboot info structure
struct multiboot_info {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;
} __attribute__((packed));

static struct multiboot_info *mb_info = NULL;

void kernel_main(uint32_t magic, struct multiboot_info *mbi) {
    // #region agent log
    debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-1\",\"hypothesisId\":\"H1\",\"location\":\"kernel/main.c:kernel_main:entry\",\"message\":\"kernel_main entered\",\"data\":{\"magic\":732803074},\"timestamp\":0}");
    // #endregion

    // Verify multiboot magic
    if (magic != 0x2BADB002) {
        // Not booted by multiboot, continue anyway
    }
    
    mb_info = mbi;
    
    // Initialize VGA text mode first for debugging
    vga_init();
    vga_puts("MeuSO 1999 - Inicializando...\n");
    
    // Initialize GDT
    vga_puts("Configurando GDT...\n");
    gdt_init();
    // #region agent log
    debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-1\",\"hypothesisId\":\"H2\",\"location\":\"kernel/main.c:after_gdt\",\"message\":\"gdt_init completed\",\"data\":{\"stage\":\"gdt\"},\"timestamp\":0}");
    // #endregion
    
    // Initialize IDT
    vga_puts("Configurando IDT...\n");
    idt_init();
    // #region agent log
    debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-1\",\"hypothesisId\":\"H2\",\"location\":\"kernel/main.c:after_idt\",\"message\":\"idt_init completed\",\"data\":{\"stage\":\"idt\"},\"timestamp\":0}");
    // #endregion
    
    // Initialize paging
    vga_puts("Configurando paginacao...\n");
    paging_init();
    
    // Initialize timer
    vga_puts("Configurando timer...\n");
    timer_init(100); // 100 Hz
    
    // Initialize keyboard
    vga_puts("Configurando teclado...\n");
    keyboard_init();

    // Initialize mouse
    vga_puts("Configurando mouse PS/2...\n");
    mouse_init();

    // Initialize disk
    vga_puts("Configurando disco ATA...\n");
    disk_init();

    // Initialize FAT32
    vga_puts("Detectando FAT32...\n");
    if (fat32_init()) {
        vga_puts("FAT32 detectado com sucesso.\n");
    } else {
        vga_puts("FAT32 nao detectado (seguindo sem montar).\n");
    }
    
    // Initialize multitasking
    vga_puts("Configurando multitarefa...\n");
    multitask_init();
    
    // Enable interrupts
    asm volatile("sti");
    // #region agent log
    debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-1\",\"hypothesisId\":\"H3\",\"location\":\"kernel/main.c:after_sti\",\"message\":\"interrupts enabled\",\"data\":{\"stage\":\"sti\"},\"timestamp\":0}");
    // #endregion
    
    gui_init();
    gui_render_boot_screen();
    
    // Main kernel loop
    for (;;) {
        static int logged_loop = 0;
        if (!logged_loop) {
            // #region agent log
            debug_log_line("{\"sessionId\":\"00ae61\",\"runId\":\"run-1\",\"hypothesisId\":\"H5\",\"location\":\"kernel/main.c:main_loop\",\"message\":\"entered main gui loop\",\"data\":{\"loop\":1},\"timestamp\":0}");
            // #endregion
            logged_loop = 1;
        }
        gui_update();
        asm volatile("nop");
    }
}

// Simple itoa implementation
void itoa(int value, char *str, int base) {
    char *ptr = str;
    char *ptr1 = str;
    char tmp_char;
    int tmp_value;
    
    if (value < 0 && base == 10) {
        *ptr++ = '-';
        value = -value;
        ptr1++;
    }
    
    do {
        tmp_value = value;
        value /= base;
        *ptr++ = "0123456789abcdef"[tmp_value - value * base];
    } while (value);
    
    *ptr-- = '\0';
    while (ptr1 < ptr) {
        tmp_char = *ptr;
        *ptr-- = *ptr1;
        *ptr1++ = tmp_char;
    }
}
