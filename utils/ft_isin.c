/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_isin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:33:35 by jandre            #+#    #+#             */
/*   Updated: 2021/05/06 16:33:35 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_isin(char *str, char c)
{
	int		i;

	i = 0;
	while (str[i])
	{
		if (str[i] == c)
			return (1);
		i++;
	}
	return (-1);
}

int ft_isin_str(char *str, char *cmp)
{
    int     i;

    i = 0;
    while (cmp[i])
    {
        if (ft_isin(str, cmp[i]) > 0)
            return (1);
        i++;
    }
    return (-1);
}