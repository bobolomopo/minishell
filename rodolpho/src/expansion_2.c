/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expansion_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rcammaro <rcammaro@student.s19.be>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/03 15:12:52 by rcammaro          #+#    #+#             */
/*   Updated: 2021/06/03 15:12:53 by rcammaro         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "header.h"

// returns the variable name in a freeable string moves pointer to first char
// after name.
// returns NULL if error. 
// if the first characted is not_valid, the returned name will be the empty
// string and the pointer will be moved to the next char (if not \0).
char	*parse_var_name(char **str)
{
	int		len;
	char	*ptr;

	ptr = *str;
	if (!(ft_isalpha(**str) || **str == '_'))
	{
		if (**str)
			(*str)++;
		return (ft_strdup(""));
	}
	(*str)++;
	len = 1;
	while (ft_isalnum(**str) || **str == '_')
	{
		(*str)++;
		len++;
	}
	return (ft_substr(ptr, 0, len));
}

static int	contains_dollar(char *str)
{
	while (*str)
	{
		if (*str == DOLLAR_SIGN)
			return (1);
		str++;
	}
	return (0);
}

// check for DOLLAR_SIGN in every word, and replace word by another one
// with the expanded variable
// Returns -1 if error, 0 upon success.
int	make_var_expansions(t_command *command, t_sh_env *shell_env)
{
	char	**arg;
	char	*new_arg;

	arg = command->argv;
	while (*arg)
	{
		if (contains_dollar(*arg))
		{
			new_arg = expand_str(*arg, shell_env);
			if (!new_arg)
				return (-1);
			free(*arg);
			*arg = new_arg;
		}
		arg++;
	}
	return (0);
}
