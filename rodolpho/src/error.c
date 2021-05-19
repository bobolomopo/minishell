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