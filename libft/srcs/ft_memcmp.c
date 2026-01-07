#include "../incs/libft.h"

int	ft_memcmp(const void *b1, const void *b2, size_t len)
{
	unsigned char	*cpy1;
	unsigned char	*cpy2;
	size_t			i;

	i = 0;
	cpy1 = (unsigned char *)b1;
	cpy2 = (unsigned char *)b2;
	while (i < len)
	{
		if (cpy1[i] != cpy2[i])
			return (cpy1[i] - cpy2[i]);
		i++;
	}
	return (0);
}