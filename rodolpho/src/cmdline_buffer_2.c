#include "header.h"

// this prototype is a requirement for tputs 
static int	ft_putchar_stdout(int c)
{
	return (write(1, &c, 1));
}

// always returns 1 (caller expects that)
int	delete_last_char(t_cmdline *cmdline, t_tcaps termcaps)
{
	if (cmdline->index == 0)
		return (1);
	write(1, "\b", 1);
	tputs(termcaps.cl_to_endline, 1, ft_putchar_stdout);
	cmdline->index--;
	return (1);
}

void	refresh_display(t_cmdline *cmdline, t_tcaps termcaps)
{
	write(1, "\r", 1);
	tputs(termcaps.cl_line, 1, ft_putchar_stdout);
	write(1, PROMPT, ft_strlen(PROMPT));
	write(1, cmdline->buffer, cmdline->index);
}
