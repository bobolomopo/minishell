#include "header.h"

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
