#include "header.h"

// duplicates the file descriptors 0, 1 and 2, to be recovered later.
int	backup_std_fds(int *fd)
{
	int	i;

	i = 0;
	while (i <= 2)
	{
		fd[i] = dup(i);
		if (fd[i] == -1)
		{
			ft_perror("dup");
			while (--i >= 0)
				close(fd[i]);
			return (-1);
		}
		i++;
	}
	return (0);
}

// restores stdin, stdout and stderr to the original file descriptors (0, 1, 2)
int	restore_std_fds(int *fd)
{
	int	i;
	int	ret_value;

	ret_value = 0;
	i = 0;
	while (i <= 2)
	{
		if (dup2(fd[i], i) == -1)
		{
			ft_perror("dup2");
			ret_value = -1;
		}
		close(fd[i]);
		i++;
	}
	return (ret_value);
}

// opens file in read or write mode (according to "op") and redirects 
// the file descriptor n to this file
int	make_redirection(int n, int op, char *file)
{
	int	fd;

	if (op == re_input)
		fd = open(file, O_RDONLY);
	else if (op == re_output)
		fd = open(file, O_WRONLY | O_CREAT | O_TRUNC, 0664);
	else
		fd = open(file, O_WRONLY | O_CREAT | O_APPEND, 0664);
	if (fd == -1)
	{
		ft_perror("open");
		return (-1);
	}
	if (dup2(fd, n) == -1)
	{
		ft_perror("dup2");
		close(fd);
		return (-1);
	}
	close(fd);
	return (0);
}

// call make_redirection() for every redirection in the list
int	process_redirections_list(t_list *lst)
{
	t_redirection	*r;

	while (lst)
	{
		r = lst->content;
		if (make_redirection(r->n, r->op, r->file) == -1)
			return (-1);
		lst = lst->next;
	}
	return (0);
}

// retuns index (0 to 6) if the command is a builtin, -1 otherwise
int	is_builtin(char *command_name)
{
	static char*	builtin[7] = {"echo", "cd", "pwd", "export", "unset",
	"env", "exit"};
	int	i;

	i = 0;
	while (i < 7)
	{
		if (ft_strcmp(command_name, builtin[i]) == 0)
			return (i);
		i++;
	}
	return (-1);
}

// jump table to call builtin command (array of pointers to functions)
// indexes must match those in is_builtin()
int	call_builtin(int builtin_index, char **argv, t_sh_env *shell_env)
{
	static int (*f[7])(char **, t_sh_env *) = {builtin_echo, builtin_cd,
	builtin_pwd, builtin_export, builtin_unset, builtin_env, builtin_exit};
	
	return (f[builtin_index](argv, shell_env));
}

// seach for the command in the PATH directories, forks and execs into the program
int	exec_binary(char **argv, char **envp)
{
	pid_t	id;
	int		status;
	char	*path;
	int		ret;

	ret = resolve_path(argv[0], &path);
	if (ret == -1)
		return (-1);
	if (ret == 0)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(argv[0], 2);
		ft_putendl_fd(": command not found", 2);
		return (127);
	}
	id = fork();
	if (id == 0)	// child process
	{
		execve(path, argv, envp);
		ft_putstr_fd("minishell: ", 2);
		ft_perror(path);
		exit(127); // or 126 if command found but not executable
	}
	wait(&status); // should I use waitpid instead?
	free(path);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status));
	if (WIFSIGNALED(status))
	{
		if (WTERMSIG(status) == 3)
			ft_putendl_fd("Quit: 3", 1); // stdin or stdout??
		return (128 + WTERMSIG(status));
	}
	return (-1); // decide what to return...
}

// performs variable expansion, redirections, executes command 
// and restores stdin, stdout and stderr
// if command is a builtin, a function is called and run in the same process (no fork)
// if not a builtin, its is executed by a sub-shell (a child process)
int	launch_command(t_command *command, t_sh_env *shell_env)
{
	int	back_up_fds[3];
	int	ret;
	int	builtin_index;

	// make variable expansion
	if (make_var_expansions(command, shell_env) == -1)
		return (-1);

	// backup stdin, stdout, stderr
	if (backup_std_fds(back_up_fds) == -1)
		return (-1);

	// perform redirections
	if (process_redirections_list(command->redirections) == -1)
	{
		restore_std_fds(back_up_fds);
		return (-1);
	}

	builtin_index = is_builtin(command->argv[0]);
	if (builtin_index != -1)
		ret = call_builtin(builtin_index, command->argv, shell_env);
	else
		ret = exec_binary(command->argv, shell_env->envp);
			
	// restore std fds.
	if (restore_std_fds(back_up_fds) == -1)
		return (-1);
	return (ret);
}

// int main(int argc, char **argv_main, char **envp)
// {
// 	t_command	command;
// 	t_list		*list;
// 	char *argv[3] = {"cat", "-e", NULL};
// 	t_redirection	red1 = {0, re_input, "infile"};
// 	t_redirection	red2 = {1, re_output, "outfile"};
// 	t_redirection	red3 = {1, re_output, "outfile1"};
// 	t_redirection	red4 = {1, re_output, "outfile2"};

// 	list = NULL;
// 	ft_lstadd_back(&list, ft_lstnew(&red1));
// 	ft_lstadd_back(&list, ft_lstnew(&red2));
// 	ft_lstadd_back(&list, ft_lstnew(&red3));
// 	ft_lstadd_back(&list, ft_lstnew(&red4));

// 	command.argv = argv;
// 	command.redirections = list;

// 	(void)argc;
// 	(void)argv_main;
// 	launch_command(&command, envp);
// 	return (0);
// }
