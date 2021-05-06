/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_tokenize.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:29:13 by jandre            #+#    #+#             */
/*   Updated: 2021/05/06 16:29:13 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	length_until_c(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (str[i] && str[i] != '&' && str[i] != '|' && str[i] != ';' && str[i] != '<' &&
			str[i] != '>')
	{
		i++;
		size++;
	}
	if (size == 0)
	{
		// we verify how many char that we need follows each other to reassemble them all
		// its easier that way to verify after if there is a syntax error like :
		// if we put "cd .. |& ls" we will obtain [cd][..][|&][ls] when we have two
		// different character in the same token, then we know there is a syntax error
		// or even if there is more than 2 in one token!
		while (str[i] && (str[i] == '&' || str[i] == '|' || str[i] == ';' || str[i] == '<' ||
			str[i] == '>'))
		{
			size++;
			i++;
		}
	}
	return (size);
}

static char **tokenize_bis(char **buffer_token, int base_length, int length)
{
    int		i;
	int		j;
	char	**result;
	int		size;

	i = 0;
	j = 0;
	// malloc the new char ** that we will get back
	result = malloc(sizeof (char *) * (length + 1));
	if (!result)
		return (NULL);
	while (buffer_token[i])
	{
		// if one of those character is in the str, get in the loop that 
		// will separate them. If there is only those character in the str
		// then keep going, its already separated
		if (ft_isin_str(buffer_token[i], "&|><;") > 0 &&
			ft_only_str(buffer_token[i], "&| ><;") < 0)
		{
			// to know the size until when we have to separate the str
			size = length_until_c(buffer_token[i]);
			result[j] = ft_strnew(size + 1);
			ft_strcpy_n(result[j], buffer_token[i], size);
			j++;
			result[j] = ft_strnew(ft_strlen(buffer_token[i]) - size + 1);
			ft_strcpy_n(result[j], buffer_token[i] + size, ft_strlen(buffer_token[i]) - size);
		}
		else
		{
			// if there is no character that need separations just copy the str in 
			// the new char **
			result[j] = ft_strnew(ft_strlen(buffer_token[i]) + 1);
			if (!result[j])
				return (NULL);
			ft_strcpy(result[j], buffer_token[i]);
		}
		j++;
		i++;
	}
	result[j] = NULL;
	// we free recursively the last char ** so no memory loss like in split fct
	ft_refree(buffer_token, base_length);
	// we check if it still needs separation the fct will separate like that :
	// if we have [ok][&&ok&&ok][super] it will become : [ok][&&][ok&&ok][super]
	// so we still have to separate it
	return (tokenize(result));
}

char **tokenize(char **buffer_token)
{
	int base_length;
	int length;
	int i;

	i = 0;
	base_length = 0;
	length = 0;
	while (buffer_token[i])
	{
		// we just look if those character are presents in whats typed in the prompt
		// and if it is, we know we have to add a new str to contain it by comparing
		// the number of occurences seen versus the base_length of the char **
		if (ft_isin_str(buffer_token[i], "&|><;") > 0 &&
			ft_only_str(buffer_token[i], "&| ><;") < 0)
			length++;
		length++;
		base_length++;
		i++;
	}
	i = 0;
	// if its the same just return what you already have
	if (length != base_length)
		return (tokenize_bis(buffer_token, base_length, length));
	return(buffer_token);
}