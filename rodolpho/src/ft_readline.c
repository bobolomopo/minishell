#include "header.h"

// this prototype is a requirement for tputs 
int ft_putchar_stdout(int c)
{
	return (write(1, &c, 1));
}

int	detect_key(void)
{
	char	buffer[2];

	read(0, buffer, 1);

	if (buffer[0] == '\033') // detecting ARROWS UP/DOWN key presses
	{
		read(0, buffer, 2);
		if (buffer[0] == '[')
		{
			if (buffer[1] == 'A')
				return (ARROW_UP);
			else if (buffer[1] == 'B')
				return (ARROW_DOWN);
		}
		return (0);
	}

	// int key = buffer[0];
	// printf("key value: %d\n", key);
	
	return (buffer[0]); // check if printable character?
}

void	add_to_buffer(int key, char *text_buffer)
{
	while (*text_buffer)
		text_buffer++;
	*text_buffer = (char)key;
	text_buffer++;
	*text_buffer = '\0';
}

void	refresh_display(char *prompt, char *text_buffer, t_termcaps termcaps)
{
	// fucking termcaps bug
	char *mv_cursor_1st_col = "\r";

	tputs(mv_cursor_1st_col, 1, ft_putchar_stdout);			// linux version
	// tputs(termcaps.mv_cursor_col1, 1, ft_putchar_stdout); // mac version
	tputs(termcaps.cl_line, 1, ft_putchar_stdout);
	write(1, prompt, ft_strlen(prompt));
	write(1, text_buffer, ft_strlen(text_buffer));
}

void	delete_last_char(char *text_buffer, t_termcaps termcaps)
{
	if (*text_buffer == '\0') // if empty buffer
		return ;
	// tputs(termcaps.mv_cursor_left, 1, ft_putchar_stdout); // move cursor left - MacOS version
	tputs("\b", 1, ft_putchar_stdout); // move cursor left - LINUX version
	tputs(termcaps.cl_to_endline, 1, ft_putchar_stdout); // clear to end of line
	// delete also from buffer
	while (*text_buffer)
		text_buffer++;
	text_buffer--;
	*text_buffer = '\0';

}

int	add_to_history(char *line, t_list **history)
{
	t_list	*elem;

	elem = ft_lstnew(line);
	if (!elem)
		return (-1);
	ft_lstadd_front(history, elem);
	return (0);
}

void	initialize_cmdline(t_cmdline *cmdline)
{
	cmdline->text_buffer[0] = '\0';
	cmdline->backup_buffer = NULL;
	ft_strlcpy(cmdline->prompt, "prompt > ", 20); 
	cmdline->position = NULL;
}

int	ft_readline(char **line, t_list **history, t_termcaps termcaps)
{
	struct termios	termios_p_backup;
	int				key;
	t_cmdline		cmdline;

	if (setup_terminal(&termios_p_backup) == -1)
		return (-1);
	initialize_cmdline(&cmdline);
	g_ptr = &cmdline; // the signalhandler has access to cmdline via the global variable

	write(1, cmdline.prompt, ft_strlen(cmdline.prompt));
	while (1)
	{
		key = detect_key();
		if (key == 127) // backspace key
		{
			delete_last_char(cmdline.text_buffer, termcaps);
			continue ;
		}
		if (key == 4) // ctrl-d
		{
			if (cmdline.text_buffer[0]) // if buffer not empty
				continue ;
			ft_putendl_fd("exit", 1);
			ft_strlcpy(cmdline.text_buffer, "exit", 100);
			break ;
		}
		if (key == ARROW_UP || key == ARROW_DOWN)
		{
			if (key == ARROW_UP)
				arrow_up(cmdline.text_buffer, &cmdline.backup_buffer, *history, &cmdline.position);
			else
				arrow_down(cmdline.text_buffer, &cmdline.backup_buffer, *history, &cmdline.position);
			refresh_display(cmdline.prompt, cmdline.text_buffer, termcaps);
		}
		else
		{
			if (key == '\n') // return key
			{
				write(1, "\n", 1); // break line and exit loop
				break ;	
			}
			if (ft_isprint(key))
			{
				write(1, &key, 1); // echo to screen
				add_to_buffer(key, cmdline.text_buffer);
			}
		}
	}
	*line = ft_strdup(cmdline.text_buffer);
	add_to_history(*line, history);
	
	tcsetattr(0, 0, &termios_p_backup); // recover terminal settings

	g_ptr = NULL;
	return (0);
}
