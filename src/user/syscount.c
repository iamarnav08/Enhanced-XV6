#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

const char *syscall_names[] = {
    [0]  = "invalid",       // Placeholder for index 0, as system calls start from 1
    [1]  = "fork",
    [2]  = "exit",
    [3]  = "wait",
    [4]  = "pipe",
    [5]  = "read",
    [6]  = "kill",
    [7]  = "exec",
    [8]  = "fstat",
    [9]  = "chdir",
    [10] = "dup",
    [11] = "getpid",
    [12] = "sbrk",
    [13] = "sleep",
    [14] = "uptime",
    [15] = "open",
    [16] = "write",
    [17] = "mknod",
    [18] = "unlink",
    [19] = "link",
    [20] = "mkdir",
    [21] = "close",
    [22] = "waitx",
    [23] = "getSysCount",   // Newly defined system call
    [24] = "__",
    [25] = "__",
    [26] = "__",
    [27] = "__",
    [28] = "__",
    [29] = "__",
    [30] = "__",
    [31] = "__",
    [32] = "__"
};


int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(2, "Usage: syscount <mask> command [args]\n");
        exit(1);
    }

    int mask = atoi(argv[1]);

    if (fork() == 0) {
        printf("running the commnad\n");
        // Child process runs the command
        exec(argv[2], &argv[2]);
        fprintf(2, "exec %s failed\n", argv[2]);
        exit(1);
    } else {
        // Parent process waits for the child to finish
        wait(0);
        printf("command finished\n");

        int index=0;
        // Get the syscall count from the kernel
        int count = getSysCount(mask, &index);
        
        
        printf("PID %d called %s %d times.\n", getpid(), syscall_names[index], count);
    }
    exit(0);
}
