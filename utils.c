#include "pipex.h"

size_t	ft_strlen(const char *s)
{
	size_t i = 0;
	while (s && s[i])
		i++;
	return (i);
}

int	ft_strncmp(const char *s1, const char *s2, size_t n)
{
	size_t i = 0;
	while (i < n)
	{
		if ((unsigned char)s1[i] != (unsigned char)s2[i]
			|| !s1[i] || !s2[i])
			return ((unsigned char)s1[i] - (unsigned char)s2[i]);
		i++;
	}
	return (0);
}

char	*ft_strdup(const char *s)
{
	size_t len = ft_strlen(s);
	char *p = malloc(len + 1);
	size_t i = 0;
	if (!p)
		return (NULL);
	while (i < len)
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}

char	*ft_substr(const char *s, size_t start, size_t len)
{
	size_t slen = ft_strlen(s);
	size_t i = 0;
	char *out;

	if (start > slen)
		len = 0;
	else if (start + len > slen)
		len = slen - start;
	out = malloc(len + 1);
	if (!out)
		return (NULL);
	while (i < len)
	{
		out[i] = s[start + i];
		i++;
	}
	out[i] = '\0';
	return (out);
}

char	*ft_strjoin3(const char *a, const char *b, const char *c)
{
	size_t na = ft_strlen(a);
	size_t nb = ft_strlen(b);
	size_t nc = ft_strlen(c);
	char *out = malloc(na + nb + nc + 1);
	size_t i = 0;

	if (!out)
		return (NULL);
	while (i < na)
	{
		out[i] = a[i];
		i++;
	}
	i = 0;
	while (i < nb)
	{
		out[na + i] = b[i];
		i++;
	}
	i = 0;
	while (i < nc)
	{
		out[na + nb + i] = c[i];
		i++;
	}
	out[na + nb + nc] = '\0';
	return (out);
}

void	free_strv(char **v)
{
	size_t i = 0;
	if (!v)
		return;
	while (v[i])
		free(v[i++]);
	free(v);
}

void	putstr_fd(const char *s, int fd)
{
	if (s)
		write(fd, s, ft_strlen(s));
}

void	puterr_prefix_cmd(const char *prefix, const char *cmd, const char *msg)
{
	putstr_fd(prefix, 2);
	putstr_fd(cmd, 2);
	putstr_fd(": ", 2);
	putstr_fd(msg, 2);
	putstr_fd("\n", 2);
}

void	die_perror(const char *ctx, int code)
{
	perror(ctx);
	exit(code);
}

int	is_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\r' || c == '\v' || c == '\f');
}


void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	size_t				i;
	unsigned char		*d;
	const unsigned char	*s;

	if (!dst && !src)
		return (NULL);
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	i = 0;
	while (i < n)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

/*
** Returns 1 if line is exactly limiter + "\n" or limiter (EOF without \n).
*/
int	ft_streq_limiter(const char *line, const char *limiter)
{
	size_t	ll;
	size_t	ml;

	if (!line || !limiter)
		return (0);
	ml = ft_strlen(limiter);
	ll = ft_strlen(line);
	if (ll == ml && ft_strncmp(line, limiter, ml) == 0)
		return (1);
	if (ll == ml + 1 && line[ll - 1] == '\n' && ft_strncmp(line, limiter, ml) == 0)
		return (1);
	return (0);
}
