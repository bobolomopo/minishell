
#include "header.h"

void	free_split(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}

void	exec_command(char **argv, char **envp)
{
	pid_t	pid_child;
	int		stat_loc;
	char	*file;

	file = resolve_path(argv[0]);

	pid_child = fork();
	if (pid_child == 0)	// child
	{
		execve(file, argv, envp);
		printf("%s\n", strerror(errno));
	}
	else				// parent process
		wait(&stat_loc);
		free(file);
}

int	main(int argc, char **argv, char **envp)
{
	char	*prompt = "minishell> ";
	char	*line;
	char	**argv_command;

	(void)argc;
	(void)argv;
	write(1, prompt, ft_strlen(prompt));
	while (get_next_line(0, &line) == 1)
	{
		argv_command = ft_split(line, ' ');
		free(line);
		exec_command(argv_command, envp);
		free_split(argv_command);
		write(1, prompt, ft_strlen(prompt));
	}
	return (0);
}
