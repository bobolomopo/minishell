#include "header.h"

int	line_is_blank(char *line)
{
	while (*line)
	{
		if (!ft_isspace(*line))
			return (0);
		line++;
	}
	return (1);
}

// adds the last command-line to the history
// upon error, message is displayed, but this error is non-fatal
// for the shell.
void	add_to_history(char *line, t_list **history)
{
	t_list	*elem;

	if (line_is_blank(line))
		return ;
	elem = ft_lstnew(line);
	if (!elem)
	{
		ft_perror("add_to_history");
		return ;
	}
	ft_lstadd_front(history, elem);
}

int	clear_env_return(char **envp, int ret)
{
	ft_free_split(envp);
	return (ret);
}

// Ok, this is insane. I'll code a clear_mem_exit that handles every case.
int clear_env_history_return(char **envp, t_list **history, int ret)
{
	clear_env_return(envp, 0);
	ft_lstclear(history, free);
	return (ret);
}

// for now, only interactive mode.
int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_list		*commands_list;
	t_list		*history;
	t_termcaps	termcaps;
	t_shell_env	shell_env;

	setup_signal_handlers();
	if (setup_env(&shell_env, envp) == -1)
		return (1);
	history = NULL;
	if (init_termcaps(&termcaps) == -1)
		return (clear_env_return(shell_env.envp, 1));
	while (42)
	{
		if (ft_readline(&line, history, termcaps) == -1)
			return (clear_env_history_return(shell_env.envp, &history, 1));
		add_to_history(line, &history);
		parse_line(line, &commands_list); // lexer - parser

		shell_env.mem2clear->commands_list = commands_list;
		shell_env.mem2clear->history = history;

		execute_line(commands_list, &shell_env); // execute all pipelines
		clear_commands_list(&commands_list);
	}
	(void)argc;
	(void)argv;
}
