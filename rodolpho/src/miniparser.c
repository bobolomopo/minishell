#include "header.h"

// parse line into list of commands
// according to bash definitions:

// a list of commands is a sequence of pipelines, separated by ";"
// a pipeline is a list of simple commands, separated by "|"
// a simple command is a command name and its arguments, and redirections

// THIS IS A MINI-PARSER, JUST FOR SOME TESTS OF THE EXECUTOR 
// It shall be replaced by the real parser.


// replace all "$" ocurrences by DOLLAR_SIGN
void	tag_dollar_sign(char **argv)
{
	char	*ptr;

	while (*argv)
	{
		ptr = *argv;
		while (*ptr)
		{
			if (*ptr == '$')
				*ptr = DOLLAR_SIGN;
			ptr++;
		}
		argv++;
	}
}

// split a command string into its separate words and store it in a t_command struct
t_command	*split_command(char *command)
{
	t_command	*simple_command;

	simple_command = malloc(sizeof(*simple_command));
	simple_command->argv = ft_split(command, ' ');
	simple_command->redirections = NULL;
	tag_dollar_sign(simple_command->argv);
	return (simple_command);
}

// split pipeline in a list of simple commands
t_list	*pipeline2list(char *pipeline)
{
	char	**commands;
	int		i;
	t_list	*elem;
	t_list	*lst;
	t_command *simple_command;

	lst = NULL;
	commands = ft_split(pipeline, '|');
	i = 0;
	while (commands[i])
	{
		simple_command = split_command(commands[i]);
		elem = ft_lstnew(simple_command);
		ft_lstadd_back(&lst, elem);
		i++;
	}
	ft_free_split(commands);
	return (lst);
}

// split line into pipelines and add them to list
int	parse_line(char *line, t_list **pipelines_lst)
{
	char	**array_of_pipelines;
	int		i;
	t_list	*pipeline;
	t_list	*new;

	*pipelines_lst = NULL;
	array_of_pipelines = ft_split(line, ';');
	i = 0;
	while (array_of_pipelines[i])
	{
		pipeline = pipeline2list(array_of_pipelines[i]);
		new = ft_lstnew(pipeline);
		ft_lstadd_back(pipelines_lst, new);	
		i++;
	}
	ft_free_split(array_of_pipelines);
	return (0);
}

