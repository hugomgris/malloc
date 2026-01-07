#include "../incs/libft.h"

/*	This function creates a new list based on an existing one:
	- First, security check that parameters are not null
	- Second, iterate using "lst = lst->next" until last node of old list
	- Use the ft_lstnew to create new node while applying the f function
	- Security if malloc fails: clear the new list and return NULL
	- Put the newest node to the back of the list
	- Return the pointer to new_list	*/

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*new_list;
	t_list	*new_node;

	if (!lst || !f || !del)
		return (NULL);
	new_list = NULL;
	while (lst)
	{
		new_node = ft_lstnew((*f)(lst->content));
		if (!new_node)
		{
			ft_lstclear(&new_list, del);
			return (NULL);
		}
		ft_lstadd_back(&new_list, new_node);
		lst = lst->next;
	}
	return (new_list);
}