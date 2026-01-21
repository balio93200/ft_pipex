#include "pipex.h"

static char	*grow_buf(char *buf, size_t *cap)
{
	char	*nbuf;
	size_t	ncap;

	ncap = (*cap) * 2;
	nbuf = (char *)malloc(ncap);
	if (!nbuf)
		return (NULL);
	ft_memcpy(nbuf, buf, *cap);
	free(buf);
	*cap = ncap;
	return (nbuf);
}

static char	*read_line(int fd)
{
	char	c;
	ssize_t	r;
	char	*buf;
	size_t	len;
	size_t	cap;

	cap = 64;
	buf = (char *)malloc(cap);
	if (!buf)
		return (NULL);
	len = 0;
	while (1)
	{
		r = read(fd, &c, 1);
		if (r <= 0)
			break ;
		buf[len++] = c;
		if (c == '\n')
			break ;
		if (len + 1 >= cap)
		{
			buf = grow_buf(buf, &cap);
			if (!buf)
				return (NULL);
		}
	}
	if (r <= 0 && len == 0)
		return (free(buf), (char *)0);
	buf[len] = '\0';
	return (buf);
}

static void	heredoc_writer(const char *limiter, int write_end)
{
	char	*line;

	while (1)
	{
		line = read_line(STDIN_FILENO);
		if (!line)
			break ;
		if (ft_streq_limiter(line, limiter))
		{
			free(line);
			break ;
		}
		write(write_end, line, ft_strlen(line));
		free(line);
	}
	close(write_end);
	exit(0);
}

int	create_heredoc_pipe(const char *limiter, pid_t *writer_pid)
{
	int		pfd[2];
	pid_t	pid;

	if (pipe(pfd) == -1)
		die_perror("pipe", 1);
	pid = fork();
	if (pid == -1)
		die_perror("fork", 1);
	if (pid == 0)
	{
		close(pfd[0]);
		heredoc_writer(limiter, pfd[1]);
	}
	close(pfd[1]);
	*writer_pid = pid;
	return (pfd[0]);
}
