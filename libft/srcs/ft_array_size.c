#include "../incs/libft.h"

size_t	ft_array_size(void **array)
{
	size_t	i = 0;

	while (array[i])
	{
		i++;
	}
	
	return i;
}