#include "kernel_func.h"
#define STACK_LENGTH 1024

static void my_mscratch(reg re)
{
    asm volatile("csrw mscratch, %0" : : "r" (re));
}

void user_first_process(void)
{
    mini_printf("hello from user mode!\n");
    while(1); // stop here!
}

uint8_t process_stack[STACK_LENGTH];
CONTEXT context;
extern void switch_to_context(CONTEXT* next); // you can check it in switch.S

void scheduler_init(void)
{
    my_mscratch(0);

    context.sp = (reg) &process_stack[STACK_LENGTH - 1];
    context.ra = (reg) user_first_process;
}

void schedule()
{
    CONTEXT* next = &context;
    switch_to_context(next);
}
