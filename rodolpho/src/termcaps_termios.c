/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   termcaps_termios.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 15:46:45 by rcammaro          #+#    #+#             */
/*   Updated: 2021/06/03 15:46:46 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// prints msg to stdout and returns value
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
int	init_termcaps(t_tcaps *termcaps)
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
	termcaps->cl_line = tgetstr("cd", NULL);
	termcaps->cl_to_endline = tgetstr("ce", NULL);
	return (0);
}

// makes a backup of the current terminal settings and then
// puts terminal in non canonical mode and disables echo
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
	termios_p.c_lflag &= ~(ICANON);
	termios_p.c_lflag &= ~(ECHO);
	ret = tcsetattr(0, 0, &termios_p);
	if (ret == -1)
		return (-1);
	return (0);
}

// recover terminal settings.
// if error, display error msg.
void	reset_terminal(struct termios *termios_p_backup)
{
	if (tcsetattr(0, 0, termios_p_backup) == -1)
		ft_perror("tcsetattr");
}
