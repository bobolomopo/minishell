/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 15:15:12 by rcammaro          #+#    #+#             */
/*   Updated: 2021/06/03 15:15:13 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

int	line_is_blank(char *line)
{
	while (*line)
	{
		if (!ft_isspace(*line))
			return (0);
		line++;
	}
	return (1);
}

// adds the last command-line to the history
// upon error, message is displayed, but this error is non-fatal
// for the shell.
void	add_to_history(char *line, t_list **history)
{
	t_list	*elem;

	if (line_is_blank(line))
		return ;
	elem = ft_lstnew(line);
	if (!elem)
	{
		ft_perror("add_to_history");
		return ;
	}
	ft_lstadd_front(history, elem);
}

int	init_shell(char **envp, t_sh_env *shenv, t_list **history, t_tcaps *tc)
{
	setup_signal_handlers();
	if (init_termcaps(tc) == -1)
		return (-1);
	if (setup_env(shenv, envp) == -1)
		return (-1);
	*history = NULL;
	shenv->mem2clear.history = NULL;
	shenv->mem2clear.pipelines_lst = NULL;
	return (0);
}

// for now, only interactive mode.
int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_list		*pipelines_lst;
	t_list		*history;
	t_tcaps		termcaps;
	t_sh_env	shell_env;

	if (init_shell(envp, &shell_env, &history, &termcaps) == -1)
		return (1);
	while (42)
	{
		if (ft_readline(&line, history, termcaps) == -1)
			return (clear_mem_exit(&shell_env, 1));
		add_to_history(line, &history);
		shell_env.mem2clear.history = history;
		parse_line(line, &pipelines_lst);
		shell_env.mem2clear.pipelines_lst = pipelines_lst;
		execute_line(pipelines_lst, &shell_env);
		ft_lstclear(&pipelines_lst, clear_pipeline);
	}
	(void)argc;
	(void)argv;
}
