#include "header.h"

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
