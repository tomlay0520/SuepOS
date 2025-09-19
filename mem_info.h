#pragma once
#include "kernel_func.h"

#define PAGE_SIZE 4096
#define PAGE_IS_USED (uint8_t)(1 << 0) // 00000001 in binary means the page is used
#define PAGE_IS_LAST (uint8_t)(1 << 1) // 00000010 in binary means the page is the last page in a contiguous allocation
typedef struct Page {
    uint8_t flags;
} Page;

extern ptr TEXT_ENTRY;
extern ptr TEXT_END;
extern ptr DATA_ENTRY;
extern ptr DATA_END;
extern ptr RODATA_ENTRY;
extern ptr RODATA_END;
extern ptr BSS_ENTRY;
extern ptr BSS_END;
extern ptr HEAP_ENTRY;
extern ptr HEAP_END;

// extern uint32_t TEXT_ENTRY;
// extern uint32_t TEXT_END;
// extern uint32_t RODATA_ENTRY;
// extern uint32_t RODATA_END;
// extern uint32_t DATA_ENTRY;
// extern uint32_t DATA_END;
// extern uint32_t BSS_ENTRY;
// extern uint32_t BSS_END;
// extern uint32_t HEAP_ENTRY;
// extern uint32_t HEAP_END;


static ptr ALLOCATE_START = 0;
static ptr ALLOCATE_END = 0;
static uint32_t ALLOCATE_PAGES = 0;