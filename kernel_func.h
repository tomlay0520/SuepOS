#pragma once
#include "type.h"
#include "hardware_conf.h"
#include <stddef.h>



// UART0 functions
extern void uart0_init(void);
extern void uart0_put_char(char ch);
extern void uart0_put_string(char *s);

// memory management functions



