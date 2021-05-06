/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:29:54 by jandre            #+#    #+#             */
/*   Updated: 2021/05/06 16:29:54 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

char	*ft_strchr(const char *s, int c)
{
	int		i;
	char	*result;
	char	test;

	i = 0;
	test = (char)c;
	result = (char *)s;
	while (s[i])
	{
		if (s[i] == test)
			return (result + i);
		i++;
	}
	if (s[i] == c)
		return (result + i);
	return (NULL);
}

int		ft_charpos(char *str, char c)
{
	int i;

	i = 0;
	while (str[i] && str[i] != c)
		i++;
	return (i);
}

int		ft_initialize(int fd, char **line, char **rest)
{
	if (fd > OPEN_MAX || !line)
		return (-1);
	if (rest[fd] == NULL)
	{
		if (!(rest[fd] = ft_strnew(0)))
			return (-1);
	}
	return (1);
}

int		ft_copy(char **rest, int fd)
{
	if (ft_strchr(rest[fd], '\n'))
	{
		ft_strcpy(rest[fd], ft_strchr(rest[fd], '\n') + 1);
		return (1);
	}
	if (ft_strlen(rest[fd]) > 0)
	{
		rest[fd] = ft_strchr(rest[fd], '\0');
		return (1);
	}
	return (0);
}

int		get_next_line(int fd, char **line)
{
	static char	*rest[OPEN_MAX];
	char		buffer[BUFFER_SIZE + 1];
	char		*ptr;
	int			verif;

	if (ft_initialize(fd, line, rest) < 0)
		return (-1);
	while (!(ft_strchr(rest[fd], '\n')) &&
			(verif = read(fd, buffer, BUFFER_SIZE)) > 0)
	{
		buffer[verif] = '\0';
		ptr = rest[fd];
		if (!(rest[fd] = ft_strjoin(ptr, buffer)))
			return (-1);
		free(ptr);
	}
	if (!(*line = ft_substr(rest[fd], 0, ft_charpos(rest[fd], '\n'))))
		return (-1);
	return (ft_copy(rest, fd));
}
