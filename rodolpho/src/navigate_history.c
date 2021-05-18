#include "header.h"

// save original text buffer in a temp buffer
// replace buffer by next command in the list, update command pointer
// if DOWN on first element of list restore original buffer
navigate_history(int key, char *buffer, t_list **next_item, t_list *history)
{
	int			is_new_line;
	int			is_1st_item_hist;
	static char	backup[100];

	if (history == NULL)
		return ;
	is_new_line = (*next_item == history);
	if (is_new_line && key == ARROW_DOWN)
		return ;
	if (*next_item == NULL && key == ARROW_UP)
		return ;
	if (is_new_line && key == ARROW_UP) 
	{
		ft_strlcpy(backup, buffer, 100);
		ft_strlcpy(buffer, (*next_item)->content, 100);
		(*next_item) = (*next_item)->next;
		return ;
	}
	is_1st_item_hist = (*next_item == history->next);
	if (is_1st_item_hist && key == ARROW_DOWN)
	{
		ft_strlcpy(buffer, backup, 100);
		(*next_item) = history;
		return ;
	}
	if (key == ARROW_UP)
	{
		ft_strlcpy(buffer, (*next_item)->content, 100);
		(*next_item) = (*next_item)->next;
	}
	else
	{
		// move next_item pointer BACK
		// copy to buffer
	}
	
}