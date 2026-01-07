#include "../incs/libft.h"

/*	This function simply uses a deleting function to empty the content of a node
	before freeing the pointer to that node.		*/

void	ft_lstdelone(t_list *lst, void (*del)(void*))
{
	(*del)(lst->content);
	free(lst);
	return ;
}
