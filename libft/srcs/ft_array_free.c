#include "../incs/libft.h"

void	ft_array_free(void **array)
{
	int	i;

	if (array == NULL)
		return ;
	i = -1;
	while (array[++i])
		free(array[i]);
	free(array);
	array = NULL;
}
