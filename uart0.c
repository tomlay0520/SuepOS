#include "type.h"
#include "hardware_conf.h"

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
	* REGITERS LIST:
	* IER = 0  ISR = 1  LCR = 0
	* MCR = 0  LSR = 60 HEX
	* MSR = BITS 0-3 = 0, BITS 4-7 = inputs
	* FCR = 0  TX = High OP1 = High
	* OP2 = High  RTS = High DTR = High
	* RXRDY = High TXRDY = Low INT = Low
 */
#define LSR_RX_READY (1 << 0)
#define LSR_TX_IDLE  (1 << 5)

#define uart_read_reg(reg) (*(UART_REG(reg)))
#define uart_write_reg(reg, v) (*(UART_REG(reg)) = (v))

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
