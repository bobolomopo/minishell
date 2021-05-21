#include "header.h"

int	builtin_env(char **argv, char **envp)
{
	int	i;

	(void)argv;
	i = 0;
	while (envp[i]) 
		ft_putendl_fd(envp[i++], 1);
	return (0);
}

int	builtin_echo(char **argv, char **envp)
{
	int	n_flag;
	int	i;

	(void)envp;
	n_flag = 0;
	if (argv[1] && ft_strcmp(argv[1], "-n") == 0)
		n_flag = 1;
	i = 1 + n_flag;
	while (argv[i])
	{
		if (i > 1 + n_flag)
			ft_putchar_fd(' ', 1);
		ft_putstr_fd(argv[i], 1);
		i++;
	}
	if (!n_flag)
		ft_putchar_fd('\n', 1);
	return (0);
}

// it does not rely on PWD env variable (like bash also does not)
int	builtin_pwd(char **argv, char **envp)
{
	char	*pwd;

	(void)argv;
	(void)envp;
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		ft_perror("pwd");
		return (1);
	}
	ft_putendl_fd(pwd, 1);
	free(pwd);
	return (0);
}
