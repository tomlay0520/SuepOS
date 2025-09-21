#include "kernel_func.h"
#include "mem_info.h"
#define STACK_LENGTH 1024    // Maximum length of the process stack (bytes)
#define MAX_PROCESS 5        // Maximum number of concurrent processes supported

// Enumeration defining possible process states
typedef enum {
    PROC_READY,       
    PROC_RUNNING,     
    PROC_BLOCKED,    
    PROC_FINISHED     
} ProcState;

// Process Control Block (PCB) structure - manages all information for a process
typedef struct PCB {
    void (*entry)(void);       // Pointer to the process's entry function (where execution starts)
    CONTEXT context;           
    uint8_t* stack;            
    ProcState state;       
    int pid;                   // Unique Process ID for identification
    struct PCB* next;          // Pointer to next PCB in the scheduling queue (for linked list)
} PCB;

// Process queue structure - manages a FIFO queue of PCBs for scheduling
typedef struct {
    PCB* head;                 
    PCB* tail;           
    int count;                 
} ProcQueue;

// External declarations for functions and symbols from other files
extern void shell();
static void my_mscratch(reg re);
void user_first_process(void);
int CREATE_A_PROCESS(void (*s)(void));
void scheduler_init(void);
void process_give_up(void);
void scheduler(void);
void delay(int count);

/**
 * Sets the mscratch register (RISC-V specific)
 * mscratch is a machine-mode register often used to store context pointers
 * re Value to write to the mscratch register
 */
static void my_mscratch(reg re) {
    asm volatile("csrw mscratch, %0" : : "r" (re));  // CSRW = Control/Status Register Write
}

/**
 * First user-mode process implementation
 * Prints a message, launches the shell, and enters an infinite loop
 */
void user_first_process(void) {
    mini_printf("hello from user mode!\n");  // Print user mode message
    shell();                                 // Launch the shell interface
    while(1);                                // Halt execution after shell exits
}

static ProcQueue pcb_queue;               // Main queue of processes for scheduling
static PCB pcb_pool[MAX_PROCESS];         // Preallocated pool of PCBs (avoids dynamic allocation)
static int next_pid = 1;                  // Counter for generating unique PIDs
static PCB* current_running = NULL;       // Pointer to the currently executing process

static void init_queue() {
    pcb_queue.head = NULL;    // No head initially
    pcb_queue.tail = NULL;    // No tail initially
    pcb_queue.count = 0;      // Zero processes in queue
}

/**
 * Adds a PCB to the end of the process queue (FIFO order)
 * pcb Pointer to the PCB to add to the queue
 */
static void enqueue(PCB* pcb) {
    if (!pcb_queue.head) {                // If queue is empty
        pcb_queue.head = pcb;             // New PCB becomes both head and tail
        pcb_queue.tail = pcb;
    } else {
        pcb_queue.tail->next = pcb;       // Link current tail to new PCB
        pcb_queue.tail = pcb;             // Update tail to new PCB
    }
    pcb->next = NULL;                     // Ensure new tail has no next pointer
    pcb_queue.count++;                    // Increment process count
}

/**
 * Removes and returns the PCB at the front of the process queue
 * Pointer to the removed PCB, or NULL if queue is empty
 */
static PCB* dequeue() {
    if (!pcb_queue.head) return NULL;     // Return NULL if queue is empty
    
    PCB* front = pcb_queue.head;          // Store pointer to current head
    pcb_queue.head = front->next;         // Update head to next PCB in queue
    
    if (!pcb_queue.head) {                // If queue is now empty
        pcb_queue.tail = NULL;            // Clear tail pointer
    }
    
    pcb_queue.count--;                    // Decrement process count
    front->next = NULL;                   // Detach from queue
    return front;                         // Return the removed PCB
}

int CREATE_A_PROCESS(void (*s)(void)) {
    if (pcb_queue.count >= MAX_PROCESS)   
        return 0;

    void* stack_page = page_alloc(1);
    if (!stack_page) return 0;            // Return failure if stack allocation fails

    // Get a PCB from the preallocated pool
    PCB* pcb = &pcb_pool[pcb_queue.count];
    pcb->pid = next_pid++;                // Assign unique PID
    pcb->entry = s;                       
    pcb->stack = stack_page;             
    pcb->state = PROC_READY;              

    // Initialize context: set stack pointer to top of stack (grows downward on RISC-V)
    pcb->context.sp = (reg)(stack_page + PAGE_SIZE - 1);  
    pcb->context.ra = (reg)s;                             

    mini_printf("Created process %d, stack at: 0x%x\n", pcb->pid, stack_page);
    enqueue(pcb);                         // Add new process to scheduling queue
    return 1;                             // Return success
}

void scheduler_init(void) {
    my_mscratch(0);       // Initialize mscratch register
    init_queue();         // Initialize process queue
}

void process_give_up(void) {
    if (current_running) {
        current_running->state = PROC_READY;  // Mark current process as READY
        enqueue(current_running);             // Add back to scheduling queue
    }
    scheduler();  // Trigger scheduler to select next process
}

void scheduler() {
    if (pcb_queue.count <= 0) {  // Check if no processes are available
        mini_printf("PANIC: NO PROCESS!!!\n");
        return;   
    }

    // Get next process from the front of the queue
    PCB* next = dequeue();
    if (!next) return;

    // Update states: current process (if exists) becomes READY, next becomes RUNNING
    if (current_running) {
        current_running->state = PROC_READY;
    }
    next->state = PROC_RUNNING;
    current_running = next;  // Update current running process

    // Switch CPU context to the next process (implemented in assembly)
    switch_to_context(&next->context);
}

void delay(int count) {
    const int iterations = 50000;  // Inner loop iterations per count
    while (count--) {
        for (volatile int i = 0; i < iterations; i++);  // Volatile prevents optimization
    }
}