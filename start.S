#include "hardware_conf.h"

    # Size of each hart's stack is 1024 bytes
    .equ    STACK_SIZE, 1024

    .global _start

    .text
_start:
    # Park harts with id != 0
    csrr    t0, mhartid        # Read current hart id
    mv      tp, t0             # Keep CPU's hartid in its tp for later usage
    bnez    t0, park           # If we're not on hart 0, park the hart

    # Setup stacks for all harts
    # Calculate stack top address for hart 0
    la      sp, stacks + STACK_SIZE  # Set stack pointer to end of first stack
    
    # If you want to set up stacks for other harts too, you'd do it here
    # but since we're parking them, only hart 0 needs a stack
    
    j       start_kernel       # Hart 0 jump to C code

park:
    wfi                        # Wait for interrupt
    j       park               # Loop indefinitely

    # Stack space allocation
    .balign 16
stacks:
    .skip   STACK_SIZE * MAX_CPU  # Allocate space for all harts stacks

    .end