# RISC-V Context Switching Assembly
# Reference: RISC-V Privileged Specification v1.12

# Save all General-Purpose Registers (x1-x31) to context structure
# struct context {
#     uint32_t ra;    // x1   - Return address
#     uint32_t sp;    // x2   - Stack pointer  
#     uint32_t gp;    // x3   - Global pointer
#     uint32_t tp;    // x4   - Thread pointer
#     uint32_t t0;    // x5   - Temporary
#     // ... all registers through x31
# };
#
# @param base: register containing pointer to context structure
.macro reg_save base
    sw ra, 0(\base)      # x1 - Return address
    sw sp, 4(\base)      # x2 - Stack pointer
    sw gp, 8(\base)      # x3 - Global pointer
    sw tp, 12(\base)     # x4 - Thread pointer
    sw t0, 16(\base)     # x5 - Temporary
    sw t1, 20(\base)     # x6 - Temporary
    sw t2, 24(\base)     # x7 - Temporary
    sw s0, 28(\base)     # x8 - Saved register/frame pointer
    sw s1, 32(\base)     # x9 - Saved register
    sw a0, 36(\base)     # x10 - Function argument/return value
    sw a1, 40(\base)     # x11 - Function argument/return value
    sw a2, 44(\base)     # x12 - Function argument
    sw a3, 48(\base)     # x13 - Function argument
    sw a4, 52(\base)     # x14 - Function argument
    sw a5, 56(\base)     # x15 - Function argument
    sw a6, 60(\base)     # x16 - Function argument
    sw a7, 64(\base)     # x17 - Function argument
    sw s2, 68(\base)     # x18 - Saved register
    sw s3, 72(\base)     # x19 - Saved register
    sw s4, 76(\base)     # x20 - Saved register
    sw s5, 80(\base)     # x21 - Saved register
    sw s6, 84(\base)     # x22 - Saved register
    sw s7, 88(\base)     # x23 - Saved register
    sw s8, 92(\base)     # x24 - Saved register
    sw s9, 96(\base)     # x25 - Saved register
    sw s10, 100(\base)   # x26 - Saved register
    sw s11, 104(\base)   # x27 - Saved register
    sw t3, 108(\base)    # x28 - Temporary
    sw t4, 112(\base)    # x29 - Temporary
    sw t5, 116(\base)    # x30 - Temporary
    # Note: t6 (x31) is not saved here because it's used as base pointer
    # t6 must be saved separately after reg_save completes
.endm

# Restore all General-Purpose Registers from context structure
# @param base: register containing pointer to context structure
.macro reg_restore base
    lw ra, 0(\base)      # x1 - Return address
    lw sp, 4(\base)      # x2 - Stack pointer
    lw gp, 8(\base)      # x3 - Global pointer
    lw tp, 12(\base)     # x4 - Thread pointer
    lw t0, 16(\base)     # x5 - Temporary
    lw t1, 20(\base)     # x6 - Temporary
    lw t2, 24(\base)     # x7 - Temporary
    lw s0, 28(\base)     # x8 - Saved register/frame pointer
    lw s1, 32(\base)     # x9 - Saved register
    lw a0, 36(\base)     # x10 - Function argument/return value
    lw a1, 40(\base)     # x11 - Function argument/return value
    lw a2, 44(\base)     # x12 - Function argument
    lw a3, 48(\base)     # x13 - Function argument
    lw a4, 52(\base)     # x14 - Function argument
    lw a5, 56(\base)     # x15 - Function argument
    lw a6, 60(\base)     # x16 - Function argument
    lw a7, 64(\base)     # x17 - Function argument
    lw s2, 68(\base)     # x18 - Saved register
    lw s3, 72(\base)     # x19 - Saved register
    lw s4, 76(\base)     # x20 - Saved register
    lw s5, 80(\base)     # x21 - Saved register
    lw s6, 84(\base)     # x22 - Saved register
    lw s7, 88(\base)     # x23 - Saved register
    lw s8, 92(\base)     # x24 - Saved register
    lw s9, 96(\base)     # x25 - Saved register
    lw s10, 100(\base)   # x26 - Saved register
    lw s11, 104(\base)   # x27 - Saved register
    lw t3, 108(\base)    # x28 - Temporary
    lw t4, 112(\base)    # x29 - Temporary
    lw t5, 116(\base)    # x30 - Temporary
    lw t6, 120(\base)    # x31 - Temporary (also used as base)
.endm

.text

# void switch_to(struct context *next);
#
# Context switching function that saves current task context and
# restores the next task's context.
#
# @param a0: pointer to context structure of next task
#
# Design Notes:
# - Uses mscratch CSR to store pointer to previous task's context
# - t6 is used as base pointer because it's the last register (x31)
# - Follows standard RISC-V calling convention (a0 for first argument)
.globl switch_to_context
.align 4
switch_to_context:
    # Swap t6 with mscratch value (which holds previous task's context pointer)
    csrrw t6, mscratch, t6
    
    # If previous task context is NULL (first switch), skip saving
    beqz t6, 1f
    
    # Save all registers of previous task using t6 as base pointer
    reg_save t6
    
    # Save the original t6 value that was swapped into mscratch
    mv t5, t6            # t5 now points to current task's context
    csrr t6, mscratch    # Retrieve original t6 value
    sw t6, 120(t5)       # Save t6 in the context structure

1:
    # Update mscratch to point to next task's context
    csrw mscratch, a0
    
    # Restore all registers for the next task
    mv t6, a0            # Load next task's context pointer into t6
    reg_restore t6
    
    # Return to the restored return address (ra)
    # This will jump to the next task's execution point
    ret

.end