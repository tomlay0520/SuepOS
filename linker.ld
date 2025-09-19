/*
    it is the linker script,
    aiming to map the sections of kernel to someplace of RAM.
*/

OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY
{
    ram   (wxa!ri) : ORIGIN = 0x80000000, LENGTH = DEFINED(__MEM_SIZE__) ? __MEM_SIZE__ : 64M
}

SECTIONS
{
    /* text segment */
    .text : {
        PROVIDE(_text_start = .);
        *(.text.init)
        *(.text .text.*)
        PROVIDE(_text_end = .);
    } >ram

    /* read-only segment */
    .rodata : {
        PROVIDE(_rodata_start = .);
        *(.rodata .rodata.*)
        *(.srodata .srodata.*)
        PROVIDE(_rodata_end = .);
    } >ram

    /* data segment */
    .data : {
        PROVIDE(_data_start = .);
        *(.sdata .sdata.*)
        *(.data .data.*)
        PROVIDE(_data_end = .);
    } >ram

    /* BSS segment */
    .bss : {
        PROVIDE(_bss_start = .);
        *(.sbss .sbss.*)
        *(.bss .bss.*)
        *(COMMON)
        PROVIDE(_bss_end = .);
    } >ram

    /* Heap starts right after BSS */
    . = ALIGN(4096);
    PROVIDE(_heap_start = .);
    
    /* Reserve some space for heap */
    . = . + 0x100000; /* 1MB heap */
    PROVIDE(_heap_end = .);

    /* Stack space */
    .stack : ALIGN(16) {
        PROVIDE(_stack_start = .);
        . = . + 0x4000; /* 16KB stack */
        PROVIDE(_stack_end = .);
    } >ram

    /* Memory boundaries */
    PROVIDE(_memory_start = ORIGIN(ram));
    PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));

    /* Heap size calculation */
    PROVIDE(_heap_size = _heap_end - _heap_start);
}