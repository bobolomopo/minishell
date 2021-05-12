#include "libft.h"
#include <stdio.h>

int	main()
{
	pid_t	id;
	int		fd_pipe[2];

	char *argv_comm_1[3] = {"/bin/ls", "-l", NULL};
	char *argv_comm_2[2] = {"/usr/bin/wc", NULL};


	pipe(fd_pipe);
	id = fork();
	if (id == 0)	// 1st child process
	{
		close(fd_pipe[0]); // close read end
		dup2(fd_pipe[1], 1); // stdout is closed and now fd 1 points to the write end of the pipe 
		close(fd_pipe[1]);
		execve(argv_comm_1[0], argv_comm_1, NULL);
		printf("execve returned erro\n");
		return (1);
	}
	close(fd_pipe[1]); // close write end
	id = fork();
	if (id == 0)	// 2nd child process
	{
		dup2(fd_pipe[0], 0); // stdin is closed and now fd 0 points to the read end of the pipe
		execve(argv_comm_2[0], argv_comm_2, NULL);
		printf("execve returned erro\n");
		return (1);
	}
	close(fd_pipe[0]);	// close read end
	wait(NULL);
	wait(NULL);
	printf("Parent process says goodbye!\n");
	return (0);
}
