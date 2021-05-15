#ifndef HEADER_H
# define HEADER_H

# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <string.h>
# include "libft.h"
# include <fcntl.h>

typedef struct	s_command
{
	char	**argv;
	t_list	*redirections;
}				t_command;

typedef struct s_redirection
{
	int		n;
	int		op;
	char	*file;
}				t_redirection;

enum	redirection_ops
{
	re_input,			// <
	re_output,			// >
	re_output_append	// >>
};

int		launch_command(t_command *command, char **envp);
int		run_pipeline(t_list *lst, char **envp, int n);
int		resolve_path(char *command, char **path);
void	ft_perror(char *func_name);

#endif
