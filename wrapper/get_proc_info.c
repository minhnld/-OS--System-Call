#include "get_proc_info.h"
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>

long get_proc_info(pid_t pid, struct procinfos* info){
    long sys_return_value;
    sys_return_value = syscall(335, pid , info);
    return 0;
}
