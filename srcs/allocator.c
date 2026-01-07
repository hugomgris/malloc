#include "../incs/ft_malloc.h"

static size_t get_zone_size(t_zone_type type)
{
	if (type == TINY)
		return PAGE_SIZE * 4;
	else if (type == SMALL)
		return PAGE_SIZE * 32;
	else
		return 0;
}

static t_zone	*create_zone(t_zone_type type, size_t size)
{
	size_t	zone_size = get_zone_size(type);
	t_zone	*zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (zone == MAP_FAILED)
		return NULL;

	zone->type = type;
	zone->size = zone_size;
	zone->next = NULL;
	zone->mem = (void *)zone + sizeof(t_zone);

	t_block	*block = (t_block *)zone->mem;
	block->size = size;
	block->free = 0;
	block->next = NULL;

	zone->blocks = block;

	return zone;
}

void	*allocate_in_zone(t_zone_type type, size_t size)
{
	t_zone	*zone = g_zones;

	// If zone alredy exists, manage through global
	while (zone)
	{
		if (zone->type == type)
		{
			t_block *block = zone->blocks;

			while (block)
			{
				if (block->free && block->size >= size)
				{
					block->free = 0;
					return (void *)(block + 1);
				}
				block = block->next;
			}
		}
		zone = zone->next;
	}

	// Else, create a new zone
	t_zone	*nzone = create_zone(type, size);
	if (nzone == NULL)
		return NULL;

	nzone->next = g_zones;
	g_zones = nzone;
	return (void *)(nzone->blocks + 1);
}

void	*allocate_large(size_t size)
{
	size_t	total_size = sizeof(t_zone) + sizeof(t_block) + size;
	t_zone	*zone = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (zone == MAP_FAILED)
		return NULL;

	zone->type = LARGE;
	zone->size = total_size;
	zone->next = g_zones;
	g_zones = zone;

	zone->mem = (void *)zone + sizeof(t_zone);
	t_block *block = (t_block *)zone->mem;
	block->size = size;
	block->free = 0;
	block->next = NULL;

	zone->blocks = block;
	
	return (void *)(block + 1);
}