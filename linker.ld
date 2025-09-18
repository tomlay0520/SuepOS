OUTPUT_ARCH("riscv")
ENTRY(_start)

MEMORY {
    ram (wxa!ri) : ORIGIN = 0x80000000, LENGTH = DEFINED(__MEM_SIZE__) ? __MEM_SIZE__ : 64M
}

SECTIONS
{
    /* text seg */
    .text : {
        PROVIDE(_text_entry = .);    /* the start address of text seg */
        *(.text .text.*)
        PROVIDE(_text_end = .);      /* the end address of text seg */
    } >ram

    /* read-only seg */
    .rodata : {
        PROVIDE(_rodata_entry = .);  /* the start address of read-only seg */
        *(.rodata .rodata.*)
        PROVIDE(_rodata_end = .);    /* the end address of read-only seg */
    } >ram

    /* data seg */
    .data : {
        . = ALIGN(4096);
        PROVIDE(_data_entry = .);    /* the start address of data seg */
        *(.sdata .sdata.*)
        *(.data .data.*)
        PROVIDE(_data_end = .);      /* the end address of data seg */
    } >ram

    /* BSS seg） */
    .bss : {
        PROVIDE(_bss_entry = .);     /* the start address of bss seg */
        *(.sbss .sbss.*)
        *(.bss .bss.*)
        *(COMMON)
        PROVIDE(_bss_end = .);       /* the start address of bss seg */
    } >ram

    /* heap def */
    .heap : {
        PROVIDE(_heap_entry = .);    /* the start address of heap */
        . = . + 0x10000;             /* 64KB heap size */
        PROVIDE(_heap_end = .);      /* the end address of heap */
    } >ram

    PROVIDE(_memory_start = ORIGIN(ram));
    PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));

    PROVIDE(_heap_size = _heap_end - _heap_entry);
}