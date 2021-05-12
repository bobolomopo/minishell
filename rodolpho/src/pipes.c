#include "libft.h"

typedef struct	s_piped_commands
{
	t_list	*commands_lst;
	char	*in_file;		// NULL if stdin
	char	*out_file;		// NULL if stdout
}				t_piped_commands;

void	*free_fd_arr(int **fd, int n_rows)
{
	// TODO
	return (NULL);
}

// allocate [n][2] array of ints, open pipes
int	**open_pipes(int n)
{
	int **fd;
	int	i;

	fd = malloc(sizeof(*fd) * n);
	if (!fd)
		return (NULL);
	i = 0;
	while (i < n)
	{
		fd[i] = malloc(sizeof(int) * 2);
		if (!fd[i])
			return (free_fd_arr(fd, i - 1));
		i++;
	}
	i = 0;
	while (i < n)
	{
		if (pipe(fd[i]) == -1)
			return (NULL);	// close open pipes and free array...
		i++;
	}
	return (fd);
}

// executes sequence of commands, handling piping and redirections
void	exec_piped_commands(t_piped_commands group)
{
	int	n_pipes;
	int	**pipes;

	n_pipes = ft_lstsize(group.commands_lst) - 1;  // if 0 ? -1 ?
	pipes = open_pipes (n_pipes);
	if (!pipes)
		return ; // error
	
	// open fd's for in_file and out_file

	// for each command in the list, fork new process, setup fds, and exec

	// first command in is fd from in_file
	// last command out in fd from out_file
	// others: 
	//		command[i] 	in: pipes[i - 1][0] (read_end)
	//					out: pipes[i][1]	(write_end)

}