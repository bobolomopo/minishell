#include "header.h"

char	*assemble_path(char *dir, char *file)
{
	char	*str;
	int		size;

	size = ft_strlen(dir) + ft_strlen(file) + 2;
	str = malloc(size);
	if (!str)
		return (NULL);
	ft_strlcpy(str, dir, size);
	ft_strlcat(str, "/", size);
	ft_strlcat(str, file, size);
	return (str);
}

// returns a freeable string containing the complete file path 
// (absolute or relative)
char	*resolve_path(char *command)
{
	char		**path_dirs;
	char		*env_path;
	char		*path;
	int			i;
	struct stat	buf;

	if (ft_strchr(command, '/')) // if command contains '/', don't search in the PATH dirs
		return (ft_strdup(command));
	env_path = getenv("PATH"); // TODO: check if NULL
	path_dirs = ft_split(env_path, ':'); // TODO: check if error
	i = 0;
	while (path_dirs[i])
	{
		path = assemble_path(path_dirs[i], command);
		if (stat(path, &buf) == 0)
			break ;
		free(path);
		path = NULL;
		i++;
	}
	ft_free_split(path_dirs);
	return (path);
}
