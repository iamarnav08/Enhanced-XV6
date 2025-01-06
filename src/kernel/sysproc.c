#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"


uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0; // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if (growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while (ticks - ticks0 < n)
  {
    if (killed(myproc()))
    {
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

uint64
sys_waitx(void)
{
  uint64 addr, addr1, addr2;
  uint wtime, rtime;
  argaddr(0, &addr);
  argaddr(1, &addr1); // user virtual memory
  argaddr(2, &addr2);
  int ret = waitx(addr, &wtime, &rtime);
  struct proc *p = myproc();
  if (copyout(p->pagetable, addr1, (char *)&wtime, sizeof(int)) < 0)
    return -1;
  if (copyout(p->pagetable, addr2, (char *)&rtime, sizeof(int)) < 0)
    return -1;
  return ret;
}

uint64 
sys_getSysCount(void) 
{
  int mask;
  struct proc *p = myproc();

  //retrieve value of mask from cli arguement
  argint(0, &mask);
  int* call_index_ptr;
  argaddr(1, (uint64*)&call_index_ptr);

  for(int i=0; i<32; i++) {
    if((mask >> i) & 1) {
      if (copyout(myproc()->pagetable, (uint64)call_index_ptr, (char *)&i, sizeof(i)) < 0)
        return -1;
      int ret=p->syscall_count_copy[i];
      // for(int j=0; j<33; i++){
      //   p->syscall_count_copy[i] = 0;
      // }
      return ret;
    }
  }
  return -1;

}

uint64
sys_sigalarm(void)
{
  int interval;
  uint64 handler;

  // Retrieve the interval and handler from the user stack
  argint(0, &interval);
  argaddr(1, &handler);

  // Access the current process
  struct proc *p = myproc();

  // Set the alarm interval and handler in the process structure
  p->alarm_interval = interval;
  p->alarm_handler = (uint64)handler;
  // printf("Alarm set for process %d with interval %d\n", p->pid, interval);
  // printf("Alarm handler set to %p\n", handler);
  p->ticks_count = 0;  // Reset tick counter
  return 0; // Return success
}

uint64 sys_sigreturn(void)
{
  struct proc *p = myproc();
  // memmove(p->trapframe, p->saved_tf, sizeof(*p->trapframe));
  // kfree(p->saved_tf);
  // p->saved_tf = 0;
  if (p->saved_tf) {
    memmove(p->trapframe, p->saved_tf, sizeof(*p->trapframe));
    kfree(p->saved_tf);
    p->saved_tf = 0;
  }
  p->alarm_status = 0;
  p->ticks_count = 0;

  usertrapret();
  
  return 0;
}

uint64 sys_settickets(void)
{
  int tickets;
  argint(0, &tickets);
  struct proc *p = myproc();
  p->tickets = tickets;
  return 0;
}