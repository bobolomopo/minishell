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
# include <signal.h>

#define ARROW_UP -1
#define ARROW_DOWN -2

#define DOLLAR_SIGN -2

typedef struct	s_shell_env
{
	char			**envp;
	unsigned char	question_mark;
}				t_shell_env;


typedef	struct	s_cmdline
{
	char	text_buffer[100];
	char	*backup_buffer;
	char	prompt[20];
	t_list	*position;
	int		readline_mode;

}				t_cmdline;

typedef struct	s_termcaps
{
	char	*cl_line;
	char	*cl_to_endline;
	char	*mv_cursor_col1;
	char	*mv_cursor_left;
	// char	*enter_del_mode;   // I'll have to try those again
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

// global variable for signal handling
void	*g_ptr;

// environment
int		setup_env(t_shell_env *shell_env, char **envp);
char	*find_var(char **env, char *var);
int		set_var(t_shell_env *shell_env, char *var_name, char *value);
char	*expand_var(char **envp,char *var);

// signal handling
void	signal_handler_c(int signal_code);
void	sigquit_handler(int signal_code);

// command-line
int		init_termcaps(t_termcaps *termcaps);
int		ft_readline(char **line, t_list **history, t_termcaps termcaps);
void	arrow_up(char *buffer, char **backup, t_list *history, t_list **position);
void	arrow_down(char *buffer, char **backup, t_list *history, t_list **position);

// mini-parser (for tests)
int		parse_line(char *line, t_list **commands_lst);

// expansion
int		make_var_expansions(t_command *command, t_shell_env *shell_env);
char	*parse_var_name(char **str);

// execution
int		execute_line(t_list *lst, t_shell_env *shell_env);
int		launch_command(t_command *command, t_shell_env *shell_env);
int		run_pipeline(t_list *lst, t_shell_env *shell_env, int n);
int		resolve_path(char *command, char **path);
void	ft_perror(char *func_name);

// builtins
int		builtin_env(char **argv, t_shell_env *shell_env);
int		builtin_echo(char **argv, t_shell_env *shell_env);
int		builtin_pwd(char **argv, t_shell_env *shell_env);
int		builtin_cd(char **argv, t_shell_env *shell_env);
int		builtin_export(char **argv, t_shell_env *shell_env);

#endif
