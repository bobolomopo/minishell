/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:29:17 by jandre            #+#    #+#             */
/*   Updated: 2021/05/06 16:29:17 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int		ft_nbrwords(char const *s, char c)
{
	int		count;
	int		i;

	i = 0;
	count = 0;
	while (s[i])
	{
		while (s[i] == c && s[i])
			i++;
		if (s[i])
			count++;
		while (s[i] != c && s[i])
			i++;
	}
	return (count);
}

static char		**ft_write_words(char **result, int nbrmot,
		const char *s, char c)
{
	int		i;
	int		j;
	int		k;

	i = -1;
	k = -1;
	while (s[++i] && k < nbrmot - 1)
	{
		j = 0;
		while (s[i] && s[i] == c)
			i++;
		while (s[i + j] && s[i + j] != c)
			j++;
		if ((!(result[++k] = ft_substr(s, i, j))) && k < nbrmot)
			return (ft_refree(result, k));
		i = i + j - 1;
		i++;
	}
	k++;
	result[k] = NULL;
	return (result);
}

char			**ft_split(char	const *s, char c)
{
	char	**result;
	int		nbrmot;

	if (!s)
		return (NULL);
	nbrmot = ft_nbrwords(s, c);
	if (!(result = (char **)malloc(sizeof(char *) * (nbrmot + 1))))
		return (NULL);
	return (ft_write_words(result, nbrmot, s, c));
}