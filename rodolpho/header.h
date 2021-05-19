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

#define ARROW_UP -1
#define ARROW_DOWN -2

typedef struct	s_termcaps
{
	char	*cl_line;
	char	*cl_to_endline;
	char	*mv_cursor_col1;
	char	*mv_cursor_left;
	// char	*enter_del_mode;
	// char	*del_char;
	// char	*exit_del_mode;
}				t_termcaps;

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

int		init_termcaps(t_termcaps *termcaps);
int		ft_readline(char **line, t_list **history, t_termcaps termcaps);
void	arrow_up(char *buffer, char **backup, t_list *history, t_list **position);
void	arrow_down(char *buffer, char **backup, t_list *history, t_list **position);

int		parse_line(char *line, t_list **commands_lst);

int		execute_line(t_list *lst, char **envp);

int		launch_command(t_command *command, char **envp);
int		run_pipeline(t_list *lst, char **envp, int n);
int		resolve_path(char *command, char **path);
void	ft_perror(char *func_name);

#endif
