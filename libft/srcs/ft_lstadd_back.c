#include "../incs/libft.h"

/*	This function takes a new, independent node and add it to the last
	position of an existing list.
	If the new node is empty, the function ends.
	If the existing list is empty, it sets the new node as the first node.
	Otherwise, it uses a temp copy of the list to go to the last node and
	set its pointer "next" to the new node.		*/

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (!new)
		return ;
	if (!*lst)
	{
		*lst = new;
		return ;
	}
	tmp = ft_lstlast(*lst);
	tmp->next = new;
	return ;
}
