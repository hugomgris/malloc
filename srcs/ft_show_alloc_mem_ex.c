#include "../incs/ft_malloc.h"

static void print_block_ex(t_block *block)
{
	void	*start = (void *)(block + 1);
	void	*end = (char *)start + block->size;

	printf("  [%s] %p - %p : %zu bytes\n", block->free ? "ft_free" : "USED",
           start, end, block->size);
}

void	ft_show_alloc_mem_ex(void)
{
	pthread_mutex_lock(&g_malloc_mutex);

	t_zone	*zone = g_zones;
	size_t	total_used = 0;
	size_t	total_freed = 0;

	while (zone)
	{
		const char	*label;

		if (zone->type == TINY)
			label = "TINY";
		else if (zone->type == SMALL)
			label = "SMALL";
		else
			label = "LARGE";

		printf("%s%s ZONE @ %p | total zone size: %zu bytes%s\n", BYEL, label, zone, zone->size, RESET);

		t_block	*block = zone->blocks;

		while (block)
		{
			print_block_ex(block);

			if (block->free)
				total_freed += block->size;
			else
				total_used += block->size;

			block = block->next;
		}

		zone = zone->next;
	}

	printf("%sTOTAL USED : %zu bytes%s\n", GRNHB, total_used, RESET);
	printf("%sTOTAL FREED : %zu bytes%s\n", CYNHB, total_freed, RESET);

	pthread_mutex_unlock(&g_malloc_mutex);
}