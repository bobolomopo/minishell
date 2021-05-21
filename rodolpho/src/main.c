#include "header.h"


int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_list		*commands_list;
	t_list		*history;
	t_termcaps	termcaps;
	t_shell_env	shell_env;

	signal(SIGINT, signal_handler_c); // maybe only if in interactive mode?
	signal(SIGQUIT, sigquit_handler); // maybe only if in interactive mode?
	setup_env(&shell_env, envp);

	// if interactive mode
	history = NULL;
	init_termcaps(&termcaps);
	while (1)
	{
		ft_readline(&line, &history, termcaps); // display prompt and readline
		if (!line)
			continue ;

		if (ft_strcmp(line, "exit") == 0) // for testing
			break;
		
		parse_line(line, &commands_list); // lexer - parser- expander
		execute_line(commands_list, &shell_env); // execute all pipelines
		
		printf("$?: %hhu\n", shell_env.question_mark); // testing

		// clear commands list: TODO

	}
	ft_lstclear(&history, free);
	ft_free_split(shell_env.envp);

	(void)argc;
	(void)argv;
}

