#include "multitask.h"
#include "kernel.h"
#include "paging.h"

#define MAX_TASKS 256

static task_t tasks[MAX_TASKS];
static int task_count = 0;
static int current_task = 0;

// Initialize multitasking
void multitask_init(void) {
    task_count = 0;
    current_task = 0;
    memset(tasks, 0, sizeof(tasks));
}

// Create new task
int task_create(void (*entry)(void), uint32_t stack_size) {
    if (task_count >= MAX_TASKS) {
        return -1;
    }
    
    // Allocate stack
    uint32_t stack = (uint32_t)kmalloc(stack_size);
    if (!stack) {
        return -1;
    }
    
    // Setup task
    task_t *task = &tasks[task_count];
    task->id = task_count;
    task->state = TASK_READY;
    task->stack = stack;
    task->stack_size = stack_size;
    
    // Setup stack frame
    stack += stack_size;
    stack -= sizeof(task_context_t);
    task_context_t *ctx = (task_context_t *)stack;
    
    // Initialize context
    ctx->eip = (uint32_t)entry;
    ctx->eflags = 0x200; // Enable interrupts
    ctx->eax = 0;
    ctx->ebx = 0;
    ctx->ecx = 0;
    ctx->edx = 0;
    ctx->esi = 0;
    ctx->edi = 0;
    ctx->ebp = 0;
    ctx->esp = (uint32_t)stack;
    
    task->context = ctx;
    task_count++;
    
    return task->id;
}

// Schedule next task
void schedule(void) {
    if (task_count <= 1) {
        return;
    }
    
    // Find next ready task
    int next = (current_task + 1) % task_count;
    while (tasks[next].state != TASK_READY && next != current_task) {
        next = (next + 1) % task_count;
    }
    
    if (next != current_task) {
        // Switch context
        task_switch(&tasks[current_task], &tasks[next]);
        current_task = next;
    }
}

// Simple kmalloc (bump allocator for now)
void *kmalloc(size_t size) {
    static uint32_t heap_ptr = 0x200000; // Start at 2MB
    static uint32_t heap_end = 0x400000; // End at 4MB
    
    if (heap_ptr + size > heap_end) {
        return NULL;
    }
    
    void *ptr = (void *)heap_ptr;
    heap_ptr += size;
    return ptr;
}

// Task switch (assembly)
extern void task_switch(task_t *from, task_t *to);
