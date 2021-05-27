#include "header.h"

int	builtin_env(char **argv, t_sh_env *shell_env)
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

int	builtin_echo(char **argv, t_sh_env *shell_env)
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
int	builtin_pwd(char **argv, t_sh_env *shell_env)
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

int	builtin_cd(char **argv, t_sh_env *shell_env)
{
	char	*path;
	char	*pwd;
	char	*old_pwd;

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
	old_pwd = getcwd(NULL, 0);
	if (!old_pwd)
		ft_perror("cd"); // non-fatal error
	if (chdir(path) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_perror(path);
		return (1);
	}
	if (old_pwd)
		set_var(shell_env, "OLDPWD", old_pwd); // todo: error handling
	free(old_pwd);
	pwd = getcwd(NULL, 0);
	set_var(shell_env, "PWD", pwd); // error handling :(
	free(pwd);
	return (0);
}

static int	is_valid_name(char *var)
{
	if (!(ft_isalpha(*var) || *var == '_'))
		return (0);
	var++;
	while (*var && *var != '=')
	{
		if (!(ft_isalnum(*var) || *var == '_'))
			return (0);
		var++;
	}
	return (1);
}

static void	print_error_msg_name(char *arg, char *builtin)
{
	ft_putstr_fd("Minishell: ", 2);
	ft_putstr_fd(builtin, 2);
	ft_putstr_fd(": `", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd("': not a valid identifier\n", 2);
}

int	builtin_export(char **argv, t_sh_env *shell_env)
{
	int		i;
	char	*var_name;
	char	*var_value;
	char	*ptr;
	int		return_value;

	if (!argv[1])
	{
		ft_putendl_fd("TODO: weird list of vars", 1); // TODO
		return (0);
	}
	return_value = 0;
	i = 0;
	while (argv[++i])
	{
		if (!is_valid_name(argv[i]))
		{
			print_error_msg_name(argv[i], "export");
			return_value = 1;
			continue ;
		}
		if (!ft_strchr(argv[i], '=')) // for now, only name=value will take effect
			continue ;
		ptr = argv[i];
		var_name = parse_var_name(&ptr); // TODO error handling
		var_value = ptr + 1;
		set_var(shell_env, var_name, var_value); // TODO error handling
		free(var_name);
	}
	return (return_value);
}

int	builtin_unset(char **argv, t_sh_env *shell_env)
{
	int	i;
	int	ret_value;

	i = 0;
	ret_value = 0;
	while (argv[++i])
	{
		if (is_valid_name(argv[i]))
			remove_var(argv[i], shell_env);
		else
		{
			print_error_msg_name(argv[i], "unset");
			ret_value = 1;
		}
	}
	return (ret_value);
}
