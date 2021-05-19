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

// returns pointer to SHLVL variable, NULL if not found
char	*find_shlvl(char **env)
{
	int	i;

	i = -1;
	while (env[++i])
	{
		if (ft_strncmp(env[i], "SHLVL=", 6) == 0)
			break ;
	}
	return (env[i]);
}

// works for levels up to 9
void	increment_shlvl(char **env)
{
	char	*var;

	var = find_shlvl(env);
	if (!var)
		return ;
	var[6]++;
}

// makes a copy of the environment and increments the SHLVL variable
int	setup_env(char ***shell_env, char **envp)
{
	*shell_env = copy_env(envp);
	if (!*shell_env)
		return (-1);
	increment_shlvl(*shell_env);
	return (0);
}
