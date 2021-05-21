#include "header.h"

// if SIGINT (ctrl-c) is received, breakline
// if it happened during the read() call in ft_readline, clear the buffer and
// re-display the prompt. This is indicated via setting g_ptr to NULL or not.
void	signal_handler_c(int signal_code)
{
	t_cmdline	*cmdl;

	ft_putchar_fd('\n', 1);
	if (g_ptr)
	{
		cmdl = (t_cmdline *)g_ptr;
		cmdl->text_buffer[0] = '\0';
		write(1, cmdl->prompt, ft_strlen(cmdl->prompt));
		cmdl->position = NULL;
		free(cmdl->backup_buffer);
		cmdl->backup_buffer = NULL;
	}
	(void)signal_code;
}

void	sigquit_handler(int signal_code)
{
	// doing nothing, for now
	(void)signal_code;
}