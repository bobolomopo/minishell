#include "header.h"

/*
if SIGINT (ctrl-c) is received, breakline
if it happened during the read() call in ft_readline, clear the buffer and
re-display the prompt. This is indicated via setting g_ptr to NULL or not.
*/
void	signal_handler_c(int signal_code)
{
	t_cmdline	*cmdl;

	ft_putchar_fd('\n', 1);
	if (g_ptr)
	{
		cmdl = (t_cmdline *)g_ptr;
		cmdl->index = 0;
		write(1, PROMPT, ft_strlen(PROMPT));
		cmdl->position = NULL;
		free(cmdl->backup_buffer);
		cmdl->backup_buffer = NULL;
	}
	(void)signal_code;
}

/*
if SIGQUIT (ctrl-backslash) is received, is does not affect the shell 
As this is not inherited by an execd process, this signal will terminate
the executed commands.
*/
void	sigquit_handler(int signal_code)
{
	(void)signal_code;
	ft_putendl_fd("Quit: 3", 1);
}

 /*
 Defines which signals will be handled.
 OBS: Not sure if this is needed when in non-interactive mode
 */
void	setup_signal_handlers(void)
{
	signal(SIGINT, signal_handler_c);
	signal(SIGQUIT, sigquit_handler);
}
