#ifndef __HARDWARE_CONF_H__
#define __HARDWARE_CONF_H__  


#define MAX_CPU 8



// memory layout:
//     0x00000000 --> +-----------+
//                    |   BIOS    |
//     0x00001000 --> +-----------+
//                    | boot code |
//     0x02000000 --> +-----------+
//                    |   CLINT   |
//     0x0C000000 --> +-----------+
//                    |   PLIC    |
//     0x10000000 --> +-----------+
//                    |   UART0   |
//     0x10001000 --> +-----------+
//                    |  VIRTIO0  |
//     0x80000000 --> +-----------+
//                    |           |
//                    |  KERNEL   |
//                    |           |
//                    +-----------+

// Memory-mapped I/O addresses for the UART:
#define UART0 0x10000000L
// Main memory size is as follows:
#define MAIN_MEMORY 64 * 1024 * 1024  // 64 MB of main memory

#endif