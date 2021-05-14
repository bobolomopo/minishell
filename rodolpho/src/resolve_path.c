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

// if command contais "/" or if the binary is found in a PATH directory, return 1 and a
// a freeable string to path
// return 0 if binary not found
// return -1 if error
int	resolve_path(char *command, char **path)
{
	char		**path_dirs;
	char		*env_path;
	int			i;
	struct stat	buf;

	if (ft_strchr(command, '/'))
	{
		*path = ft_strdup(command);
		if (*path)
			return (-1);
		return (1);
	}
	env_path = getenv("PATH");
	if (env_path == NULL)
	{
		*path = NULL;
		return (0);
	}
	path_dirs = ft_split(env_path, ':');
	if (!path_dirs)
		return (-1);
	i = 0;
	while (path_dirs[i])
	{
		*path = assemble_path(path_dirs[i], command); // check if error
		if (!*path)
		{
			ft_free_split(path_dirs);
			return (-1);
		}
		if (stat(*path, &buf) == 0)
		{
			ft_free_split(path_dirs);
			return (1);
		}
		free(*path);
		i++;
	}
	ft_free_split(path_dirs);
	return (0);
}
