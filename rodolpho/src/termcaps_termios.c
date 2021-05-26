#include "header.h"

static int	error(char *msg, int value)
{
	write(2, msg, ft_strlen(msg));
	return (value);
}

/*
Establishes a connection with the terminfo database and gets the termcaps
needed for ft_readline
Returns -1 if error.
*/
int	init_termcaps(t_termcaps *termcaps)
{
	char	*term_type;
	int		ret;

	term_type = getenv("TERM"); // not sure if I should use our env or the inherited one
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
	return (0);
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
