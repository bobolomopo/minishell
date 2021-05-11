#include <curses.h>
#include <term.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}	

int	main()
{
	char	*term_type;
	int		ret;
	char	*cl_line_cap;
	char	*mv_cursor_cap;

	term_type = getenv("TERM");
	printf("%s\n", term_type);

	ret = tgetent(NULL, term_type);
	printf("%d\n", ret);
	// errors should be handled here, in case the return value is 0 or -1

	ret = tgetnum("co");
	printf("ret: %d\n", ret);

	ret = tgetflag("bw");
	printf("capability: %d\n", ret);

	mv_cursor_cap = tgetstr("cr", NULL);
	cl_line_cap = tgetstr("cd", NULL);

	write(1, "something", 9);
	sleep(2);

	tputs(mv_cursor_cap, 1, ft_putchar); // move cursor to column 0
	tputs(cl_line_cap, 1, ft_putchar); // clear line

	write(1, "other stuff", 11);

	sleep(2);

}