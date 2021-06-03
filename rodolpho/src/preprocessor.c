#include "header.h"

void	text(char **dst, char *line, int *state)
{
	if (*line == ';')
		*(*dst)++ = SEMICOLON;
	else if (*line == '|')
		*(*dst)++ = PIPE;
	else if (*line == '$')
		*(*dst)++ = DOLLAR_SIGN;
	else if (*line == ' ')
		*(*dst)++ = SPACE;
	else if (*line == '\"')
		*state = s_dquote;
	else if (*line == '\'')
		*state = s_squote;
	else
		*(*dst)++ = *line;
}

void	dquote(char **dst, char *line, int *state)
{
	if (*line == '$')
		*(*dst)++ = DOLLAR_SIGN;
	else if (*line == '\"')
		*state = s_text;
	else if (*line == '\\')
		*state = s_bslash_in_dquote;
	else
		*(*dst)++ = *line;
}

void	squote(char **dst, char *line, int *state)
{
	if (*line == '\'')
		*state = s_text;
	else
		*(*dst)++ = *line;
}

void	backslash(char **dst, char *line, int *state)
{
	*(*dst)++ = *line;
	*state = s_text;
}

void	bslash_in_dquote(char **dst, char *line, int *state)
{
	if (*line == '$' || *line == '\"' || *line == '\\')
		*(*dst)++ = *line;
	else
	{
		*(*dst)++ = '\\';
		*(*dst)++ = *line;
	}
	*state = s_dquote;
}

char	*pre_processor(char *line)
{
	char	*dst;
	char	*ptr;
	int		state;
	static void	(*f[5])(char **, char *, int *)
				= {text, dquote, squote, backslash, bslash_in_dquote};

	dst = malloc(ft_strlen(line));
	ptr = dst;
	state = s_text;
	while (*line)
	{
		f[state](&ptr, line, &state);
		line++;
	}
	if (state != s_text)
	{
		ft_putendl_fd("Syntax error", 2);
		free(dst);
		dst = NULL;
	}
	return (dst);
}

// int main()
// {
// 	char *line = "hello; how | are $ you?  \" I \\x am ; just fine | \", \'tah $x\'";

// 	char *dst = pre_processor(line);
// 	if (!*dst)
// 		return (1);

// 	char *ptr = dst;
// 	while (*ptr)
// 	{
// 		if (ft_isprint(*ptr))
// 			ft_putchar_fd(*ptr, 1);
// 		else
// 			ft_printf("[%d]", *ptr);
// 		ptr++;
// 	}
// 	ft_putendl_fd("", 1);
// 	free(dst);
// }