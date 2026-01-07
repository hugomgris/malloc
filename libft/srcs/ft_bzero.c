#include "../incs/libft.h"

/* This function works very similarly from ft_memset, except that it replaces
every character with NULL and returns nothing. */

void	ft_bzero(void *b, size_t len)
{
	char	*s;

	s = (char *)b;
	while (len-- > 0)
		*(s++) = '\0';
	return ;
}