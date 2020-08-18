#include <linux/kernel.h>
#include <linux/sched.h>                // task_struct
#include <linux/errno.h>                // EINVAL
#include <linux/string.h>               // strcpy
#include <linux/syscalls.h>             // macro SYSCALL_DEFINEx
#include <linux/uaccess.h>              // copy_to_user


struct proc_info { //info about a single process
    pid_t pid;                          //pid of the process
    char name[16];                      //file name of the program executed
};

struct procinfos { //info about processes we need
    long   studentID;                   //for the assignment testing
    struct proc_info proc;              // process with pid or current process
    struct proc_info parent_proc;       // parent process
    struct proc_info oldest_child_proc; // oldest child process
};


//asmlinkage long sys_get_proc_info(pid_t pid , struct procinfos * info)
SYSCALL_DEFINE2(sys_get_proc_info, pid_t, pid, struct procinfos*, info)
{
    struct task_struct  *proc = NULL; 
    struct task_struct  *parent_proc = NULL;
    struct task_struct  *oldest_child_proc = NULL;
    void *isChild = NULL; 
    
    /*       Allocating Memory in the Kernel       */
    struct procinfos *kinfo = (struct procinfos*) kmalloc(
        sizeof(struct procinfos), GFP_KERNEL
    );

    if(kinfo == NULL)       // Check error >-<
        return ENOMEM;      // cannot allocate memory
    /*--------------------------------------------*/
    
    /*  Find current process or process with pid  */
    if(pid == -1) proc = current;
    else proc = find_task_by_vpid(pid);

    if(proc == NULL)        // Check error >-<
        return ESRCH;       // No such process

    kinfo->proc.pid = proc->pid;
    strcpy(kinfo->proc.name, proc->comm); 
    /*--------------------------------------------*/

    /*    Find parent process of first process    */
    parent_proc = proc->parent;

    if(parent_proc == NULL) // Check error >-<
        return ESRCH;       // No such process

    kinfo->parent_proc.pid = parent_proc->pid;
    strcpy(kinfo->parent_proc.name, parent_proc->comm); 
    /*--------------------------------------------*/

    /* Find oldest child process of first process */
    isChild = list_first_entry_or_null (
        &proc->children, struct task_struct, sibling);

    if(isChild == NULL) {
        kinfo->oldest_child_proc.pid = (pid_t) -1;
        strcpy(kinfo->oldest_child_proc.name, "No Name"); 
    }
    else {
        oldest_child_proc  = list_last_entry( 
            &proc->children, struct task_struct, sibling); 

        if(oldest_child_proc == NULL) // Check error >-<
        return ESRCH;                 // No such process

        kinfo->oldest_child_proc.pid = oldest_child_proc->pid;
        strcpy(kinfo->oldest_child_proc.name, oldest_child_proc->comm); 
    }
    /*--------------------------------------------*/
    kinfo->studentID = 1712177; // My student ID

    /* Copy data from kernel space to user space  */
    copy_to_user(info, kinfo, sizeof(struct procinfos));
    kfree(kinfo); // Deallocate kernel memory
    /*--------------------------------------------*/

    return 0;
}

