#include "header.h"

// returns pointer to list element before elem
// return lst if elem is the first item
// return NULL if elem not in the list
t_list	*ft_lst_prev_item(t_list *lst, t_list *elem)
{
	if (elem == lst)
		return (elem);
	while (lst)
	{
		if (lst->next == elem)
			return (lst);
		lst = lst->next;
	}
	return (NULL);
}

int	backup_buffer(char *buffer, char **backup)
{
	*backup = ft_strdup(buffer);
	if (!*backup)
		return (-1);
	return (0);
}

void	restore_buffer(char *buffer, char **backup_buffer)
{
	ft_strlcpy(buffer, *backup_buffer, 100);
	free(*backup_buffer);
	*backup_buffer = NULL;
}

void	arrow_up(char *buffer, char **backup, t_list *history, t_list **position)
{
	if (*position == NULL) // current command, not in history
	{
		backup_buffer(buffer, backup); //TODO: handle error
		if (history)
			(*position) = history;
		else		// if history is empty, last line cannot be run...
			return ;
	}
	else if ((*position)->next == NULL) // last item in history
		return ;
	else
		(*position) = (*position)->next;
	ft_strlcpy(buffer, (*position)->content, 100);
}

void	arrow_down(char *buffer, char **backup, t_list *history, t_list **position)
{
	if (*position == NULL)
		return ;
	if (*position == history) // first item in history
	{
		restore_buffer(buffer, backup);
		*position = NULL;
		return ;
	}
	*position = ft_lst_prev_item(history, *position); // TODO
	ft_strlcpy(buffer, (*position)->content, 100);
}
