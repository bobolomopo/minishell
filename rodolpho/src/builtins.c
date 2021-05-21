#include "header.h"

int	builtin_env(char **argv, t_shell_env *shell_env)
{
	int		i;
	char	**envp;

	(void)argv;
	envp = shell_env->envp;
	i = 0;
	while (envp[i]) 
		ft_putendl_fd(envp[i++], 1);
	return (0);
}

int	builtin_echo(char **argv, t_shell_env *shell_env)
{
	int	n_flag;
	int	i;

	(void)shell_env;
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
int	builtin_pwd(char **argv, t_shell_env *shell_env)
{
	char	*pwd;

	(void)argv;
	(void)shell_env;
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

// returns pointer to var's first char after "=", or NULL if var not found
// no memory allocation
char	*expand_var(char **envp,char *var)
{
	int		name_len;
	char	*var_location;

	var_location = find_var(envp, var);
	if (!var_location)
		return (NULL);
	name_len = ft_strlen(var);
	return (var_location + name_len + 1);
}

int	builtin_cd(char **argv, t_shell_env *shell_env)
{
	char	*path;
	char	*pwd;

	path = argv[1];
	if (!path)
	{
		path = expand_var(shell_env->envp, "HOME");
		if (!path)
		{
			ft_putendl_fd("minishell: cd: HOME not set", 2);
			return (1);
		}
	}
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_perror(path);
		return (1);
	}
	pwd = getcwd(NULL, 0); // TODO error handling
	set_var(shell_env, "PWD", pwd);
	free(pwd);
	return (0);
}
