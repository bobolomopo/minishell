#include <stdio.h>
#include <string.h>
#include "libft.h"
#include <signal.h>

void	exec_command(char **argv, char **envp)
{
	pid_t	pid_child;
	int		stat_loc;

	pid_child = fork();
	if (pid_child == 0)	// child
	{
		execve(argv[0], argv, envp);
		printf("%s\n", strerror(errno));
	}
	else				// parent process
		wait(&stat_loc);
}

// just ignore the SIGINT signal via a signal handler, as this does not get inherited by an execd process.
void	signal_handler(int signal_code)
{
	(void)signal_code;
}

int	main(int argc, char **argv, char **envp)
{
	char	*prompt = "prompt $ ";
	char	*line;
	char	**argv_command;

	signal(SIGINT, signal_handler); // ctrl-C
	signal(SIGQUIT, signal_handler); /* ctrl-\ */
	write(1, prompt, ft_strlen(prompt));
	while (get_next_line(0, &line) == 1)
	{
		if (ft_strcmp(line, "exit") == 0)
		{
			free(line);
			return (0);
		}
		argv_command = ft_split(line, ' ');
		free(line);
		exec_command(argv_command, envp);
		ft_free_split(argv_command);
		write(1, prompt, ft_strlen(prompt));
	}
	(void)argc;
	(void)argv;
	return (0);
}
