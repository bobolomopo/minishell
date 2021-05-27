#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>

int main()
{

	void *ptr = malloc(10);

	pid_t pid = fork();
	
	if (pid) // padre
	{
		printf("hola je suis el padre, pid del hijo: %d\n", pid);
		free(ptr);
	}
	else	// hijo
	{
		char *argv[] = {"echo", "hello", "test!", NULL};

		execve("/usr/bin/echo", argv, NULL);
		
	}

	return (0);
}