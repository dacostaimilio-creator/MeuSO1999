#ifndef MULTITASK_H
#define MULTITASK_H

#include <stdint.h>
#include <stddef.h>

// Task states
typedef enum {
    TASK_RUNNING,
    TASK_READY,
    TASK_BLOCKED,
    TASK_TERMINATED
} task_state_t;

// Task context
typedef struct {
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t eip, eflags;
} task_context_t;

// Task structure
typedef struct task {
    int id;
    task_state_t state;
    uint32_t stack;
    uint32_t stack_size;
    task_context_t *context;
} task_t;

void multitask_init(void);
int task_create(void (*entry)(void), uint32_t stack_size);
void schedule(void);
void *kmalloc(size_t size);
extern void task_switch(task_t *from, task_t *to);

#endif
