#include <get_proc_info.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
int main() {
	pid_t mypid = getpid();
	printf("PID: %d\n", mypid);
	struct procinfos info;
	if (get_proc_info(mypid, &info) == 0) {
// TODO: print all information in struct procinfos info
	printf("Student ID: %lu \n",info.studentID );
	printf("My Process info : \n");
    printf("PID: %d \n",info.proc.pid);
	printf("Name: %s \n",info.proc.name);	
    printf("My Parent Process info : \n");
    printf("PID: %d \n",info.parent_proc.pid);
	printf("Name: %s \n",info.parent_proc.name);
    printf("My Oldest child Process info : \n");
    printf("PID: %d \n",info.oldest_child_proc.pid);
	printf("Name: %s \n",info.oldest_child_proc.name);	
}else{
printf("Cannot get information from the process %d\n", mypid);
}
// If necessary, uncomment the following line to make this program run
// long enough so that we could check out its dependence
// sleep(100);
}
