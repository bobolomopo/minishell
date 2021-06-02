/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clear_memory.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 17:54:31 by rcammaro          #+#    #+#             */
/*   Updated: 2021/06/02 17:54:32 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// frees memory:
// envp (array of strings)
// history (list of strings)
// pipelines_lst (list of lists of t_command)
void	clear_memory(t_sh_env *shell_env)
{
	ft_free_split(shell_env->envp);
	ft_lstclear(&shell_env->mem2clear.history, free);
	ft_lstclear(&shell_env->mem2clear.pipelines_lst, clear_pipeline);
}

int	clear_mem_exit(t_sh_env *shell_env, int exit_code)
{
	clear_memory(shell_env);
	exit (exit_code);
}
