#include "header.h"

char **copy_env(char **envp)
{
	int	i;
	int	n;
	char **shell_env;
	int	len;

	n = 0;
	while (envp[n])
		n++;
	shell_env = malloc(sizeof(*shell_env) * (n + 1));
	if (!shell_env)
		return (NULL);
	i = -1;
	while (++i < n)
	{
		len = ft_strlen(envp[i]);
		shell_env[i] = malloc(sizeof(char) * (len + 1));
		if (!shell_env[i])
			return (ft_free_split(shell_env));
		ft_strlcpy(shell_env[i], envp[i], len + 1);
	}
	shell_env[n] = NULL;
	return (shell_env);
}

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

// works for levels up to 9 TODO: make it work for bigger numbers...
void	increment_shlvl(char **env)
{
	char	*var;

	var = find_var(env, "SHLVL");
	if (!var)
		return ;
	var[6]++;
}

// makes a copy of the environment, increments the SHLVL variable and 
// sets the "?" special variable to 0
int	setup_env(t_shell_env *shell_env, char **envp)
{
	shell_env->envp = copy_env(envp);
	if (!shell_env->envp)
		return (-1);
	increment_shlvl(shell_env->envp);
	shell_env->question_mark = 0;
	return (0);
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
int	add_var(t_shell_env *shell_env,  char *new_var)
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

int	set_var(t_shell_env *shell_env, char *var_name, char *value)
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
