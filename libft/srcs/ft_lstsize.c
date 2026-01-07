#include "../incs/libft.h"

int	ft_lstsize(t_list *lst)
{
	t_list	*somersault;
	int		i;

	somersault = lst;
	i = 0;
	while (somersault != NULL)
	{
		somersault = somersault->next;
		i++;
	}
	return (i);
}