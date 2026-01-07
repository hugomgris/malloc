#include "../incs/libft.h"

void	ft_lstclear(t_list **lst, void (*del)(void *))
{
	t_list	*tmp;

	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		(*ft_lstdelone)(*lst, del);
		*lst = tmp;
	}
	free(*lst);
	*lst = NULL;
}