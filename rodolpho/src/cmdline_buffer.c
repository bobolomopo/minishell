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
Echoes key back to the screen and adds it to buffer.
Returns -1 if error, 1 otherwise (caller expects that).
*/
int	add_char(int key, t_cmdline *cmdline)
{
	if (cmdline->index == cmdline->size - 1)
		if (grow_buffer(cmdline) == -1)
			return (-1);
	write(1, &key, 1); // echo to screen
	cmdline->buffer[cmdline->index] = (char)key;
	cmdline->index++;
	return (1);
}

// this prototype is a requirement for tputs 
int ft_putchar_stdout(int c)
{
	return (write(1, &c, 1));
}

// always returns 1 (caller expects that)
int	delete_last_char(t_cmdline *cmdline, t_tcaps termcaps)
{
	if (cmdline->index == 0)
		return (1);

	// tputs(termcaps.mv_cursor_left, 1, ft_putchar_stdout); // move cursor left - MacOS version
	tputs("\b", 1, ft_putchar_stdout); // move cursor left - LINUX version
	tputs(termcaps.cl_to_endline, 1, ft_putchar_stdout); // clear to end of line

	cmdline->index--;
	return (1);
}

void	refresh_display(t_cmdline *cmdline, t_tcaps termcaps)
{
	tputs("\r", 1, ft_putchar_stdout);			// linux version
	// tputs(termcaps.mv_cursor_col1, 1, ft_putchar_stdout); // mac version
	tputs(termcaps.cl_line, 1, ft_putchar_stdout);
	write(1, PROMPT, ft_strlen(PROMPT));
	write(1, cmdline->buffer, cmdline->index);
}
