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

//  [n]<file [n]>file [n]>>file
t_redirection	*parse_redirection(char *arg)
{
	char	*file;
	int		op;
	int		n;
	t_redirection	*red;
	
	if (*arg == '<')
		n = 0;
	else if (*arg == '>')
		n = 1;
	else
		n = ft_atoi_mv_ptr(&arg);

	if (*arg == '<')
		op = re_input;
	else
	{
		if (*(arg + 1) == '>')
			op = re_output_append;
		else
			op = re_output;
	}
	while (*arg == '<' || *arg == '>')
		arg++;
	file = ft_strdup(arg);
	red = malloc(sizeof(*red));
	red->n = n;
	red->op = op;
	red->file = file;
	return (red);
}

void	remove_arg(char **argv)
{
	int	i;

	i = 0;
	free(argv[i]);
	while (argv[i])
	{
		argv[i] = argv[i + 1];
		i++;
	}
}

t_list	*get_redirections(t_command *command)
{
	char	**argv;
	int		i;
	t_redirection	*red;
	t_list	*redirections_lst;

	redirections_lst = NULL;
	argv = command->argv;
	i = 0;
	while (argv[i])
	{
		if (ft_strchr(argv[i], '<') || ft_strchr(argv[i], '>'))
		{
			red = parse_redirection(argv[i]);
			ft_lstadd_back(&redirections_lst, ft_lstnew(red));
			if (*(red->file) == '\0')
			{
				free(red->file);
				red->file = ft_strdup(argv[i + 1]);
				remove_arg(&argv[i + 1]);
			}
			remove_arg(&argv[i--]);
		}
		i++;
	}
	return (redirections_lst);
}

// split a command string into its separate words and store it in a t_command struct
t_command	*split_command(char *command)
{
	t_command	*simple_command;

	simple_command = malloc(sizeof(*simple_command));
	simple_command->argv = ft_split(command, ' ');
	simple_command->redirections = get_redirections(simple_command);
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

