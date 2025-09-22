#include "kernel_func.h"
#include "mem_info.h"
#define STACK_LENGTH 1024    
#define MAX_PROCESS 5        

typedef enum {
    PROC_READY,       // 就绪
    PROC_RUNNING,     // 运行中
    PROC_BLOCKED,     // 阻塞
    PROC_FINISHED     // 已完成
} ProcState;

typedef struct PCB {
    void (*entry)(void);       // 入口函数指针
    CONTEXT context;           // 上下文
    uint8_t* stack;            // 栈指针
    ProcState state;           // 状态
    int pid;                   // 进程ID
    struct PCB* next;          // 下一个PCB
} PCB;

// 进程队列
typedef struct {
    PCB* head;                 // 队首
    PCB* tail;                 // 队尾
    int count;                 // 计数
} ProcQueue;

extern void shell();
static void my_mscratch(reg re);
void user_first_process(void);
int CREATE_A_PROCESS(void (*s)(void));
void scheduler_init(void);
void process_give_up(void);
void scheduler(void);
void delay(int count);
void process_exit(void);

static void my_mscratch(reg re) {
    asm volatile("csrw mscratch, %0" : : "r" (re));
}

void user_first_process(void) {
    mini_printf("hello from user mode!\n");
    shell();
    while(1);
}

static ProcQueue pcb_queue;
static PCB pcb_pool[MAX_PROCESS];
static int next_pid = 1;
static PCB* current_running = NULL;

// 初始化队列
static void init_queue() {
    pcb_queue.head = NULL;
    pcb_queue.tail = NULL;
    pcb_queue.count = 0;
}

// 入队
static void enqueue(PCB* pcb) {
    if (!pcb_queue.head) {
        pcb_queue.head = pcb;
        pcb_queue.tail = pcb;
    } else {
        pcb_queue.tail->next = pcb;
        pcb_queue.tail = pcb;
    }
    pcb->next = NULL;
    pcb_queue.count++;
}

// 出队
static PCB* dequeue() {
    if (!pcb_queue.head) return NULL;
    
    PCB* front = pcb_queue.head;
    pcb_queue.head = front->next;
    
    if (!pcb_queue.head) {
        pcb_queue.tail = NULL;
    }
    
    pcb_queue.count--;
    front->next = NULL;
    return front;
}
static void remove_from_queue(PCB* pcb) {
    if (!pcb_queue.head || !pcb) return;
    
    PCB* prev = NULL;
    PCB* curr = pcb_queue.head;
    
    while (curr) {
        if (curr == pcb) {
            if (prev) {
                prev->next = curr->next;
            } else {
                pcb_queue.head = curr->next;
            }
            
            if (pcb_queue.tail == curr) {
                pcb_queue.tail = prev;
            }
            
            pcb_queue.count--;
            curr->next = NULL;
            return;
        }
        prev = curr;
        curr = curr->next;
    }
}

int CREATE_A_PROCESS(void (*s)(void)) {
    static int next_pcb_index = 0;  // static var to trace the next index of pcb
    void* stack_page = page_alloc(1);
    if (!stack_page) {
        mini_printf("Error: Stack allocation failed\n");
        return 0;
    }

    // 查找可用PCB
    PCB* pcb = NULL;
    for (int i = 0; i < MAX_PROCESS; i++) {
        int index = (next_pcb_index + i) % MAX_PROCESS;
        if (pcb_pool[index].state == PROC_FINISHED || pcb_pool[index].state == 0) {
            pcb = &pcb_pool[index];
            next_pcb_index = (index + 1) % MAX_PROCESS;  // 更新下一个索引
            break;
        }
    }
    
    if (!pcb) {
        mini_printf("Error: No available PCB\n");
        page_free(stack_page);
        return 0;
    }
    pcb->pid = next_pid++;
    pcb->entry = s;
    pcb->stack = stack_page;
    pcb->state = PROC_READY;
    pcb->next = NULL;
    pcb->context.sp = (reg)((uint8_t*)stack_page + PAGE_SIZE) & ~0xF;
    pcb->context.ra = (reg)s;

    mini_printf("Created process %d at PCB[%ld]\n", 
                pcb->pid, pcb - pcb_pool);
    
    enqueue(pcb);
    return 1;
}

static void debug_queue() {
    mini_printf("Queue: ");
    PCB* current = pcb_queue.head;
    while (current) {
        mini_printf("%d(%s) ", current->pid, 
                   current->state == PROC_READY ? "R" : 
                   current->state == PROC_RUNNING ? "X" : "F");
        current = current->next;
    }
    mini_printf("\n");
}

void scheduler_init(void) {
    my_mscratch(0);
    init_queue();
    for (int i = 0; i < MAX_PROCESS; i++) {
        pcb_pool[i].state = PROC_FINISHED;
        pcb_pool[i].stack = NULL;
        pcb_pool[i].next = NULL;
        pcb_pool[i].pid = -1; 
    }
    mini_printf("Scheduler initialized with %d PCBs\n", MAX_PROCESS);
}

void process_give_up(void) {
    if (current_running && current_running->state == PROC_RUNNING) {
        current_running->state = PROC_READY;
        enqueue(current_running);
    }
    scheduler();
}

void scheduler() {
    debug_queue();
    
    PCB* next = NULL;
    while (pcb_queue.count > 0) {
        next = dequeue();
        if (next->state == PROC_READY) {
            break;
        }
        next = NULL;
    }
    
    if (!next) {
        mini_printf("No ready process\n");
        return;
    }

    if (current_running) {
        current_running->state = PROC_READY;
    }
    
    next->state = PROC_RUNNING;
    current_running = next;
    
    switch_to_context(&next->context);
}

void delay(int count) {
    const int iterations = 50000;
    while (count--) {
        for (volatile int i = 0; i < iterations; i++) {
            asm volatile ("nop"); // nop instruction opt
        }
    }
}

void process_exit(void) {
    if (current_running) {
        void* stack_to_free = current_running->stack;
        int pid_to_free = current_running->pid;
        
        current_running->state = PROC_FINISHED;
        remove_from_queue(current_running);
        
        mini_printf("Process %d exiting\n", pid_to_free);
        scheduler();
        
        page_free(stack_to_free);
    }
}