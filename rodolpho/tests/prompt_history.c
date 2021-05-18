#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <termios.h>
#include <curses.h>
#include <term.h>
#define ARROW_UP -1
#define ARROW_DOWN -2

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}	

int	ft_strlen(char *str)
{
	int	len;

	len = 0;
	while (*str++)
		len++;
	return (len);
}

int	error(char *msg, int value)
{
	write(2, msg, ft_strlen(msg));
	return (value);
}

int	setup_terminal(struct termios *termios_p_backup)
{
	int	ret;
	struct termios	termios_p;

	ret = tcgetattr(0, termios_p_backup);
	if (ret == -1)
		return (-1); // error
	ret = tcgetattr(0, &termios_p);
	if (ret == -1)
		return (-1); // error
	termios_p.c_lflag &= ~(ICANON); // put terminal in non canonical mode
	termios_p.c_lflag &= ~(ECHO);	// disable echo
	ret = tcsetattr(0, 0, &termios_p);
	if (ret == -1)
		return (-1); // error
	return (1);
}

int	init_termcaps(char **cl_line_cap, char **mv_cursor_cap)
{
	char	*term_type;
	int		ret;

	term_type = getenv("TERM");
	if (term_type == NULL)
		return (error("TERM environmental variable must be set\n", -1));
	ret = tgetent(NULL, term_type);
	if (ret == 0)
		return (error("Terminal type not found in terminfo database\n", -1));
	if (ret == -1)
		return (error("Terminfo database not found\n", -1));
	*mv_cursor_cap = tgetstr("cr", NULL);
	*cl_line_cap = tgetstr("cd", NULL);
	return (1);
}


int	detect_key(void)
{
	char	buffer[2];

	read(0, buffer, 1);
	if (buffer[0] == '\033') // detecting ARROWS UP/DOWN key presses
	{
		read(0, buffer, 2);
		if (buffer[0] == '[')
		{
			if (buffer[1] == 'A')
				return (ARROW_UP);
			else if (buffer[1] == 'B')
				return (ARROW_DOWN);
		}
		else
			return (0);
	}
	return (buffer[0]);
}

int	update_command_index(int command_index, int key, int history_size)
{
	if (key == ARROW_UP)
		command_index++;
	else
		command_index--;
	if (command_index < 0)
		return (0);
	else if (command_index == history_size)
		return (history_size - 1);
	return (command_index);
}

void	display_command(char *prompt, char *command, char *mv_cursor_cap, char *cl_line_cap)
{
	tputs(mv_cursor_cap, 1, ft_putchar); // move cursor to column 0
	tputs(cl_line_cap, 1, ft_putchar); // clear line
	write(1, prompt, ft_strlen(prompt)); // display prompt
	write(1, command, ft_strlen(command)); // disply command
}

int	main()
{
	char	*prompt = "prompt > ";
	char	*history[4] = {"", "last command", "avant-dernier commande", "3rd last command"};
	int		command_index;
	int		key;
	struct termios	termios_p_backup;
	int		ret;
	char	*cl_line_cap;
	char	*mv_cursor_cap;

	// put terminal in non canonical mode and disable echo
	ret = setup_terminal(&termios_p_backup);
	if (ret == -1)
		return (1); // error

	// initialize termcaps
	ret = init_termcaps(&cl_line_cap, &mv_cursor_cap);	
	if (ret == -1)
		return (1); // error

	
	command_index = 0;
	while (1)
	{
		key = detect_key();
		if (key == ARROW_UP || key == ARROW_DOWN)
		{
			command_index = update_command_index(command_index, key, 4);
			display_command(prompt, history[command_index], mv_cursor_cap, cl_line_cap);
		}
		else if (!key)
			break ;
		else
			write(1, &key, 1);
	}

	// recover terminal settings
	tcsetattr(0, 0, &termios_p_backup);
	return (0);
}
