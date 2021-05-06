/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:29:59 by jandre            #+#    #+#             */
/*   Updated: 2021/05/06 16:29:59 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

int	main()
{
	char	*buffer;
    char    **buffer_token;
    int     i;
    
    i = 0;
	// first writing of the prompt maybe change it with the environment variable user once its implemented
	write(1, "$> ", 3);
	// we read STDIN 
	while (get_next_line(0, &buffer) > 0)
    {
		// We split with ' ' as a separator
        buffer_token = ft_split(buffer, ' ');
        // now we assemble every char that correspond to '&', '>', '<', ';' or '|'
        buffer_token = tokenize(buffer_token);
        // Just to visualize everything
        while (buffer_token[i])
        {
            printf("[%s]", buffer_token[i]);
            i++;
        }
        write(1, "\n$> ", 4);
        i = 0;
    }
    // little goodbye msg and then free everything it will be in a fonction after
	printf("Bye \n");
	free(buffer);
    return (0);
}