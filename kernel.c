#include "kernel_func.h"


/*
BEFORE STARTING KERNEL, 
you should know that:
1. memory allocation:
    - kernel is loaded at 0x80000000
    - user program is loaded at 0x80400000

2. memory-mapped I/O addresses:
    - CLINT is at 0x02000000
    - PLIC is at 0x0C000000
    - UART0 is at 0x10000000
    - VIRTIO0 is at 0x10001000

3. memory layout:

    0x00000000 --> +-----------+
                   |   BIOS    |
    0x00001000 --> +-----------+
                   | boot code |
    0x02000000 --> +-----------+
                   |   CLINT   |
    0x0C000000 --> +-----------+
                   |   PLIC    |
    0x10000000 --> +-----------+
                   |   UART0   |
    0x10001000 --> +-----------+
                   |  VIRTIO0  |
    0x80000000 --> +-----------+
                   |           |
                   |  KERNEL   |
                   |           |
    0x84000000 --> +-----------+
*/


/*
    # Main Memoryof kernel Layout:
    # 0x80000000 +-----------------+
    #            |     .text       | TEXT SEGMENT
    #            +-----------------+
    #            |    .rodata      | READ-ONLY DATA SEGMENT
    #            +-----------------+
    #            |     .data       | DATA SEGMENT
    #            +-----------------+
    #            |      .bss       | BSS SEGMENT AUTO-INITIALIZED TO ZERO
    #            +-----------------+
    #            |      heap       | DYNAMIC MEMORY ALLOCATION
    #            |                 | 
    #            +-----------------+
    #            |  available RAM  | FREE FOR OTHER USES
    # 0x84000000 +-----------------+  (0x80000000 + 64MB)
*/

void start_kernel(void)
{
    // KERNEL WILL START FROM HERE
    // EVERY MODULE YOU WRITE MUST BE INITIALIZED HERE
    // FOR EXAMPLE, TO INITIALIZE UART0, CALL THE FUNCTION uart0_init()
    uart0_init();
    mini_printf("Kernel is starting...\n");
    display_welcome();

    init_page_allocator();
    page_test();
    scheduler_init();
    CREATE_A_PROCESS(test_task01);
    CREATE_A_PROCESS(test_task02);
    CREATE_A_PROCESS(test_task03);
    mini_printf("here?\n");
    scheduler();
    mini_printf("GUESS WHAT NOBODY CARES!\n");
    while(1); // stop here
}