#include "kernel_func.h"
#define STACK_LENGTH 1024
#define MAX_PROCESS 5


extern void shell();

static void my_mscratch(reg re)
{
    asm volatile("csrw mscratch, %0" : : "r" (re));
}

void user_first_process(void)
{
    mini_printf("hello from user mode!\n");
    shell();
    while(1); // stop here!
}

uint8_t process_stack[MAX_PROCESS][STACK_LENGTH];
CONTEXT context_array[MAX_PROCESS];
extern void switch_to_context(CONTEXT* next); // you can check it in switch.S
static int stack_pointer = 0;
static int base_pointer = -1;

int CREATE_A_PROCESS(void (*s)(void))
{
    if (stack_pointer < MAX_PROCESS){
        // mini_printf("stack pointer: %x\n", &stack_pointer);
        // mini_printf("base  pointer: %x\n", &base_pointer);
        context_array[stack_pointer].sp = (reg) &process_stack[stack_pointer][STACK_LENGTH - 1];
        context_array[stack_pointer].ra = (reg) s;
        stack_pointer += 1;
        return 1; // 1 means success.
    } else return 0;
    // 0 means failed.
}

void scheduler_init(void)
{
    my_mscratch(0);
}

void process_give_up(void)
{
    // give up the current process, and let others ocupy HART
    scheduler();
}


void scheduler()
{
    if (stack_pointer <= 0){        
        mini_printf("PANIC: NO PROCESS!!!\n");
        return;   
    }

    base_pointer = (base_pointer + 1) % stack_pointer;
    CONTEXT* next_ctx = &(context_array[base_pointer]);
    switch_to_context(next_ctx);

}

void delay(int count)
{
    const int iterations = 50000;
    while (count--) {
        for (volatile int i = 0; i < iterations; i++);
    }
}