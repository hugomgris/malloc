#include "../incs/libft.h"

static char	**failure(char **strs)
{
	size_t	i;

	i = 0;
	while (strs[i])
		free(strs[i++]);
	free(strs);
	return (NULL);
}

static size_t	count_words(char const *s, char c)
{
	size_t	i;
	size_t	count;

	count = 0;
	i = 0;
	while (s[i])
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char	**get_strs(char **strs, const char *s, char c)
{
	size_t	len;
	size_t	i;
	size_t	j;

	i = 0;
	j = 0;
	len = 0;
	while (s[i])
	{
		if (s[i] != c)
			len++;
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
		{
			strs[j] = ft_substr(s, (i - len + 1), len);
			if (!strs[j++])
				return (failure(strs));
			len = 0;
		}
		i++;
	}
	strs[j] = NULL;
	return (strs);
}

char	**ft_split(char const *s, char c)
{
	char	**strs;
	size_t	cw;

	cw = count_words(s, c);
	strs = (char **)malloc(sizeof(char *) * (cw + 1));
	if (!strs)
		return (NULL);
	strs = get_strs(strs, s, c);
	return (strs);
}