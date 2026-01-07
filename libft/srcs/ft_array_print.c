#include "../incs/libft.h"

void	ft_array_print(char **array)
{
	if (array)
	{
		for (int i = 0; array[i]; i++) {
			ft_printf("%s\n", array[i]);
		}
	}

	/* int	i;s

	i = -1;
	while (array[++i])
	{
		ft_printf("%s\n", array[i]);
		array[i + 1] ? ft_printf(" ") : ft_printf(" ");
	}
	ft_printf("\n"); */
}
