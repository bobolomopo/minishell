#include "header.h"

void	exec_command(char **argv, char **envp)
{
	pid_t	pid_child;
	int		stat_loc;
	char	*path;

	if (argv[0] == NULL)
		return ;
	path = resolve_path(argv[0]);
	if (!path)
	{
		printf("minishell: %s: command not found\n", argv[0]); // gotta print it to stderr...
		return ;
	}
	pid_child = fork();
	if (pid_child == 0)	// child process
	{
		execve(path, argv, envp);
		printf("%s\n", strerror(errno)); // error messages show go to stdout...
	}
	else				// parent process
	{
		wait(&stat_loc);
		free(path);
	}
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
		ft_free_split(argv_command);
		write(1, prompt, ft_strlen(prompt));
	}
	return (0);
}
