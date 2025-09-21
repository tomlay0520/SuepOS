#pragma once
#include "type.h"
#include "hardware_conf.h"
#include <stddef.h> 




// UART0 functions
extern void uart0_init(void);
extern void uart0_put_char(char ch);
extern void uart0_put_string(char *s);
extern void mini_printf(const char *fmt, ...);
extern void display_welcome();
extern char uart0_get_char(void);
extern void readline(char *buffer, int max_length);

// memory management functions
extern void init_page_allocator();
extern void *page_alloc(int npages);
extern void page_free(void *p);
extern void page_test();
extern void page_stats();


// scheduler (process aka HART management)
extern int CREATE_A_PROCESS();
extern void delay(int count);
extern void test_task01(void);
extern void test_task02(void);
extern void test_task03(void);
extern void process_give_up(void);
extern void shell(void);




// process management functions
extern void scheduler_init(void);
extern void scheduler(void);

typedef struct GPRegister_context
{
    reg ra; reg sp;
	reg gp; reg tp;
	reg t0; reg t1;
	reg t2; reg s0;
	reg s1; reg a0;
	reg a1; reg a2;
	reg a3; reg a4;
	reg a5; reg a6;
	reg a7; reg s2;
	reg s3; reg s4;
	reg s5; reg s6;
	reg s7; reg s8;
	reg s9; reg s10;
	reg s11; reg t3;
	reg t4; reg t5;
	reg t6;
} CONTEXT;



