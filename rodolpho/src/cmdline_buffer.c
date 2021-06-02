/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdline_buffer.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/02 17:59:46 by rcammaro          #+#    #+#             */
/*   Updated: 2021/06/02 17:59:47 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

static int	get_max_len(t_list *history)
{
	int	max_len;
	int	len;

	max_len = 0;
	while (history)
	{
		len = ft_strlen(history->content);
		if (len > max_len)
			max_len = len;
		history = history->next;
	}
	return (max_len);
}

/* Allocates memory for the buffer, size big enough to hold any of the contents
in the history.
Assignates the address of cmdline to the global variable g_ptr
(signalhandler needs it).
Displays the prompt.
*/
int	initialize_cmdline(t_cmdline *cmdline, t_list *history)
{
	int	max_len_history;

	max_len_history = get_max_len(history);
	cmdline->size = BUFFER_STEP;
	if (max_len_history + 1 > BUFFER_STEP)
		cmdline->size = max_len_history + 1;
	cmdline->buffer = malloc(cmdline->size);
	if (!cmdline->buffer)
		return (-1);
	cmdline->index = 0;
	cmdline->backup_buffer = NULL;
	cmdline->position = NULL;
	g_ptr = cmdline;
	write(1, PROMPT, ft_strlen(PROMPT));
	return (0);
}

void	close_cmdline(t_cmdline *cmdline)
{
	free(cmdline->backup_buffer);
	g_ptr = NULL;
}

static int	grow_buffer(t_cmdline *cmdline)
{
	char	*new_buffer;
	int		new_size;

	new_size = cmdline->size + BUFFER_STEP;
	new_buffer = malloc(new_size);
	if (!new_buffer)
		return (-1);
	ft_memcpy(new_buffer, cmdline->buffer, cmdline->size);
	free(cmdline->buffer);
	cmdline->buffer = new_buffer;
	cmdline->size = new_size;
	return (0);
}

/*
Echoes key back to the screen and adds it to buffer (it might call grow_buffer
if more space is needed).
Returns -1 if error, 1 otherwise (caller expects that).
*/
int	add_char(int key, t_cmdline *cmdline)
{
	if (cmdline->index == cmdline->size - 1)
		if (grow_buffer(cmdline) == -1)
			return (-1);
	write(1, &key, 1);
	cmdline->buffer[cmdline->index] = (char)key;
	cmdline->index++;
	return (1);
}
