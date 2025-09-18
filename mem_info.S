#ifdef __MEMORY_S__

# it is the GAS syntax
# it is used to define the memory layout of the kernel
.section .rodata

# Memory Layout:
# 0x80000000 +-----------------+
#            |     .text       | TEXT SEGMENT
#            +-----------------+
#            |    .rodata      | READ-ONLY DATA SEGMENT
#            +-----------------+
#            |     .data       | DATA SEGMENT
#            +-----------------+
#            |      .bss       | BSS SEGMENT AUTO-INITIALIZED TO ZERO
#            +-----------------+
#            |      heap       | DYNAMIC MEMORY ALLOCATION
#            |                 | 
#            +-----------------+
#            |  available RAM  | FREE FOR OTHER USES
# 0x84000000 +-----------------+  (0x80000000 + 64MB)


.gloabal HEAP_ENTRY # it means the start address of the heap, 
                    # which will be defined in the linker script.
HEAP_ENTRY: .word  _heap_entry # it is a label, 
                               # which can be used to refer to the address of the heap start.

.global HEAP_END
HEAP_END: .word  _heap_end

.global TEXT_ENTRY
TEXT_ENTRY: .word  _text_entry

.global TEXT_END
TEXT_END: .word  _text_end

.global DATA_ENTRY
DATA_ENTRY: .word  _data_entry

.global DATA_END
DATA_END: .word  _data_end

.global BSS_ENTRY
BSS_ENTRY: .word  _bss_entry

.global BSS_END
BSS_END: .word  _bss_end

.global RODATA_ENTRY
RODATA_ENTRY: .word  _rodata_entry

.global RODATA_END
RODATA_END: .word  _rodata_end


#endif