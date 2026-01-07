#include "../incs/libft.h"

/*	This function uses a temporary pointer as a copy of a pointer to a list
	and iterates the list with the while loop until it reaches the end
	of the list, i.e. the node whose pointer "next" points to null.
	It then returns a pointer to that node.	*/

t_list	*ft_lstlast(t_list *lst)
{
	t_list	*tmp;

	if (lst == NULL)
		return (NULL);
	tmp = lst;
	while (tmp->next)
		tmp = tmp->next;
	return (tmp);
}

