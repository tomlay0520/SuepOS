# SYSCALL = y
USE_LINKER_SCRIPT = true

SRCS_ASM = \
	start.S \

SRCS_C = \
	kernel.c \
	uart0.c \

include qemu_gcc.mk