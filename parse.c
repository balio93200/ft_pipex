#include "pipex.h"

static size_t	count_tokens(const char *s)
{
	size_t i;
	size_t count;
	char q;

	i = 0;
	count = 0;
	while (s && s[i])
	{
		while (s[i] && is_space(s[i]))
			i++;
		if (!s[i])
			break;
		count++;
		q = 0;
		while (s[i] && (!is_space(s[i]) || q))
		{
			if (!q && (s[i] == '\'' || s[i] == '"'))
				q = s[i++];
			else if (q && s[i] == q)
				q = 0, i++;
			else
				i++;
		}
	}
	return (count);
}

static size_t	token_len(const char *s, size_t i)
{
	size_t len;
	char q;

	len = 0;
	q = 0;
	while (s[i] && (!is_space(s[i]) || q))
	{
		if (!q && (s[i] == '\'' || s[i] == '"'))
			q = s[i++];
		else if (q && s[i] == q)
			q = 0, i++;
		else
			len++, i++;
	}
	return (len);
}

static char	*extract_token(const char *s, size_t *i_ptr)
{
	size_t i;
	size_t k;
	size_t len;
	char q;
	char *out;

	i = *i_ptr;
	len = token_len(s, i);
	out = (char *)malloc(len + 1);
	if (!out)
		return (NULL);
	k = 0;
	q = 0;
	while (s[i] && (!is_space(s[i]) || q))
	{
		if (!q && (s[i] == '\'' || s[i] == '"'))
			q = s[i++];
		else if (q && s[i] == q)
			q = 0, i++;
		else
			out[k++] = s[i++];
	}
	out[k] = '\0';
	*i_ptr = i;
	return (out);
}

static void	free_partial(char **v, size_t n)
{
	size_t i = 0;
	while (i < n)
		free(v[i++]);
	free(v);
}

char	**split_cmd(const char *s)
{
	char **out;
	size_t tokc;
	size_t i = 0;
	size_t k = 0;

	tokc = count_tokens(s);
	out = (char **)malloc(sizeof(char *) * (tokc + 1));
	if (!out)
		return (NULL);
	while (k < tokc)
	{
		while (s[i] && is_space(s[i]))
			i++;
		out[k] = extract_token(s, &i);
		if (!out[k])
			return (free_partial(out, k), (char **)0);
		k++;
		while (s[i] && is_space(s[i]))
			i++;
	}
	out[k] = NULL;
	return (out);
}
