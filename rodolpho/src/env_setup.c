#include "header.h"

char	**env_copy_error(char **envp)
{
	ft_perror("copy_env");
	if (envp)
		ft_free_split(envp);
	return (NULL);
}

char	**copy_env(char **envp)
{
	int	i;
	int	n;
	char **shell_envp;
	int	len;

	n = 0;
	while (envp[n])
		n++;
	shell_envp = malloc(sizeof(*shell_envp) * (n + 1));
	if (!shell_envp)
		return (env_copy_error(shell_envp));
	i = -1;
	while (++i < n)
	{
		len = ft_strlen(envp[i]);
		shell_envp[i] = malloc(sizeof(char) * (len + 1));
		if (!shell_envp[i])
			return (env_copy_error(shell_envp));
		ft_strlcpy(shell_envp[i], envp[i], len + 1);
	}
	shell_envp[n] = NULL;
	return (shell_envp);
}

// works for levels up to 9
// TODO: make it work for bigger numbers...
void	increment_shlvl(char **env)
{
	char	*var;

	var = find_var(env, "SHLVL");
	if (!var)
		return ;
	var[6]++;
}

/*
makes a copy of the environment
increments the SHLVL variable
sets the "?" special variable to 0
removes OLDPWD variable
*/
int	setup_env(t_shell_env *shell_env, char **envp)
{
	shell_env->envp = copy_env(envp);
	if (!shell_env->envp)
	{

		return (-1);
	}
	increment_shlvl(shell_env->envp);
	remove_var("OLDPWD", shell_env);
	shell_env->question_mark = 0;
	return (0);
}
