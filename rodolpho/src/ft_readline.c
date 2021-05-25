#include "header.h"
#include <termios.h>
#include <curses.h>
#include <term.h>

int	error(char *msg, int value)
{
	write(2, msg, ft_strlen(msg));
	return (value);
}

int ft_putchar_stdout(int c)
{
	return (write(1, &c, 1));
}

int	init_termcaps(t_termcaps *termcaps)
{
	char	*term_type;
	int		ret;

	term_type = getenv("TERM");
	if (term_type == NULL)
		return (error("TERM environmental variable must be set\n", -1));
	ret = tgetent(NULL, term_type);
	if (ret == 0)
		return (error("Terminal type not found in terminfo database\n", -1));
	if (ret == -1)
		return (error("Terminfo database not found\n", -1));
	termcaps->mv_cursor_col1 = tgetstr("cr", NULL);
	termcaps->cl_line = tgetstr("cd", NULL);
	termcaps->mv_cursor_left = tgetstr("le", NULL);
	termcaps->cl_to_endline = tgetstr("ce", NULL);
	// termcaps->del_char = tgetstr("dc", NULL);
	// termcaps->enter_del_mode = tgetstr("dm", NULL);
	// termcaps->exit_del_mode = tgetstr("ed", NULL);
	return (1);
}

// makes a backup of the current terminal settings and then
// puts terminal in non canonical mode with disabled echo
// returns -1 if error
int	setup_terminal(struct termios *termios_p_backup)
{
	int				ret;
	struct termios	termios_p;

	ret = tcgetattr(0, termios_p_backup);
	if (ret == -1)
		return (-1);
	ret = tcgetattr(0, &termios_p);
	if (ret == -1)
		return (-1);
	termios_p.c_lflag &= ~(ICANON); // put terminal in non canonical mode
	termios_p.c_lflag &= ~(ECHO);	// disable echo
	ret = tcsetattr(0, 0, &termios_p);
	if (ret == -1)
		return (-1);
	return (0);
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
	tputs(termcaps.mv_cursor_col1, 1, ft_putchar_stdout);
	tputs(termcaps.cl_line, 1, ft_putchar_stdout);
	write(1, prompt, ft_strlen(prompt));
	write(1, text_buffer, ft_strlen(text_buffer));
}

void	delete_last_char(char *text_buffer, t_termcaps termcaps)
{
	if (*text_buffer == '\0') // if empty buffer
		return ;
	tputs(termcaps.mv_cursor_left, 1, ft_putchar_stdout); // move cursor left
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
			if (ft_isprint(key))
			{
				write(1, &key, 1); // echo to screen
				add_to_buffer(key, cmdline.text_buffer);
			}
			else if (key == '\n') // return key
			{
				write(1, &key, 1);
				break ;	
			}
		}
	}
	*line = ft_strdup(cmdline.text_buffer);
	add_to_history(*line, history);
	
	tcsetattr(0, 0, &termios_p_backup); // recover terminal settings

	g_ptr = NULL;
	return (0);
}
