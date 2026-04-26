#include "paging.h"
#include "kernel.h"

// Page directory and tables
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

// Set page table entry
void set_page_table_entry(uint32_t *page_table, uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags) {
    uint32_t index = virtual_addr / PAGE_SIZE;
    page_table[index] = (physical_addr & 0xFFFFF000) | flags;
}

// Initialize paging
void paging_init(void) {
    // Clear page directory
    memset(page_directory, 0, sizeof(page_directory));
    
    // Clear first page table
    memset(first_page_table, 0, sizeof(first_page_table));
    
    // Map first 4MB of memory (identity mapping for kernel)
    for (uint32_t i = 0; i < 1024; i++) {
        first_page_table[i] = (i * PAGE_SIZE) | 0x03; // Present + R/W
    }
    
    // Set first page table in page directory
    page_directory[0] = ((uint32_t)first_page_table) | 0x03; // Present + R/W
    
    // Enable paging
    enable_paging();
}

// Enable paging
void enable_paging(void) {
    // Load page directory
    asm volatile("mov %0, %%cr3" : : "r"(page_directory));
    
    // Enable paging in CR0
    uint32_t cr0;
    asm volatile("mov %%cr0, %0" : "=r"(cr0));
    cr0 |= 0x80000000; // Set PG bit
    asm volatile("mov %0, %%cr0" : : "r"(cr0));
}
