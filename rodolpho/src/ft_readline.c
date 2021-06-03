/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_readline.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 15:13:05 by rcammaro          #+#    #+#             */
/*   Updated: 2021/06/03 15:13:06 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

/*
Reads a key press from the terminal.
If single byte is read, returns its value.
If the 3-byte sequences 27 91 65 or 27 91 66 are read, returns ARROW_UP /
ARROW_DOWN, respectively.
Otherwise, returns 0, or -1 if error.
*/
int	detect_key(void)
{
	char	buffer[6];
	int		ret;

	ret = read(0, buffer, 6);
	if (ret == -1)
		return (ft_perror_ret("read", -1));
	if (ret == 1)
		return (buffer[0]);
	if (ret == 3 && buffer[0] == 27 && buffer[1] == 91)
	{
		if (buffer[2] == 65)
			return (ARROW_UP);
		if (buffer[2] == 66)
			return (ARROW_DOWN);
	}
	return (0);
}

// returns 0
int	enter_key(t_cmdline *cmdline)
{
	write(1, "\n", 1);
	cmdline->buffer[cmdline->index] = '\0';
	return (0);
}

// returns 0
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
	if (key == ARROW_UP)
		return (arrow_up(cmdline, history, tc));
	if (key == ARROW_DOWN)
		return (arrow_down(cmdline, history, tc));
	if (key == ENTER_KEY)
		return (enter_key(cmdline));
	if (key == CTRL_D && cmdline->index == 0)
		return (ctrl_d(cmdline));
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
	close_cmdline(&cmdline);
	reset_terminal(&termios_p_backup);
	return (ret);
}
