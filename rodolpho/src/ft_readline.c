#include "header.h"

/*
Reads a key press from the terminal.
Escape sequences:
Arrow up: '\033' '[' 'A'
Arrow down: '\033' '[' 'B'
*/
int	detect_key(void)
{
	char	buffer[2];
	int		ret;

	ret = read(0, buffer, 1);
	if (ret == -1)
		return (ft_perror_ret("read", -1));
	if (buffer[0] == '\033')
	{
		ret = read(0, buffer, 2);
		if (ret == -1)
			return (ft_perror_ret("read", -1));
		if (buffer[0] == '[')
		{
			if (buffer[1] == 'A')
				return (ARROW_UP);
			if (buffer[1] == 'B')
				return (ARROW_DOWN);
		}
		return (0);
	}
	return (buffer[0]);
}

int	enter_key(t_cmdline *cmdline)
{
	write(1, "\n", 1);
	cmdline->buffer[cmdline->index] = '\0';
	return (0);
}

int	ctrl_d(t_cmdline *cmdline)
{
	ft_putendl_fd("exit", 1);
	ft_strlcpy(cmdline->buffer, "exit", cmdline->size);
	return (0);
}

// return 1 if key read successfully and input is not over
// 0 if finished (enter key or ctrl-d)
// -1 if error
int	read_key_and_process(t_cmdline *cmdline, t_list *history, t_tcaps tc)
{
	int	key;

	key = detect_key();
	if (key == -1)
		return (-1);
	if (ft_isprint(key))
		return (add_char(key, cmdline));
	if (key == BACKSPACE)
		return (delete_last_char(cmdline, tc));
	if (key == ENTER_KEY)
		return (enter_key(cmdline));
	if (key == CTRL_D && cmdline->index == 0)
		return (ctrl_d(cmdline));
	if (key == ARROW_DOWN)
		return (arrow_down(cmdline, history, tc));
	if (key == ARROW_UP)
		return (arrow_up(cmdline, history, tc));
	return (1);
}

/*
Reads input from the terminal, allowing navigation in history.
Upon success, line is a freeable string and 0 is returned.
Upon error, -1 is returned and line is NULL.
Terminal is put in non-canonical mode and echo is disabled, to allow
read() to get each key stroke. Terminal settings are recovered before
exiting.
*/
int	ft_readline(char **line, t_list *history, t_tcaps termcaps)
{
	struct termios	termios_p_backup;
	t_cmdline		cmdline;
	int				ret;

	*line = NULL;
	if (setup_terminal(&termios_p_backup) == -1)
		return (-1);
	if (initialize_cmdline(&cmdline, history) == -1)
		return (-1);
	ret = 1;
	while (ret == 1)
		ret = read_key_and_process(&cmdline, history, termcaps);
	if (ret == -1)
		free(cmdline.buffer);
	else
		*line = cmdline.buffer;
	free(cmdline.backup_buffer);
	g_ptr = NULL;
	tcsetattr(0, 0, &termios_p_backup);
	return (ret);
}
