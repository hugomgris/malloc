#include "../incs/malloc.h"

void	*realloc(void *ptr, size_t size)
{
	if (size == 0)
	{
		free(ptr);
		return NULL;
	}

	if (ptr == NULL)
		return malloc(size);

	size = ALIGN16(size);

	t_block *block = (t_block *)ptr - 1;

	if (block->size >= size)
		return ptr;

	void *new_ptr = malloc(size);
	if (new_ptr)
	{
		ft_memcpy(new_ptr, ptr, block->size);
		free(ptr);  // This will mark the old ptr as freed
	}
	
	return new_ptr;
}