#include "../incs/ft_malloc.h"

void	*ft_malloc(size_t size)
{
	if (is_debug_mode())
		printf("Allocation of %zu bytes\n", size);

	void *ptr;

	pthread_mutex_lock(&g_malloc_mutex);
	if (size == 0)
	{
		pthread_mutex_unlock(&g_malloc_mutex);
		return NULL;
	}

	size = ALIGN16(size);
	if (size <= TINY_LIMIT)
		ptr = allocate_in_zone(TINY, size);
	else if (size <= SMALL_LIMIT)
		ptr = allocate_in_zone(SMALL, size);
	else
		ptr = allocate_large(size);

	pthread_mutex_unlock(&g_malloc_mutex);
	return ptr;
}
