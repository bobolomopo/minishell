/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_refree.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:48:35 by jandre            #+#    #+#             */
/*   Updated: 2021/05/06 16:48:35 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char **ft_refree(char **res, int index)
{
	while (--index > 0)
		free(res[index]);
	free(res);
	return (NULL);
}