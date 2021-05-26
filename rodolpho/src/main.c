#include "header.h"

// for now, only interactive mode.
int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_list		*commands_list;
	t_list		*history;
	t_termcaps	termcaps;
	t_shell_env	shell_env;

	setup_env(&shell_env, envp);
	setup_signal_handlers();
	history = NULL;
	init_termcaps(&termcaps);
	while (1)
	{
		ft_readline(&line, &history, termcaps); // display prompt and readline
		if (!line)
			continue ;
		
		parse_line(line, &commands_list); // lexer - parser

		shell_env.mem2clear->commands_list = commands_list;
		shell_env.mem2clear->history = history;

		execute_line(commands_list, &shell_env); // execute all pipelines
		clear_commands_list(&commands_list);
	}
	(void)argc;
	(void)argv;
}
