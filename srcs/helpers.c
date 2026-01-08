#include "../incs/malloc.h"

void	*ft_memcpy(void *dst, const void *src, size_t len)
{
	size_t		i;
	char		*d;
	const char	*s;

	if (dst == (void *)0 && src == (void *)0)
		return (NULL);
	d = (char *)dst;
	s = (const char *)src;
	i = 0;
	while (i < len)
	{
		d[i] = s[i];
		i++;
	}
	return (dst);
}

char	*ft_strcpy(char *dst, const char *src)
{
    if (dst == NULL || src == NULL)
        return NULL;

    int i;

    i = 0;
    while (src[i] != '\0')
    {
        dst[i] = src[i];
        i++;
    }
    dst[i] = '\0';
    return (dst);
}