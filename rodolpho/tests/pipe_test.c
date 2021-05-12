#include "libft.h"
#include <stdio.h>

int	main()
{
	pid_t	id;
	int		fd_pipe_1[2];
	int		fd_pipe_2[2];

	char *argv_comm_1[3] = {"/bin/ls", "-l", NULL};
	char *argv_comm_2[2] = {"/usr/bin/wc", NULL};
	char *argv_comm_3[3] = {"/bin/cat", "-e", NULL};

	pipe(fd_pipe_1);
	id = fork();
	if (id == 0)	// 1st child process
	{
		// output pipe
		close(fd_pipe_1[0]); // close read end
		dup2(fd_pipe_1[1], 1); // stdout is closed and now fd 1 points to the write end of the pipe 
		close(fd_pipe_1[1]);
		execve(argv_comm_1[0], argv_comm_1, NULL);
		printf("execve returned erro\n");
		return (1);
	}
	close(fd_pipe_1[1]); // close write end
	pipe(fd_pipe_2);
	id = fork();
	if (id == 0)	// 2nd child process
	{
		// input pipe
		dup2(fd_pipe_1[0], 0); // stdin is closed and now fd 0 points to the read end of the pipe
		close(fd_pipe_1[0]);

		// outputpipe
		close(fd_pipe_2[0]); // close read end of 2nd pipe
		dup2(fd_pipe_2[1], 1); // stdout deviated to write end of 2nd pipe
		close(fd_pipe_2[1]);
		execve(argv_comm_2[0], argv_comm_2, NULL);
		printf("execve returned erro\n");
		return (1);
	}
	close(fd_pipe_1[0]);	// close read end of 1st pipe
	close(fd_pipe_2[1]);	// close write end of 2nd pipe
	id = fork();
	if (id == 0) // 3rd child process
	{
		// input pipe
		dup2(fd_pipe_2[0], 0); // stdin deviated to read end of 2nd pipe
		close(fd_pipe_2[0]);
		execve(argv_comm_3[0], argv_comm_3, NULL);
		printf("execve returned erro\n");
		return (1);
	}
	close(fd_pipe_2[0]);
	wait(NULL);
	wait(NULL);
	printf("Parent process says goodbye!\n");
	return (0);
}
