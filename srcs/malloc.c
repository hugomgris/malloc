#include "../incs/malloc.h"

static __thread int in_malloc = 0;

void	set_malloc_tracking(int enable)
{
	in_malloc = !enable;
}

void	*malloc(size_t size)
{
	void *ptr;
	int should_track = !in_malloc;
	int should_lock = !in_malloc;

	if (should_lock)
		pthread_mutex_lock(&g_malloc_mutex);
		
	if (size == 0)
	{
		if (should_lock)
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

	if (should_lock)
		pthread_mutex_unlock(&g_malloc_mutex);

	if (ptr && should_track)
	{
		in_malloc = 1;
		
		void *end = (char *)ptr + size;
		
		if (is_debug_mode())
		{
			char msg[256];
			int len = snprintf(msg, sizeof(msg), "%s[DEBUG] Allocated %zu bytes at %p%s\n", BGRN, size, ptr, RESET);
			if (len > 0)
				write(2, msg, len);
		}
		
		if (is_bonus_mode())
		{
			append_to_history(ptr, end, size);
		}
		
		in_malloc = 0;
	}

	return ptr;
}
