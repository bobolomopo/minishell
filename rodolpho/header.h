#ifndef HEADER_H
# define HEADER_H

# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <string.h>
# include "libft.h"

typedef struct	s_piped_commands
{
	t_list	*commands_lst;
	char	*in_file;		// NULL if stdin
	char	*out_file;		// NULL if stdout 
	int		out_file_truc;
}				t_piped_commands;

char	*resolve_path(char *command);

#endif
