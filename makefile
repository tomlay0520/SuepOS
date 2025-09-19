# SYSCALL = y
USE_LINKER_SCRIPT = true

SRCS_ASM = \
	start.S \

SRCS_C = \
	kernel.c \
	uart0.c \
	page_alloc.c \


include qemu_gcc.mk