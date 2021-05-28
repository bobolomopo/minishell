#include "header.h"

// function to be called by a subshell (child process).
// This function does not return. It exits.
void	exec_bin(t_command *command, t_sh_env *shenv)
{
	int		ret;
	char	*path;

	if (process_redirections_list(command->redirections) == -1)
		clear_mem_exit(shenv, -1);
	ret = resolve_path(command->argv[0], &path);
	if (ret == -1)
		clear_mem_exit(shenv, -1); // todo: error msg from resolve path
	if (ret == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(command->argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		clear_mem_exit(shenv, 127);
	}
	execve(path, command->argv, shenv->envp);
	// only executed in case of error:
	ft_putstr_fd("minishell: ", 2);
	ft_perror(path);
	free(path);
	clear_mem_exit(shenv, 127); // or 126 if command found but not executable
}

int	exit_code_from_child(int status)
{
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 3)
			ft_putendl_fd("Quit: 3", 1); // stdin or stdout??
		return (128 + WTERMSIG(status));
	}
	return (0);
}

// function to be called by the main (parent) process.
// makes variable expansions, check if command is a builtin
// if builtin, calls run_builtin
// else, forks into a subshell, which calls exec_bin
// the return code for the command (builtin or not) is returned, or
// -1 if error.
int	run_simple_command(t_command *command, t_sh_env *shenv)
{
	int		builtin_index;
	int		status;
	pid_t	id;

	if (make_var_expansions(command, shenv) == -1)
		return (-1);
	builtin_index = is_builtin(command->argv[0]);
	if (builtin_index != -1)
		return (run_builtin(builtin_index, command, shenv));
	id = fork();
	if (id == 0)
		exec_bin(command, shenv);
	wait(&status);
	return (exit_code_from_child(status));
}

// lst is a list of pipelines
// executes every pipeline
// a pipeline of size 1 is run by run_simple_command. Otherwise, by run_pipeline
int	execute_line(t_list *lst, t_sh_env *shell_env)
{
	t_list	*pipeline;
	int		n_commands;
	int		ret;

	while (lst)
	{
		pipeline = lst->content;
		n_commands = ft_lstsize(pipeline);
		if (n_commands == 0)
			return (0);

		// make the variable expansions here instead of inside the other functions?
		
		if (n_commands == 1)
			ret = run_simple_command(pipeline->content, shell_env); 
		else
			ret = run_pipeline(pipeline, shell_env, n_commands);

		// what to do if ret == -1?

		shell_env->question_mark = ret;  // update "?" special variable
		lst = lst->next;
	}
	return (0);
}
