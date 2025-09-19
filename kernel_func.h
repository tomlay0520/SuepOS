#pragma once
#include "type.h"
#include "hardware_conf.h"
#include <stddef.h> 


// UART0 functions
extern void uart0_init(void);
extern void uart0_put_char(char ch);
extern void uart0_put_string(char *s);
extern void mini_printf(const char *fmt, ...);

// memory management functions
extern void init_page_allocator();
extern void *page_alloc(int npages);
extern void page_free(void *p);
void page_test();
void page_stats();


