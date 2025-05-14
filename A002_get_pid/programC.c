#include <stdio.h>
#include <unistd.h>
int main() {
	pid_t pid = getpid();
	pid_t ppid = getppid();
	printf("My PID is: %d\n", pid);
	printf("My parent's PID is: %d\n\n", ppid);
	pid_t child_pid = fork();
	if (child_pid == 0) {
		printf("I am the child process PID: %d\n", getpid());
		printf("My parent's PID is: %d\n\n", getppid());
	} else if (child_pid > 0) {
		printf("I am the parent process PID: %d\n", getpid());
		printf("My child's PID is: %d\n\n", child_pid);
	} else {
		printf("Fork failed\n");
		return 1;
	}
	

	return 0;
}
	
