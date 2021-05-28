#include "header.h"

// this function is only here for testing/debugging
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

// int main(int argc, char **argv_main, char **envp)
// {
// 	t_command	command;
// 	t_list		*list;
// 	char *argv[3] = {"cat", "-e", NULL};
// 	t_redirection	red1 = {0, re_input, "infile"};
// 	t_redirection	red2 = {1, re_output, "outfile"};
// 	t_redirection	red3 = {1, re_output, "outfile1"};
// 	t_redirection	red4 = {1, re_output, "outfile2"};

// 	list = NULL;
// 	ft_lstadd_back(&list, ft_lstnew(&red1));
// 	ft_lstadd_back(&list, ft_lstnew(&red2));
// 	ft_lstadd_back(&list, ft_lstnew(&red3));
// 	ft_lstadd_back(&list, ft_lstnew(&red4));

// 	command.argv = argv;
// 	command.redirections = list;

// 	(void)argc;
// 	(void)argv_main;
// 	launch_command(&command, envp);
// 	return (0);
// }
