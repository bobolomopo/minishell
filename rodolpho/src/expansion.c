#include "header.h"

// appends str2 to str1. The sources are freed (so they must be mallocd memory).
// NULL + str2 = str2 (even if str is NULL)
// str1 + NULL = str1
// if error, returns NULL
// obs: ft_strjoin returns NULL in case of error.
char	*append(char *str1, char *str2)
{
	char	*dst;

	if (!str1)
		return (str2);
	if (!str2)
		return (str1);
	dst = ft_strjoin(str1, str2);	
	free(str1);
	free(str2);
	return (dst);
}

// makes a copy of chars up to DOLLAR_SIGN or '\0'
// *str is moved to '\0' or DOLLAR_SIGN
char	*get_chars_part(char **str)
{
	int		len;
	char	*ptr;
	char	*dst;

	ptr = *str;
	len = 0;
	while (**str && **str != DOLLAR_SIGN)
	{
		len++;
		(*str)++;
	}
	dst = ft_substr(ptr, 0, len);
	if (!dst)
		return (NULL);
	return (dst);
}

// returns the variable name in a freeable string moves pointer to first char
// after name.
// returns NULL if error or no valid name 
char	*parse_var_name(char **str)
{
	int		len;
	char	*ptr;

	ptr = *str;
	if (!(ft_isalpha(**str) || **str == '_'))
		return (NULL);
	(*str)++;
	len = 1;
	while (ft_isalnum(**str) || **str == '_')
	{
		(*str)++;
		len++;
	}
	return (ft_substr(ptr, 0, len));
}

// moves *str to char after var_name
// returns a newly mallocd string containing the value of var
// if var does not exits, returns the empty string.
// NULL if error or var does not exist -- TODO fix this behavior to separate error from non exist.
char	*get_var_value_part(char **str, t_shell_env *shell_env)
{
	char	*var_name;
	char	*var_value;

	(*str)++;
	if (**str == '?')
	{
		(*str)++;
		return (ft_itoa(shell_env->question_mark)); // NOT SURE... unsigned char x int
	}
	var_name = parse_var_name(str);
	if (!var_name)
		return (NULL);
	var_value = expand_var(shell_env->envp, var_name);
	free(var_name);
	if (!var_value)
		return (ft_strdup(""));
	return (ft_strdup(var_value));
}

// expands all DOLLAR_SIGN-VAR_NAME occurrences into their respective values
// returns a freeable str or NULL if error
char	*expand_str(char *str, t_shell_env *shell_env)
{
	char	*dst;
	char	*chars_part;
	char	*var_value_part;

	dst = NULL;
	while (*str)
	{
		if (*str == DOLLAR_SIGN)
		{
			var_value_part = get_var_value_part(&str, shell_env); // TODO error handling
			dst = append(dst, var_value_part); // TODO error handling
			continue ;
		}
		chars_part = get_chars_part(&str); // TODO error handling
		dst = append(dst, chars_part); // TODO error handling
	}
	return (dst);
}

int	contains_dollar(char *str)
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
int	make_var_expansions(t_command *command, t_shell_env *shell_env)
{
	char	**arg;
	char	*new_arg;

	arg = command->argv;
	while (*arg)
	{
		if (contains_dollar(*arg))
		{
			new_arg = expand_str(*arg, shell_env);  // error handling TODO...
			free(*arg);
			*arg = new_arg;
		}
		arg++;
	}
	return (0);
}

// stuff of testing

// int	main(int argc, char **argv, char **envp)
// {
// 	t_shell_env	shell_env;
// 	char 	arg[100] = "hello $HOME$_$?bye$NOVAR";

// 	char *ptr = arg;
// 	while (*ptr)
// 	{
// 		if (*ptr == '$')
// 			*ptr = DOLLAR_SIGN;
// 		ptr++;
// 	}

// 	shell_env.envp = envp;
// 	shell_env.question_mark = 43;

// 	char *str = expand_str(arg, &shell_env);
// 	printf("%s\n", str);
// 	free(str);

// 	(void)argc;
// 	(void)argv;
// }
