#include "header.h"

void ft_perror(char *func_name)
{
	int error_code;

	error_code = errno;
	if (func_name)
	{
		ft_putstr_fd(func_name, 2);
		ft_putstr_fd(": ", 2);
	}
	ft_putendl_fd(strerror(error_code), 2);
}

// prints error message based on errno and returns "return_value"
int	ft_perror_ret(char *func_name, int return_value)
{
	ft_perror(func_name);
	return (return_value);
}

int	clear_mem_exit(t_shell_env *shell_env, int exit_code)
{
	clear_memory(shell_env);
	exit (exit_code);
}