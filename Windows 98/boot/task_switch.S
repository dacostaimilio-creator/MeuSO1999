# Task switch function
.global task_switch
.type task_switch, @function
task_switch:
    # Get parameters
    mov 4(%esp), %eax  # from task
    mov 8(%esp), %ebx  # to task
    
    # Save current context
    push %edi
    push %esi
    push %ebp
    push %esp
    push %ebx
    push %edx
    push %ecx
    push %eax
    pushf
    
    # Save ESP to from->context->esp
    mov %esp, %ecx
    mov 16(%eax), %eax  # from->context
    mov %ecx, 8(%eax)   # context->esp
    
    # Load to context
    mov 16(%ebx), %ebx  # to->context
    mov 8(%ebx), %esp   # context->esp
    
    # Restore context
    popf
    pop %eax
    pop %ecx
    pop %edx
    pop %ebx
    pop %esp
    pop %ebp
    pop %esi
    pop %edi
    
    ret

.size task_switch, . - task_switch
