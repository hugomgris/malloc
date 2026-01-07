#include "../incs/libft.h"

size_t	ft_strlcpy(char *dst, const char *src, size_t dstsize)
{
	size_t	l_src;

	l_src = ft_strlen(src);
	if (l_src + 1 < dstsize)
		ft_memcpy(dst, src, l_src + 1);
	else if (dstsize != 0)
	{
		ft_memcpy(dst, src, dstsize - 1);
		dst[dstsize - 1] = '\0';
	}
	return (l_src);
}
