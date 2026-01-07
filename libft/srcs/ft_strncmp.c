#include "../incs/libft.h"

int	ft_strncmp(const char *s1, const char *s2, size_t len)
{
	unsigned char	*cpy1;
	unsigned char	*cpy2;
	size_t			i;

	cpy1 = (unsigned char *)s1;
	cpy2 = (unsigned char *)s2;
	i = 0;
	if (len == 0)
		return (0);
	while ((cpy1[i] || cpy2[i]) && i < len)
	{
		if (cpy1[i] != cpy2[i])
			return (cpy1[i] - cpy2[i]);
		i++;
	}
	return (0);
}
