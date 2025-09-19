#include "kernel_func.h"
#include "mem_info.h"

// Initialize page flags to 0 (indicating free and not last)
static inline void page_clear(Page* page) {
    page->flags = 0;
}

static inline int page_is_available(Page* page) {
    return (page->flags & PAGE_IS_USED) == 0;
}

static inline void page_set_flag(Page* page, uint8_t flag) {
    page->flags |= flag;
}

static inline int page_is_last(Page* page) {
    return (page->flags & PAGE_IS_LAST) != 0;
}

static inline ptr align_page(ptr addr) {
    return (addr + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);
}

// initialize the page allocator
void init_page_allocator() {
    mini_printf("Initializing page allocator...\n");
    
    ptr heap_start_aligned = align_page((ptr)HEAP_ENTRY);
    uint32_t available_heap_size = HEAP_END - heap_start_aligned;
    
    mini_printf("Heap start: %x\n", HEAP_ENTRY);
    mini_printf("Heap end: %x\n", HEAP_END);
    mini_printf("Available heap size: %d bytes\n", available_heap_size);




    // Calculate the size needed for page descriptors:
    uint32_t page_descriptor_size = (available_heap_size / PAGE_SIZE) * sizeof(struct Page);
    uint32_t num_reserved_pages = (page_descriptor_size + PAGE_SIZE - 1) / PAGE_SIZE;
    mini_printf("Reserving %d pages for page descriptors (%d bytes)\n", num_reserved_pages, page_descriptor_size);
    
    // Calculate the number of pages available for allocation after reserving space for page descriptors
    ALLOCATE_PAGES = (available_heap_size - num_reserved_pages * PAGE_SIZE) / PAGE_SIZE;
    mini_printf("Total available pages for allocation: %d\n", ALLOCATE_PAGES);

    // Initialize ALL page descriptors (including those used for the descriptors themselves)
    uint32_t total_pages = available_heap_size / PAGE_SIZE;
    Page* pages_array = (Page*)heap_start_aligned;

    for (uint32_t i = 0; i < total_pages; i++) {
        page_clear(&pages_array[i]);
        
        // Mark pages used for page descriptors as used (since we don't have PAGE_RESERVED)
        if (i < num_reserved_pages) {
            pages_array[i].flags |= PAGE_IS_USED;
        }
    }

    ALLOCATE_START = heap_start_aligned + num_reserved_pages * PAGE_SIZE;
    ALLOCATE_END = ALLOCATE_START + ALLOCATE_PAGES * PAGE_SIZE;
    
    mini_printf("Debug - TEXT_ENTRY: %p\n", TEXT_ENTRY);
    mini_printf("Debug - TEXT_END: %p\n", TEXT_END);
    mini_printf("Debug - HEAP_ENTRY: %p\n", HEAP_ENTRY);
    mini_printf("Debug - HEAP_END: %p\n", HEAP_END);

    mini_printf("TEXT:    0x%x -> 0x%x\n", TEXT_ENTRY, (uint32_t)TEXT_END);
    mini_printf("RODATA:  0x%x -> 0x%x\n", RODATA_ENTRY, (uint32_t)RODATA_END);
    mini_printf("DATA:    0x%x -> 0x%x\n", (uint32_t)DATA_ENTRY, (uint32_t)DATA_END);
    mini_printf("BSS:     0x%x -> 0x%x\n", (uint32_t)BSS_ENTRY, (uint32_t)BSS_END);
    mini_printf("HEAP:    0x%x -> 0x%x\n", (uint32_t)HEAP_ENTRY, (uint32_t)HEAP_END);


    mini_printf("Page allocator initialized.\n");
}

/*a simple method to allocate pages*/
void *page_alloc(int npages)
{
    if (npages <= 0 || npages > ALLOCATE_PAGES) {
        return NULL;
    }
    
    struct Page *page = (struct Page *)HEAP_ENTRY;
    int consecutive_free = 0;
    int start_index = -1;
    
    /* find the continuous & available pages */
    for (int i = 0; i < ALLOCATE_PAGES; i++) {
        if (page_is_available(&page[i])) {
            if (consecutive_free == 0) {
                start_index = i;
            }
            consecutive_free++;
            
            if (consecutive_free >= npages) {
                for (int j = start_index; j < start_index + npages; j++) {
                    page_set_flag(&page[j], PAGE_IS_USED);
                    if (j == start_index + npages - 1) {
                        page_set_flag(&page[j], PAGE_IS_LAST);
                    }
                }
                return (void *)(ALLOCATE_START + start_index * PAGE_SIZE);
            }
        } else {
            consecutive_free = 0;
            start_index = -1;
        }
    }
    
    return NULL; /* no enough pages */
}

// free the pages starting from pointer p
void page_free(void *p)
{
    if (!p || (ptr)p < ALLOCATE_START || (ptr)p >= ALLOCATE_END) {
        return;
    }
    
    int start_index = ((ptr)p - ALLOCATE_START) / PAGE_SIZE;
    struct Page *page = (struct Page *)HEAP_ENTRY;
    
    /* free the continuous pages */
    int i = start_index;
    while (i < ALLOCATE_PAGES && !page_is_available(&page[i])) {
        page_clear(&page[i]);
        if (page_is_last(&page[i])) {
            break;
        }
        i++;
    }
}

// simple test function for page allocation
void page_test()
{
    mini_printf("=== Page Allocation Test ===\n");
    
    void *p1 = page_alloc(2); // parameter 2 means allocate 2 pages
    mini_printf("Allocated 2 pages at: %p\n", p1);
    
    void *p2 = page_alloc(3);
    mini_printf("Allocated 3 pages at: %p\n", p2);
    
    void *p3 = page_alloc(1);
    mini_printf("Allocated 1 page at: %p\n", p3);
    
    mini_printf("Freeing p2...\n");
    page_free(p2);
    
    void *p4 = page_alloc(4);
    mini_printf("Allocated 4 pages at: %p\n", p4);
    
    mini_printf("=== Test Completed ===\n");
    page_free(p1);
    page_free(p3);
    page_free(p4);
    
}
