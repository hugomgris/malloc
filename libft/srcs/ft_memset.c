#include "../incs/libft.h"

/* This function takes a pointer to void (i.e. to any type of bytes), makes a
temporary copy of it and iterates the copy to replace the current byte by c
before returning the original pointer */

void	*ft_memset(void *dest, int c, size_t len)
{
	unsigned char	*cpy;

	cpy = (unsigned char *)dest;
	while (len-- > 0)
		*(cpy++) = (unsigned char)c;
	return (dest);
}
