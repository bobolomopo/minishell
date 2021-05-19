#include "header.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_list		*commands_list;
	t_list		*history;
	t_termcaps	termcaps;
	char		**shell_env;

	setup_env(&shell_env, envp);

	// if interactive mode
	history = NULL;
	while (1)
	{
		init_termcaps(&termcaps);
		ft_readline(&line, &history, termcaps); // display prompt and readline

		if (ft_strcmp(line, "exit") == 0) // for testing
			break;
		
		parse_line(line, &commands_list); // lexer - parser- expander
		execute_line(commands_list, shell_env); // execute all pipelines
	
		// clear commands list: TODO

	}
	ft_lstclear(&history, free);
	ft_free_split(shell_env);

	(void)argc;
	(void)argv;
}

