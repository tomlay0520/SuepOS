# SYSCALL = y
USE_LINKER_SCRIPT = true

SRCS_ASM = \
	start.S \
	mem_info.S\

SRCS_C = \
	kernel.c \
	uart0.c \
	page_alloc.c \


include qemu_gcc.mk