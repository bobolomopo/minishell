#include "header.h"

void	clear_redirection(void *ptr)
{
	t_redirection	*redir;

	redir = ptr;
	free(redir->file);
	free(redir);
}

void	clear_command(void *ptr)
{
	t_command	*command;

	command = ptr;
	ft_free_split(command->argv);
	ft_lstclear(&command->redirections, clear_redirection);
	free(command);
}

void	clear_pipeline(void *ptr)
{
	t_list	*lst;

	lst = ptr;
	ft_lstclear(&lst, clear_command);
}

void	clear_commands_list(t_list **commands_list)
{
	ft_lstclear(commands_list, clear_pipeline);
}

// frees memory:
// envp (array of strings)
// history (list of strings)
// commands_list (list of lists of t_command)
void	clear_memory(t_shell_env *shell_env)
{
	ft_free_split(shell_env->envp);
	ft_lstclear(&shell_env->mem2clear->history, free);
	clear_commands_list(&shell_env->mem2clear->commands_list);
}
