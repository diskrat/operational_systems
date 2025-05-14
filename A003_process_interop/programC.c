#include <stdio.h>
#include <unistd.h>
int main() {
	pid_t pid = getpid();
	pid_t ppid = getppid();
	printf("My PID is: %d\n", pid);
	printf("My parent's PID is: %d\n\n", ppid);
	int i = 0;
	printf("value: %d\n",i);
	pid_t child_pid = fork();
	if (child_pid == 0) {
	
		
		printf("Im the child: value: %d\n",i);
	} else if (child_pid > 0) {
		i=1;
		printf("I'm the parent: value: %d\n",i);
	} else {
		printf("Fork failed\n");
		return 1;
	}
	//printf("unknown process: value: %d\n",i);

	return 0;
}
	
