#ifndef HEADER_H
# define HEADER_H

# include "libft.h"
# include <stdio.h>
# include <unistd.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <sys/stat.h>
# include <string.h>
# include <fcntl.h>
# include <signal.h>
# include <termios.h>
# include <curses.h>
# include <term.h>

# define ARROW_UP -2
# define ARROW_DOWN -3

# define BACKSPACE 127
# define CTRL_D 4
# define ENTER_KEY 10

# define PROMPT "super_prompt $ "
# define BUFFER_STEP 100

# define DOLLAR_SIGN -2
# define SEMICOLON -3
# define PIPE -4
# define SPACE -5
# define LESS_THAN -6
# define GREATER_THAN -7

enum	e_states
{
	s_text, s_dquote, s_squote, s_backslash, s_bslash_in_dquote
};

typedef struct	s_mem2clear
{
	t_list	*pipelines_lst;
	t_list	*history;
}				t_mem2clear;

typedef struct	s_shell_env
{
	char			**envp;
	unsigned char	question_mark;
	t_mem2clear		mem2clear;
}				t_sh_env;

typedef	struct	s_cmdline
{
	char	*buffer;
	int		size;
	int		index;
	char	*backup_buffer;
	t_list	*position;
}				t_cmdline;

typedef struct	s_termcaps
{
	char	*cl_line;
	char	*cl_to_endline;
}				t_tcaps;

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

int		setup_env(t_sh_env *shell_env, char **envp);
int		find_var_index(char **envp, char *var);
int		set_var(t_sh_env *shell_env, char *var_name, char *value);
char	*expand_var(char **envp,char *var);
void	remove_var(char *var_name, t_sh_env *shell_env);

// signal handling

void	setup_signal_handlers(void);

// command-line and ft_realine

int		init_termcaps(t_tcaps *termcaps);
int		setup_terminal(struct termios *termios_p_backup);
void	reset_terminal(struct termios *termios_p_backup);
int		ft_readline(char **line, t_list *history, t_tcaps termcaps);
int		arrow_up(t_cmdline *cmdline, t_list *history, t_tcaps termcaps);
int		arrow_down(t_cmdline *cmdline, t_list *history, t_tcaps termcaps);
int		initialize_cmdline(t_cmdline *cmdline, t_list *history);
void	close_cmdline(t_cmdline *cmdline);
int		add_char(int key, t_cmdline *cmdline);
int		delete_last_char(t_cmdline *cmdline, t_tcaps termcaps);
void	refresh_display(t_cmdline *cmdline, t_tcaps termcaps);

// mini-parser (for tests)

char	*pre_processor(char *line);
int		parse_line(char *line, t_list **commands_lst);

// expansion

int		make_var_expansions(t_command *command, t_sh_env *shenv);
char	*expand_str(char *str, t_sh_env *shell_env);
char	*parse_var_name(char **str);

// execution

int		execute_line(t_list *lst, t_sh_env *shell_env);
int		run_pipeline(t_list *lst, t_sh_env *shell_env, int n);
int		resolve_path(char *command, char **path, char **envp);
int		process_redirections_list(t_list *lst);
int		is_builtin(char *command_name);
int		run_builtin(int builtin_index, t_command *command, t_sh_env *shenv);
void	exec_bin(t_command *command, t_sh_env *shenv);
int		exit_code_from_child(int status);

// builtins

int		builtin_env(char **argv, t_sh_env *shell_env);
int		builtin_echo(char **argv, t_sh_env *shell_env);
int		builtin_pwd(char **argv, t_sh_env *shell_env);
int		builtin_cd(char **argv, t_sh_env *shell_env);
int		builtin_export(char **argv, t_sh_env *shell_env);
int		builtin_exit(char **argv, t_sh_env *shell_env);
int		builtin_unset(char **argv, t_sh_env *shell_env);

int		is_valid_name(char *var);
void	print_error_msg_name(char *arg, char *builtin);

// clear memory

void	clear_memory(t_sh_env *shell_env);
int		clear_mem_exit(t_sh_env *shell_env, int exit_code);
void	clear_pipeline(void *ptr);

// errors

void	ft_perror(char *func_name);
int		ft_perror_ret(char *func_name, int return_value);

// debugging tools

void print_list_commands(t_list *list_of_pipelines);

#endif
