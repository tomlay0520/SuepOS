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

.global _text_start
.global _text_end
.global _rodata_start
.global _rodata_end
.global _data_start
.global _data_end
.global _bss_start
.global _bss_end
.global _heap_start
.global _heap_end

# provide interface for c codes.
.global TEXT_ENTRY
TEXT_ENTRY: .word  _text_start

.global TEXT_END
TEXT_END: .word  _text_end

.global RODATA_ENTRY
RODATA_ENTRY: .word  _rodata_start

.global RODATA_END
RODATA_END: .word  _rodata_end

.global DATA_ENTRY
DATA_ENTRY: .word  _data_start

.global DATA_END
DATA_END: .word  _data_end

.global BSS_ENTRY
BSS_ENTRY: .word  _bss_start

.global BSS_END
BSS_END: .word  _bss_end

.global HEAP_ENTRY
HEAP_ENTRY: .word  _heap_start

.global HEAP_END
HEAP_END: .word  _heap_end

#endif