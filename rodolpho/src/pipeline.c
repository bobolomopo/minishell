#include "header.h"

// function to be called by a subshell.
// This function does not return, it exits.
void	run_piped_command(t_command *command, t_sh_env *shenv)
{
	int		builtin_index;
	int		ret;

	if (make_var_expansions(command, shenv) == -1)
		clear_mem_exit(shenv, -1);	// no one's gonna check this return code... 
		// maybe this should be done by the parent process
	builtin_index = is_builtin(command->argv[0]);
	if (builtin_index != -1)
	{
		ret = run_builtin(builtin_index, command, shenv);
		clear_mem_exit(shenv, ret);
	}
	exec_bin(command, shenv);
}

void	pipeline_child_process(t_command *command, t_sh_env *shenv, int *fd_pipe, int fd_in)
{
	int	error;

	close(fd_pipe[0]);	// the read-end of the pipe is closed
	error = 0;
	if (fd_in != 0)
	{
		if (dup2(fd_in, 0) == -1) // stdin becomes fd_in
			error = 1;
		close(fd_in);
	}
	if (dup2(fd_pipe[1], 1) == -1) // stdout becomes the write-end of the pipe
		error = 1;
	close(fd_pipe[1]);
	if (error)
	{
		ft_perror("dup2");
		clear_mem_exit(shenv, 1);
	}
	run_piped_command(command, shenv);
}

// makes a pipe - fork - dup2 cycle
// runs command in from the subshell (child process)
// returns read_end of pipe or -1 if error
int	send_to_pipe(t_command *comm, t_sh_env *shell_env, int fd_in)
{
	int	fd_pipe[2];
	int	id;

	if (pipe(fd_pipe) == -1)
		return (ft_perror_ret("pipe", -1));
	id = fork();
	if (id == -1)
	{
		ft_perror("fork");
		close(fd_pipe[0]);
		close(fd_pipe[1]);
		return (-1);
	}
	if (id == 0)
		pipeline_child_process(comm, shell_env, fd_pipe, fd_in);
	if (fd_in != 0)
		close(fd_in);
	close(fd_pipe[1]);
	return (fd_pipe[0]);
}

// returns pid of child process, or -1 if error
int	pipeline_last_command(t_command *command, t_sh_env *shenv, int fd_in)
{
	pid_t	id;

	id = fork();
	if (id == -1)
	{
		ft_perror("fork");
		close(fd_in);
		return (-1);
	}
	if (id == 0) // child process
	{
		if (dup2(fd_in, 0) == -1)
		{
			ft_perror("dup2");
			close(fd_in);
			return (-1);
		}
		close(fd_in);
		run_piped_command(command, shenv);
	}
	close(fd_in);
	return (id);
}

// runs a list of n piped commands (2 or more)
int	run_pipeline(t_list *lst, t_sh_env *shell_env, int n)
{
	int		pipe_left_read_end;
	int		id_last_command;
	int		status;

	pipe_left_read_end = 0;
	while (lst->next)
	{
		pipe_left_read_end = send_to_pipe(lst->content, shell_env, pipe_left_read_end);
		if (pipe_left_read_end == -1)
			return (-1);
		lst = lst->next;
	}
	id_last_command = pipeline_last_command(lst->content, shell_env, pipe_left_read_end);
	if (id_last_command == -1)
		return (-1);
	waitpid(id_last_command, &status, 0);
	while (--n)
		wait(NULL);
	return (exit_code_from_child(status));
}
