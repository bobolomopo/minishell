/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   navigate_history.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 15:21:52 by rcammaro          #+#    #+#             */
/*   Updated: 2021/06/03 15:21:53 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// returns pointer to list element before elem
// return lst if elem is the first item
// return NULL if elem not in the list
t_list	*ft_lst_prev_item(t_list *lst, t_list *elem)
{
	if (elem == lst)
		return (elem);
	while (lst)
	{
		if (lst->next == elem)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

int	backup_buffer(t_cmdline *cmdline)
{
	cmdline->buffer[cmdline->index] = '\0';
	cmdline->backup_buffer = ft_strdup(cmdline->buffer);
	if (!cmdline->backup_buffer)
	{
		ft_perror("ft_strdup");
		return (-1);
	}
	return (0);
}

void	restore_buffer(t_cmdline *cmdline)
{
	ft_strlcpy(cmdline->buffer, cmdline->backup_buffer, cmdline->size);
	cmdline->index = ft_strlen(cmdline->buffer);
	free(cmdline->backup_buffer);
	cmdline->backup_buffer = NULL;
}

/*
cmdline->position: which item of the history if being displayed in the
commandline (pointer to list).
NULL: none, new command being typed
*/

/*
returns -1 if error, 1 otherwise (caller expects that)
Does nothing if history is empty or if position is already at its last element
If moving from the new command to the first item in history, the current buffer
if copied to a backup.
*/
int	arrow_up(t_cmdline *cmdline, t_list *history, t_tcaps termcaps)
{
	if (history == NULL)
		return (1);
	if (cmdline->position && cmdline->position->next == NULL)
		return (1);
	if (cmdline->position == NULL)
	{
		if (backup_buffer(cmdline) == -1)
			return (-1);
		cmdline->position = history;
	}
	else
		cmdline->position = cmdline->position->next;
	ft_strlcpy(cmdline->buffer, cmdline->position->content, cmdline->size);
	cmdline->index = ft_strlen(cmdline->buffer);
	refresh_display(cmdline, termcaps);
	return (1);
}

// always returns 1 (caller expects that)
// Does nothing if position if the current command.
// If moving from first (most recent) item in history to current command,
// the buffer is restored form its backup.
int	arrow_down(t_cmdline *cmdline, t_list *history, t_tcaps termcaps)
{
	if (cmdline->position == NULL)
		return (1);
	if (cmdline->position == history)
	{
		restore_buffer(cmdline);
		refresh_display(cmdline, termcaps);
		cmdline->position = NULL;
		return (1);
	}
	cmdline->position = ft_lst_prev_item(history, cmdline->position);
	ft_strlcpy(cmdline->buffer, cmdline->position->content, cmdline->size);
	cmdline->index = ft_strlen(cmdline->buffer);
	refresh_display(cmdline, termcaps);
	return (1);
}
