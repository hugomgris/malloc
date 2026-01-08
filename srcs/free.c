#include "../incs/malloc.h"

static void join_free_blocks(t_zone *zone)
{
	t_block *current = zone->blocks;

	while (current && current->next)
	{
		if (current->free && current->next->free)
		{
			current->size += sizeof(t_block) + current->next->size;
			current->next = current->next->next;
		}
		else
		{
			current = current->next;
		}
	}
}

void	free(void *ptr)
{
	if (ptr == NULL)
		return;

	pthread_mutex_lock(&g_malloc_mutex);

	t_block	*block = (t_block *)ptr - 1;
	block->free = 1;

	t_zone	*zone = g_zones;

	while (zone)
	{
		t_block *current = zone->blocks;
		while (current)
		{
			if (current == block)
			{
				join_free_blocks(zone);
				break;
			}
			current = current->next;
		}
		zone = zone->next;
	}

	pthread_mutex_unlock(&g_malloc_mutex);
	
	mark_as_freed(ptr);
}