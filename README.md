# SuepOS 
## Shanghai University of Electric Power Operating System

[English](#suepos-shanghai-university-of-electric-power-os-1) | [中文](#suepos-上海电力大学操作系统)

---

## SuepOS (Shanghai University of Electric Power OS)

A simple, educational operating system for RISC-V architecture, designed and developed for teaching purposes at Shanghai University of Electric Power (SUEP). Its goal is to help students understand the core concepts of operating systems, such as boot process, privilege levels, exception handling, memory management, and preemptive multitasking, through hands-on practice with concise and readable code.

**Architecture**: RISC-V 64 (RV64GC)
**Platform**: QEMU `virt` Machine
**Status**: Under Development (Teaching Use)

### Features

*   **Bootloader**: Uses OpenSBI as the firmware to bring the system to S-mode.
*   **Memory Management**: Implements a simple page-based virtual memory system.
*   **Exception & Interrupt Handling**: Handles exceptions and timer interrupts in S-mode.
*   **Multitasking**: A minimal preemptive scheduler with simple Round-Robin policy.
*   **System Calls**: Basic system call interface (e.g., `sbi_console_putchar` for output).
*   **Teaching Focus**: Code is heavily commented, modular, and designed for clarity over performance.

### Getting Started

#### Prerequisites

*   A RISC-V 64 GCC toolchain (e.g., `riscv64-unknown-elf-gcc`)
*   QEMU system emulator for RISC-V (e.g., `qemu-system-riscv64`)
*   Make
*   by the way, i will give all resources for free :)

#### Build and Run

1.  Clone the repository:
    ```bash
    git clone https://github.com/tomlay0520/SuepOS.git
    cd SuepOS
    ```

2.  Build the kernel:
    ```bash
    make
    ```

3.  Run SuepOS on QEMU:
    ```bash
    make run
    ```
    Press `Ctrl+A` then `X` to exit QEMU.

4.  Clean all your target files:
    ```bash
    make clean
    ```

### Project Structure

```
os/
├── kernel      
│   ├── kernel.o/          
│   ├── os.bin/       
│   ├── os.elf/ 
|   └── start.o/      
├── kernel.c 
├── platform.h
├── qemu_gcc.mk
├── start.S
├── type.h
├── uart0.c
└── Makefile
```

### memory layout (kernel):
```
 Memory Layout:
 0x80000000 +-----------------+
            |     .text       | TEXT SEGMENT
            +-----------------+
            |    .rodata      | READ-ONLY DATA SEGMENT
            +-----------------+
            |     .data       | DATA SEGMENT
            +-----------------+
            |      .bss       | BSS SEGMENT AUTO-INITIALIZED TO ZERO
            +-----------------+
            |      heap       | DYNAMIC MEMORY ALLOCATION
            |                 | 
            +-----------------+
            |  available RAM  | FREE FOR OTHER USES
 0x84000000 +-----------------+  (0x80000000 + 64MB)
```

### Whole Main Memory Layout
```
    0x00000000 --> +-----------+
                   |   BIOS    |
    0x00001000 --> +-----------+
                   | boot code |
    0x02000000 --> +-----------+
                   |   CLINT   |
    0x0C000000 --> +-----------+
                   |   PLIC    |
    0x10000000 --> +-----------+
                   |   UART0   |
    0x10001000 --> +-----------+
                   |  VIRTIO0  |
    0x80000000 --> +-----------+
                   |           |
                   |  KERNEL   |
                   |           |
    0x84000000 --> +-----------+
```


### References & Further Reading

*   **The RISC-V Instruction Set Manual**
    *   Volume I: Unprivileged ISA ([https://riscv.org/technical/specifications/](https://riscv.org/technical/specifications/))
    *   Volume II: Privileged ISA ([https://riscv.org/technical/specifications/](https://riscv.org/technical/specifications/))
*   **RISC-V Supervisor Binary Interface (SBI) Specification**
    *   [https://github.com/riscv-non-isa/riscv-sbi-doc](https://github.com/riscv-non-isa/riscv-sbi-doc)
*   **OpenSBI**: The RISC-V Supervisor Binary Interface (SBI) firmware implementation.
    *   [https://github.com/riscv-software-src/opensbi](https://github.com/riscv-software-src/opensbi)
*   **QEMU RISC-V `virt` Machine Documentation**
    *   `qemu-system-riscv64 -machine virt help`
*   **Teaching OS Inspiration**:
    *   **xv6**: A simple Unix-like teaching OS. ([https://pdos.csail.mit.edu/6.828/2022/xv6.html](https://pdos.csail.mit.edu/6.828/2022/xv6.html))
    *   **rCore-Tutorial**: A fantastic OS tutorial in Rust from China. ([https://github.com/rcore-os/rCore-Tutorial](https://github.com/rcore-os/rCore-Tutorial))
    *   **Berkeley's CS61C & CS162**: Great courses on computer architecture and operating systems.

### License

This project is licensed under the MIT License - see the LICENSE file for details.

---

## SuepOS (上海电力大学操作系统)

一个为 RISC-V 架构设计的简单、用于教学的操作系统，由上海电力大学（SUEP）开发。其目标是通过简洁易读的代码和动手实践，帮助学生理解操作系统的核心概念，如启动流程、特权级、异常处理、内存管理和抢占式多任务。

**架构**: RISC-V 64 (RV64GC)
**平台**: QEMU `virt` 虚拟机
**状态**: 开发中 (教学使用)

### 已实现功能

*   **引导程序**: 使用 OpenSBI 作为固件，将系统引导至 S 模式（监管者模式）。
*   **内存管理**: 实现简单的基于页表的虚拟内存系统。
*   **异常与中断处理**: 在 S 模式下处理异常和时钟中断。
*   **多任务处理**: 一个极简的抢占式调度器，采用简单的时间片轮转策略。
*   **系统调用**: 基础的系统调用接口（例如，使用 `sbi_console_putchar` 进行输出）。
*   **教学重点**: 代码包含大量注释，采用模块化设计，清晰度优先于性能。

### 快速开始

#### 前提条件

*   RISC-V 64 位 GCC 工具链 (例如 `riscv64-unknown-elf-gcc`)
*   支持 RISC-V 的 QEMU 系统模拟器 (例如 `qemu-system-riscv64`)
*   Make 构建工具
*   另外，我将免费提供所有资源 :)

#### 构建和运行

1.  克隆代码库:
    ```bash
    git clone https://github.com/tomlay0520/SuepOS.git
    cd SuepOS
    ```

2.  编译内核:
    ```bash
    make
    ```

3.  在 QEMU 中运行 SuepOS:
    ```bash
    make run
    ```
    按下 `Ctrl+A` 然后按 `X` 来退出 QEMU。

4.  清理所有目标文件:
    ```bash
    make clean
    ```

### 项目结构

```
os/
├── kernel      
│   ├── kernel.o/          
│   ├── os.bin/       
│   ├── os.elf/ 
│   └── start.o/      
├── kernel.c 
├── platform.h
├── qemu_gcc.mk
├── start.S
├── type.h
├── uart0.c
└── Makefile
```

### 内存映射 （内核）
```
内存布局:
 0x80000000 +-----------------+
            |     .text       | TEXT SEGMENT
            +-----------------+
            |    .rodata      | READ-ONLY DATA SEGMENT
            +-----------------+
            |     .data       | DATA SEGMENT
            +-----------------+
            |      .bss       | BSS SEGMENT AUTO-INITIALIZED TO ZERO
            +-----------------+
            |      heap       | DYNAMIC MEMORY ALLOCATION
            |                 | 
            +-----------------+
            |  available RAM  | FREE FOR OTHER USES
 0x84000000 +-----------------+  (0x80000000 + 64MB)
```

### 完整的内存映射
```
    0x00000000 --> +-----------+
                   |   BIOS    |
    0x00001000 --> +-----------+
                   | boot code |
    0x02000000 --> +-----------+
                   |   CLINT   |
    0x0C000000 --> +-----------+
                   |   PLIC    |
    0x10000000 --> +-----------+
                   |   UART0   |
    0x10001000 --> +-----------+
                   |  VIRTIO0  |
    0x80000000 --> +-----------+
                   |           |
                   |  KERNEL   |
                   |           |
    0x84000000 --> +-----------+
```

### 参考资料与延伸阅读

*   **《RISC-V 指令集手册》**
    *   第一卷：非特权级 ISA ([https://riscv.org/technical/specifications/](https://riscv.org/technical/specifications/))
    *   第二卷：特权级 ISA ([https://riscv.org/technical/specifications/](https://riscv.org/technical/specifications/))
*   **《RISC-V 监管者二进制接口 (SBI) 规范》**
    *   [https://github.com/riscv-non-isa/riscv-sbi-doc](https://github.com/riscv-non-isa/riscv-sbi-doc)
*   **OpenSBI**: RISC-V SBI 规范的一个实现
    *   [https://github.com/riscv-software-src/opensbi](https://github.com/riscv-software-src/opensbi)
*   **QEMU RISC-V `virt` 虚拟机文档**
    *   `qemu-system-riscv64 -machine virt help`
*   **教学操作系统灵感来源**:
    *   **xv6**: 一个简单的类 Unix 教学操作系统。([https://pdos.csail.mit.edu/6.828/2022/xv6.html](https://pdos.csail.mit.edu/6.828/2022/xv6.html))
    *   **rCore-Tutorial**: 一个优秀的来自中国的 Rust 语言操作系统教程。([https://github.com/rcore-os/rCore-Tutorial](https://github.com/rcore-os/rCore-Tutorial))
    *   **Berkeley 的 CS61C 和 CS162 课程**: 关于计算机体系结构和操作系统的优秀课程。

### 许可证

本项目基于 MIT 许可证，详情请查看 LICENSE 文件。

