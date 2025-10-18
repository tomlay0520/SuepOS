#include "type.h"
#include "hardware_conf.h"
#include <stdarg.h>

/*
 * The UART control registers are memory-mapped at address UART0. 
 * This macro returns the address of one of the registers.
 */
#define UART_REG(reg) ((volatile uint8_t *)(UART0 + reg))

/* UART0 registers (offsets from UART0) */
#define RHR 0	// Receive Holding Register (read mode)
#define THR 0	// Transmit Holding Register (write mode)
#define DLL 0	// LSB of Divisor Latch (write mode)
#define IER 1	// Interrupt Enable Register (write mode)
#define DLM 1	// MSB of Divisor Latch (write mode)
#define FCR 2	// FIFO Control Register (write mode)
#define ISR 2	// Interrupt Status Register (read mode)
#define LCR 3	// Line Control Register
#define MCR 4	// Modem Control Register
#define LSR 5	// Line Status Register
#define MSR 6	// Modem Status Register
#define SPR 7	// ScratchPad Register

/*  THE FOLLOWING LIST THE DEFAULT VALUES OF THE UART0
	DO NOT CHANGE UNLESS YOU KNOW WHAT YOU ARE DOING!!!
	* REGISTERS LIST:
	* IER = 0  ISR = 1  LCR = 0
	* MCR = 0  LSR = 60 HEX
	* MSR = BITS 0-3 = 0, BITS 4-7 = inputs
	* FCR = 0  TX = High OP1 = High
	* OP2 = High  RTS = High DTR = High
	* RXRDY = High TXRDY = Low INT = Low
 */
#define LSR_RX_READY (1 << 0) // receive data ready.
#define LSR_TX_IDLE  (1 << 5) // keep sending reg leisure.

#define uart_read_reg(reg) (*(UART_REG(reg))) // macro read regs.
#define uart_write_reg(reg, v) (*(UART_REG(reg)) = (v)) // macro write regs.

void uart0_init()
{
	/* disable interrupts. */
	uart_write_reg(IER, 0x00);
	uint8_t lcr = uart_read_reg(LCR);
	uart_write_reg(LCR, lcr | (1 << 7));
	uart_write_reg(DLL, 0x03);
	uart_write_reg(DLM, 0x00);
	lcr = 0;
	uart_write_reg(LCR, lcr | (3 << 0));
}

int uart0_put_char(char ch)
{
	while ((uart_read_reg(LSR) & LSR_TX_IDLE) == 0);
	return uart_write_reg(THR, ch);
}

void uart0_put_string(char *s)
{
	while (*s) {
		uart0_put_char(*s++);
	}
}

// SPINNING GET
char uart0_get_char(void)
{
    while((uart_read_reg(LSR) & LSR_RX_READY) == 0);
    return uart_read_reg(RHR);
}


static void uart0_put_number(uint32_t num, int base) {
    char buffer[32];
    char *ptr = buffer;
    
    if (num == 0) {
        uart0_put_char('0');
        return;
    }
    
    while (num > 0) {
        int digit = num % base;
        *ptr++ = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
        num /= base;
    }
    
    while (ptr > buffer) {
        uart0_put_char(*--ptr);
    }
}
static void uart0_put_string_internal(const char *s) {
    while (*s) {
        uart0_put_char(*s++);
    }
}

void mini_printf(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    while (*fmt) {
        if (*fmt == '%') {
            fmt++;
            switch (*fmt) {
                case 's': {
                    const char *str = va_arg(args, const char *);
                    uart0_put_string_internal(str);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int);
                    uart0_put_char(c);
                    break;
                }
                case 'd': {
                    int num = va_arg(args, int);
                    if (num < 0) {
                        uart0_put_char('-');
                        num = -num;
                    }
                    uart0_put_number((uint32_t)num, 10);
                    break;
                }
                case 'u': {
                    uint32_t num = va_arg(args, uint32_t);
                    uart0_put_number(num, 10);
                    break;
                }
                case 'x': {
                    uint32_t num = va_arg(args, uint32_t);
                    uart0_put_string_internal("0x");
                    uart0_put_number(num, 16);
                    break;
                }
                case 'p': {
                    void *p = va_arg(args, void *);
                    uart0_put_string_internal("0x");
                    uart0_put_number((ptr)p, 16); 
                    break;
                }
                case '%': {
                    uart0_put_char('%');
                    break;
                }
                default: {
                    uart0_put_char('%');
                    uart0_put_char(*fmt);
                    break;
                }
            }
        } else {
            uart0_put_char(*fmt);
        }
        fmt++;
    }
    
    va_end(args);
}

void display_welcome()
{                                          
    mini_printf("   _|_|_|  _|    _|  _|_|_|_|  _|_|_|           _|_|      _|_|_|  \n");
    mini_printf(" _|        _|    _|  _|        _|    _|       _|    _|  _|        \n");
    mini_printf("   _|_|    _|    _|  _|_|_|    _|_|_|         _|    _|    _|_|    \n");
    mini_printf("       _|  _|    _|  _|        _|             _|    _|        _|  \n");
    mini_printf(" _|_|_|      _|_|    _|_|_|_|  _|               _|_|    _|_|_|    \n");          
}

int strcmp(const char *s1, const char *s2)
{
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char *)s1 - *(unsigned char *)s2;
}

int strlen(const char *s)
{
    int len = 0;
    while (*s++) len++;
    return len;
}

void readline(char *buffer, int max_length)
{
    int i = 0;
    char c;
    
    while (i < max_length - 1) {
        c = uart0_get_char();
        
        if (c == '\b' || c == 127) {
            if (i > 0) {
                i--;
                uart0_put_char('\b');
                uart0_put_char(' ');
                uart0_put_char('\b');
            }
            continue;
        }
        if (c == '\r' || c == '\n') {
            uart0_put_char('\r');
            uart0_put_char('\n');
            buffer[i] = '\0';
            return;
        }
    
        uart0_put_char(c);
        buffer[i++] = c;
    }
    
    buffer[max_length - 1] = '\0';
}

