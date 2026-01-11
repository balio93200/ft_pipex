#include "pipex.h"

static char	*get_env_path(char **envp)
{
	int i = 0;
	while (envp && envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

static char	*try_join(const char *dir, size_t len, const char *cmd)
{
	char *part;
	char *full;

	part = ft_substr(dir, 0, len);
	if (!part)
		return (NULL);
	full = ft_strjoin3(part, "/", cmd);
	free(part);
	if (!full)
		return (NULL);
	if (access(full, X_OK) == 0)
		return (full);
	free(full);
	return (NULL);
}

char	*find_cmd_path(const char *cmd, char **envp)
{
	char *path;
	size_t i = 0;
	size_t start = 0;
	char *found;

	if (!cmd)
		return (NULL);
	while (cmd[i])
	{
		if (cmd[i] == '/')
			return (access(cmd, X_OK) == 0 ? ft_strdup(cmd) : NULL);
		i++;
	}
	path = get_env_path(envp);
	if (!path)
		return (NULL);
	i = 0;
	while (1)
	{
		if (path[i] == ':' || path[i] == '\0')
		{
			found = try_join(path + start, i - start, cmd);
			if (found)
				return (found);
			if (path[i] == '\0')
				break;
			start = i + 1;
		}
		i++;
	}
	return (NULL);
}
