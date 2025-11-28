# Lottery Scheduler Implementation in xv6-riscv

**Assignment 2: Kernel Feature Implementation**  
**OS Fall 2025**

---

## Overview

This repository contains a modified version of the **xv6-riscv** operating system kernel. The default Round-Robin scheduler has been replaced with a **Lottery Scheduler**, a probabilistic priority-based scheduling algorithm.

### How it Works:
- Every process is assigned a number of **tickets**
- The scheduler holds a **lottery** every timer tick
- The process holding the **winning ticket** gets the CPU
- Processes with **more tickets** have a higher probability of being selected, resulting in more CPU time

---

## Implemented Features

### A. Core Scheduler Logic (`kernel/proc.c`)
- **Ticket Assignment**: Added a `tickets` field to `struct proc`
- **Default Tickets**: All new processes (via `fork`) are initialized with **10 tickets** by default
- **Lottery Algorithm**: The `scheduler()` loop now:
  - Calculates the total number of tickets among `RUNNABLE` processes
  - Picks a random winning ticket
  - Selects the corresponding process to run
- **Random Number Generator**: Implemented a Linear Congruential Generator (LCG) to support the lottery mechanism

### B. New System Call: `settickets`
- **Prototype**: `int settickets(int number)`
- **Description**: Allows a process to modify its own ticket count
- **Validation**: Returns `-1` if the ticket count is less than 1; otherwise returns `0`

---

## How to Run

### Prerequisites
- Linux environment (WSL2 Ubuntu recommended for Windows users)
- RISC-V Toolchain (`gcc-riscv64-linux-gnu`, `qemu-system-misc`)

### Build and Boot
1. Navigate to the project directory:
   ```bash
   cd xv6-riscv
   ```

2. Clean previous builds (optional but recommended):
   ```bash
   make clean
   ```

3. Compile and launch xv6 in QEMU:
   ```bash
   make qemu
   ```

4. **To exit QEMU**: Press `Ctrl+A`, release, then press `X`

---

## Testing

Three user-level programs have been added to validate the implementation.

### 1. Basic Functionality Test (`lotterytest`)
**Goal**: Verifies that processes with more tickets receive proportionally more CPU time

**Command**: 
```bash
$ lotterytest
```

**Scenario**: Creates two child processes:
- Child A gets **10 tickets**
- Child B gets **80 tickets**

**Expected Output**: You will see timing information showing Child B (80 tickets) finishes roughly **8x faster** than Child A (10 tickets), proving the weighted distribution works.

---

### 2. Stress Test (`stress`)
**Goal**: Ensures the kernel remains stable under high load

**Command**: 
```bash
$ stress
```

**Scenario**: Rapidly creates **50 processes** with varying ticket counts. Each process performs a calculation and exits.

**Expected Output**: The system should not panic or freeze. It will output:
```
Stress Test Passed: All 50 processes finished safely.
```

---

### 3. Edge-Case Test (`edgetest`)
**Goal**: Verifies robust error handling for invalid system call arguments

**Command**: 
```bash
$ edgetest
```

**Scenario**: Attempts to set:
- Negative tickets (`-10`)
- Zero tickets (`0`)
- Very high tickets (`10000`)

**Expected Output**:
- `settickets(-10)` → Rejected
- `settickets(0)` → Rejected  
- `settickets(10000)` → Accepted

---

## File Structure Changes

| File | Changes |
|------|---------|
| `kernel/proc.h` | Added `tickets` field to `struct proc` |
| `kernel/proc.c` | Modified `scheduler()`, `allocproc()`, added `random()` |
| `kernel/sysproc.c` | Added `sys_settickets()` implementation |
| `kernel/syscall.h/c` | Registered the new system call |
| `user/user.h` | Added user-space prototype |
| `user/usys.pl` | Added assembly entry point |
| `user/lotterytest.c` | New test file |
| `user/stress.c` | New test file |
| `user/edgetest.c` | New test file |

---

## Team Members

| Name | CMS ID |
|------|--------|
| Ayesha Tariq | 456571 |
| Roshan Jalil | 459766 |
| Muhammad Jahanzeb Babar | 461484 |
| Syed Ali Hassan Jaffri | 463673 |
| Syed Fawwad Ahmed | 463933 |
| Muhammad Shaheer Saleh | 467668 |

---

## License

This project is based on xv6-riscv, which is licensed under the MIT License.
