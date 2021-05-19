#include "header.h"

// for testing/debugging
void print_list_commands(t_list *list_of_pipelines)
{
	t_command *command_struc;
	t_list *list_of_simple_commands;
	int i;

	while (list_of_pipelines)
	{
		list_of_simple_commands = list_of_pipelines->content;
		printf("pipeline:\n");
		while (list_of_simple_commands)
		{
			command_struc = list_of_simple_commands->content;
			i = 0;
			printf("    command: ");
			while (command_struc->argv[i])
				printf("%s ", command_struc->argv[i++]);
			printf("\n");
			list_of_simple_commands = list_of_simple_commands->next;
		}
		list_of_pipelines = list_of_pipelines->next;
	}	
}

// lst is a list of pipelines
int	execute_line(t_list *lst, char **envp)
{
	t_list	*pipeline;
	int		n_commands;

	while (lst)
	{
		pipeline = lst->content;
		n_commands = ft_lstsize(pipeline);
		if (n_commands == 0)
			return (0);
		if (n_commands == 1)
			launch_command(pipeline->content, envp);
		else
			run_pipeline(pipeline, envp, n_commands);
		lst = lst->next;
	}
	return (0);
}
