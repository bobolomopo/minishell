#include "libft.h"
#include <fcntl.h>
#include <stdio.h>
#include "header.h"

// NOTES:
// resolve_path returns a freeable string. So it must be called before the fork, so the
// parent process can free it. Consider moving the fork to inside the functions first_command, etc.

// code a function for a single command, supporting redirections.

// when executing a builtin, does the process fork?? if not, adapt code...

void	first_command(char *in_file, int *pipe_fd, char **com_argv)
{
	int	fd_in;

	close(pipe_fd[0]);	// close read end of pipe
	dup2(pipe_fd[1], 1); // redirect stdout to write end of pipe
	close(pipe_fd[1]);	// close write end of pipe
	if (in_file)
	{
		fd_in = open(in_file, O_RDONLY);
		if (fd_in == -1)
			exit(1); // error
		dup2(fd_in, 0);	// stdin comes from fd_in now
		close(fd_in);
	}
	execve(com_argv[0], com_argv, NULL);
	printf("error in first_command execve\n");
	exit(1);
}

void	middle_command(int fd_in, int *pipe_fd, char **com_argv)
{
	dup2(fd_in, 0);
	close(fd_in);

	close(pipe_fd[0]); // close read end
	dup2(pipe_fd[1], 1); // redirect stdout
	close(pipe_fd[1]); // close write end

	execve(com_argv[0], com_argv, NULL);
	printf("error in middle_command execve\n");
	exit(1);
}

void	last_command(int *pipe_fd, char *out_file, int out_file_trunc, char ** com_argv)
{
	int	fd_out;
	int	o_flag;

	dup2(pipe_fd[0], 0); // redirect stdin form the pipe
	close(pipe_fd[0]);

	if (out_file)
	{
		o_flag = O_WRONLY | O_CREAT | O_APPEND;
		if (out_file_trunc)
			o_flag = O_WRONLY | O_CREAT | O_TRUNC;
		fd_out = open(out_file, o_flag, S_IRWXU);
		if (fd_out == -1)
			exit(1);
		dup2(fd_out, 1); // redirect stdout to file
		close(fd_out);
	}
	execve(com_argv[0], com_argv, NULL);
	printf("error in last_command execve\n");
	exit(1);
}

// executes sequence of commands, handling piping and redirections
// for 2 or more commands
void	exec_piped_commands(t_piped_commands group)
{
	int		n_commands;
	int		prev_pipe_read_end;
	int		pipe_fd[2];
	pid_t	id;
	t_list	*lst_ptr;

	n_commands = ft_lstsize(group.commands_lst);
	lst_ptr = group.commands_lst;

	pipe(pipe_fd);
	id = fork(); 
	if (id == 0)
		first_command(group.in_file, pipe_fd, group.commands_lst->content);
	close(pipe_fd[1]); // close write_end
	lst_ptr = lst_ptr->next;
	while (lst_ptr->next != NULL)
	{
		prev_pipe_read_end = pipe_fd[0];
		pipe(pipe_fd);
		id = fork();
		if (id == 0)
			middle_command(prev_pipe_read_end, pipe_fd, lst_ptr->content);
		close(prev_pipe_read_end); // close read_end from previous pipe
		close(pipe_fd[1]);		// close write_end of current pipe
		lst_ptr = lst_ptr->next;
	}
	id = fork();	// last command
	if (id == 0)
		last_command(pipe_fd, group.out_file, group.out_file_truc, lst_ptr->content);
	close(pipe_fd[0]);  // close read_end
	while (n_commands--)
		wait(NULL);
	printf("Parent process says goodbye!\n");

}

int	main()
{
	t_piped_commands group;
	t_list	*commands_lst = NULL;
	char *comm_1[2] = {"/bin/cat", NULL};
	char *comm_2[2] = {"/usr/bin/wc", NULL};
	char *comm_3[3] = {"/bin/cat", "-e", NULL};

	ft_lstadd_back(&commands_lst, ft_lstnew(comm_1));
	ft_lstadd_back(&commands_lst, ft_lstnew(comm_2));
	ft_lstadd_back(&commands_lst, ft_lstnew(comm_3));
	group.commands_lst = commands_lst;
	group.in_file = "text";
	group.out_file = "result.txt";
	group.out_file_truc = 0;
	exec_piped_commands(group);
	ft_lstclear(&commands_lst, NULL);
	return (0);
}
