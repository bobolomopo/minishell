/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jandre <Ajuln@hotmail.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/05/06 16:30:23 by jandre            #+#    #+#             */
/*   Updated: 2021/05/06 16:30:23 by jandre           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H
#ifndef BUFFER_SIZE
#define BUFFER_SIZE 32
#endif
#ifndef OPEN_MAX
#define OPEN_MAX 256
#endif
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>

int 	get_next_line(int fd, char **line);
int 	ft_charpos(char *str, char c);
char	*ft_strnew(int size);
char	*ft_strcpy(char *dst, const char *src);
char	*ft_strcpy_n(char *dst, const char *src, int size);
char	*ft_strjoin(char const *s1, char const *s2);
char	*ft_substr(char const *s, int start, int len);
char	*ft_strchr(char const *s, int c);
int 	ft_strlen(const char *str);
int 	ft_initialize(int fd, char **line, char **rest);
int 	ft_copy(char **rest, int fd);
char	**ft_split(char const *s, char c);
int 	ft_isin(char *str, char c);
int 	ft_isin_str(char *str, char *cmp);
char	**ft_refree(char **res, int index);
int 	ft_only_str(char *str, char *cmp);
char 	**tokenize(char **buffer_token);

#endif
