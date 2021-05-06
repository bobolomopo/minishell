/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_only_str.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:51:42 by jandre            #+#    #+#             */
/*   Updated: 2021/05/06 16:51:42 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_only_str(char *str, char *cmp)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isin(cmp, str[i]) < 0)
			return (-1);
		i++;
	}
	return (1);
}