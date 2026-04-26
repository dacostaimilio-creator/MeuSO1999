# IDT load function
.global idt_load
.type idt_load, @function
idt_load:
    mov 4(%esp), %eax  # Get pointer to IDT
    lidt (%eax)
    ret

.size idt_load, . - idt_load
