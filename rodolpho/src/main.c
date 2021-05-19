#include "header.h"

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_list		*commands_list;
	t_list		*history;
	t_termcaps	termcaps;

	// if interactive mode
	history = NULL;
	while (1)
	{
		init_termcaps(&termcaps);
		ft_readline(&line, &history, termcaps); // display prompt and readline
		// parse_line(line, &commands_list); // lexer - parser- expander
		// free(line);
		// ft_lstclear(&commands_list, NULL);
		// execute_line(); // execute all pipelines


		if (ft_strcmp(line, "exit") == 0) // for testing
			break;
	}
	ft_lstclear(&history, free);

	(void)commands_list;
	(void)argc;
	(void)argv;
	(void)envp;
}

