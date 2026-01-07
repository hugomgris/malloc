#include "../incs/ft_malloc.h"

void	*ft_realloc(void *ptr, size_t size)
{
	if (size == 0)
	{
		ft_free(ptr);
		return NULL;
	}

	if (ptr == NULL)
		return ft_malloc(size);

	size = ALIGN16(size);

	t_block *block = (t_block *)ptr - 1;

	if (block->size >= size)
		return ptr;

	void *new_ptr = ft_malloc(size);
	if (new_ptr)
	{
		ft_memcpy(new_ptr, ptr, block->size);
		ft_free(ptr);
	}
	
	return new_ptr;
}