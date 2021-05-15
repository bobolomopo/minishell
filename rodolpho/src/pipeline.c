#include "header.h"

void	update_last_exit(char **envp, int exit_code)
{
	// TODO

	(void)envp;
	(void)exit_code;
}

// makes a pipe - fork - dup2 cycle
// returns read_end of pipe or -1 if error
int	send_to_pipe(t_command *comm, char **envp, int fd_in)
{
	int	fd_pipe[2];
	int	id;

	if (pipe(fd_pipe) == -1)
		return (-1);	// include error msg
	id = fork();
	if (id == -1)
		return (-1);	// include error msg
	if (id == 0)	// child process
	{
		if (fd_in != 0)
		{
			dup2(fd_in, 0); // check error...
			close(fd_in);
		}
		dup2(fd_pipe[1], 1);
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		launch_command(comm, envp);
		exit(0);
	}
	if (fd_in != 0)
		close(fd_in);
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}

// runs a list of n piped commands (2 or more)
int	run_pipeline(t_list *lst, char **envp, int n)
{
	int		pipe_left_read_end;
	pid_t	id;
	int		status;

	pipe_left_read_end = 0;
	while (lst->next)
	{
		pipe_left_read_end = send_to_pipe(lst->content, envp, pipe_left_read_end);
		if (pipe_left_read_end == -1)
			return (-1);
		lst = lst->next;
	}
	id = fork(); // check for error?
	if (id == 0)
	{
		dup2(pipe_left_read_end, 0);
		close(pipe_left_read_end);
		exit(launch_command(lst->content, envp));
	}
	close(pipe_left_read_end);
	waitpid(id, &status, 0);
	while (--n)
		wait(NULL);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	return (0); // ??? what to return if command did not exit?
}

