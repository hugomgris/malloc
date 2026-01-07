#include "../incs/libft.h"

void	*ft_memchr(const void *s, int c, size_t len)
{
	const unsigned char	*cpy;
	unsigned char		cc;

	cpy = (const unsigned char *)s;
	cc = (unsigned char)c;
	while (len > 0)
	{
		if (*cpy == cc)
			return ((void *)cpy);
		cpy++;
		len--;
	}
	return (NULL);
}
