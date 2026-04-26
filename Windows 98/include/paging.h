#ifndef PAGING_H
#define PAGING_H

#include <stdint.h>
#include "kernel.h"

#define PAGE_PRESENT 0x01
#define PAGE_RW 0x02
#define PAGE_USER 0x04

void paging_init(void);
void enable_paging(void);
void set_page_table_entry(uint32_t *page_table, uint32_t virtual_addr, uint32_t physical_addr, uint32_t flags);

#endif
