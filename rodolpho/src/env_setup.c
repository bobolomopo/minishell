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
		return (-1);
	increment_shlvl(shell_env->envp);
	remove_var("OLDPWD", shell_env);
	shell_env->question_mark = 0;
	return (0);
}