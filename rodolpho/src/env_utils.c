#include "header.h"

// returns pointer to var variable, NULL if not found
char	*find_var(char **env, char *var)
{
	int	i;
	int	name_len;

	name_len = ft_strlen(var);
	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], var, name_len) == 0
			&& env[i][name_len] == '=')
		return (env[i]);
	}
	return (NULL);
}

// returns of index of the envp array in which var can be found,
// or -1 if not found.
// considering replacing find_var for this one??
int	find_var_index(char **envp, char *var)
{
	int	i;
	int	name_len;

	name_len = ft_strlen(var);
	i = -1;
	while (envp[++i])
	{
		if (ft_strncmp(envp[i], var, name_len) == 0
			&& envp[i][name_len] == '=')
		return (i);
	}
	return (-1);
}

int	count_vars(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
		i++;
	return (i);
}

// new_var: pointer to string VAR=VALUE
int	add_var(t_sh_env *shell_env,  char *new_var)
{
	char	**new_envp;
	int		n;
	int		i;

	n = count_vars(shell_env->envp);
	new_envp = malloc(sizeof(*new_envp) * (n + 2));
	if (!new_envp)
		return (-1);
	i = -1;
	while (++i < n)
		new_envp[i] = shell_env->envp[i];
	new_envp[n] = new_var;
	new_envp[n + 1] = NULL;
	free(shell_env->envp);
	shell_env->envp = new_envp;
	return (0);
}

void	update_var(char **envp, int index, char *new_var)
{
	free(envp[index]);
	envp[index] = new_var;
}

// adds var_name (if non-existant) or updates var_name's value in the environment.
// returns 0 upon success, -1 if error (TODO...)
int	set_var(t_sh_env *shell_env, char *var_name, char *value)
{
	char	*new_var;
	int		size;
	int		index;

	size = ft_strlen(var_name) + ft_strlen(value) + 2; // why 2: '=' + '\0'
	new_var = malloc(size); 
	if (!new_var)
		return (-1); // error
	ft_strlcpy(new_var, var_name, size);
	ft_strlcat(new_var, "=", size);
	ft_strlcat(new_var, value, size);

	index = find_var_index(shell_env->envp, var_name);
	if (index == -1)  // not found
		add_var(shell_env, new_var); // TODO error handling
	else
		update_var(shell_env->envp, index, new_var);
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

// if var_name exists in envp, the string is freed and all contents to its right
// are shift 1 position to the left (so, no reallocation)
void	remove_var(char *var_name, t_sh_env *shell_env)
{
	int		i;
	char	**envp;

	envp = shell_env->envp;
	i = find_var_index(envp, var_name);
	if (i == -1)
		return ;
	free(envp[i]);
	while (envp[i])	
	{
		envp[i] = envp[i + 1];
		i++;
	}
}
