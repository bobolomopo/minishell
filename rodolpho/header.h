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

typedef struct s_redirection
{
	int		n;
	int		op;
	char	*file;
}				t_redirection;

enum	redirection_operators
{
	re_input,			// <
	re_output,			// >
	re_output_append	// >>
};

typedef struct	s_command
{
	char	**argv;
	t_list	*redirections;
}				t_command;


typedef struct	s_piped_commands
{
	t_list	*commands_lst;
	char	*in_file;		// NULL if stdin
	char	*out_file;		// NULL if stdout 
	int		out_file_truc;
}				t_piped_commands;

int		resolve_path(char *command, char **path);
void	ft_perror(char *func_name);
int	launch_command(t_command *command, char **envp);

#endif
