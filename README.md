# Enhanced XV6 Operating Systems

## Project Overview
This mini project focuses on extending the XV6 operating system by implementing various system calls, scheduling policies.

## System Calls

### 1. System Call Counting (`getSysCount`)
- Modified `struct proc` to track system call occurrences
- Implemented counting for current and child processes
- Integrated with `exit` and `wait` functions

**Syntax**
```bash
syscount <mask> command [args]
```
where command [args] is any other valid command in XV6.

**Example**
```bash
$ syscount 32768 grep hello README.md
PID 6 called open 1 times.
```

### 2. Alarm System Calls
- Implemented two system calls: `sigalarm` and `sigreturn`
- Added parameters to `struct proc`:
  - Alarm interval
  - Alarm handler
  - Ticks count
  - Alarm status
  - Saved trap frame*


**Description**

If an application calls sigalarm(n, fn) , then after every n  ”ticks” of CPU time that the program consumes, the kernel will cause application function fn  to be called. When fn  returns, the application will resume where it left off.

The system call sigreturn() resets the process state to before the handler was called.

## Scheduling Policies

### Lottery-Based Scheduling (LBS)
- Created `settickets` system call
- Added tickets and arrival time to process structure
- Implemented lottery selection mechanism
- Key features:
  - Processes initialized with 1 ticket
  - Child processes inherit parent's ticket count
  - Arrival time helps prevent starvation

### Multi-Level Feedback Queue (MLFQ)
- Added priority queue attributes to process structure.
- Implemented four priority queues with different time slices:
    - Priority 0: 1 timer tick
    - Priority 1: 4 timer ticks
    - Priority 2: 8 timer ticks
    - Priority 3: 16 timer ticks
- Processes are pushed to the highest priority queue (priority 0) upon initiation.
- The scheduler always runs processes from the highest non-empty priority queue.
- If a higher priority process enters, it preempts the currently running lower priority process.
- Processes that use their entire time slice are moved to the next lower priority queue.
- Processes that voluntarily relinquish the CPU are reinserted at the tail of the same queue.
- Round-robin scheduling is used for the lowest priority queue.
- Implemented priority boosting to prevent starvation by moving all processes to the topmost queue (priority 0) after 48 ticks.

### Performance Comparison
| Policy | Average Run Time | Average Wait Time |
|--------|-----------------|-------------------|
| Round Robin | 15 | 160 |
| Lottery Based Scheduling | 14 | 143 |
| MLFQ | 14 | 140 |

<!-- TODO: Insert MLFQ Analysis Graph Here -->



## Compilation and Running
- Use Makefile with scheduler argument: `SCHEDULER=XYZ`.
- Available scheduling options are MLFQ, LBS and RR.

**Syntax**
```bash
make clean; make qemu SCHEDULER=XYZ CPUS=x
```


## Author
Arnav Sharma

Student ID: 2023111033

## Notes
- Implemented in XV6 Operating System
- Focus on system-level programming.
