#include "header.h"

// function for testing
int test_pipe(char **envp)
{
	t_command	command1;
	t_list		*redir_comm1;
	char *argv1[4] = {"cat", "infile", "non_exist_file", NULL};
	t_redirection red_cat = {2, re_output_append, "outfile_errors"};
	redir_comm1 = NULL;
	ft_lstadd_back(&redir_comm1, ft_lstnew(&red_cat));
	command1.argv = argv1;
	command1.redirections = redir_comm1;

	t_command	command2;
	t_list		*redir_comm2;
	char *argv2[3] = {"wc", NULL};
	redir_comm2 = NULL;
	command2.argv = argv2;
	command2.redirections = redir_comm2;

	t_command	command3;
	t_list		*redir_comm3;
	char *argv3[4] = {"cat", "-e", NULL};
	t_redirection red_cat2 = {1, re_output, "outfile"};
	redir_comm3 = NULL;
	ft_lstadd_back(&redir_comm3, ft_lstnew(&red_cat2));
	command3.argv = argv3;
	command3.redirections = redir_comm3;

	t_list *commands_list;

	commands_list = NULL;
	ft_lstadd_back(&commands_list, ft_lstnew(&command1));
	ft_lstadd_back(&commands_list, ft_lstnew(&command2));
	ft_lstadd_back(&commands_list, ft_lstnew(&command3));

	printf("The following command is being tested:\n");
	printf("cat infile non_exist_file 2>> outfile_errors | wc | cat -e > outfile\n");

	run_pipeline(commands_list, envp, ft_lstsize(commands_list));
	return (0);
}

int	main(int argc, char **argv, char **envp)
{
	test_pipe(envp);

	(void)argc;
	(void)argv;

	return (0);
}
