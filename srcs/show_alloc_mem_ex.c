#include "../incs/malloc.h"

static __thread int in_malloc = 0;

static void write_str(const char *str)
{
	if (!str)
		return;
	size_t len = 0;
	while (str[len])
		len++;
	write(1, str, len);
}

static void write_ptr(void *ptr)
{
	char buffer[19];
	unsigned long addr = (unsigned long)ptr;
	int i = 17;
	
	buffer[18] = '\0';
	buffer[0] = '0';
	buffer[1] = 'x';
	
	if (addr == 0)
	{
		buffer[2] = '0';
		buffer[3] = '\0';
		write_str(buffer);
		return;
	}
	
	while (addr > 0 && i >= 2)
	{
		int digit = addr % 16;
		buffer[i--] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
		addr /= 16;
	}
	
	write_str(buffer + i + 1);
}

static void write_num(size_t num)
{
	char buffer[32];
	int i = 30;
	
	buffer[31] = '\0';
	
	if (num == 0)
	{
		write_str("0");
		return;
	}
	
	while (num > 0 && i >= 0)
	{
		buffer[i--] = '0' + (num % 10);
		num /= 10;
	}
	
	write_str(buffer + i + 1);
}

static void print_block_ex(t_block *block)
{
	void	*start = (void *)(block + 1);
	void	*end = (char *)start + block->size;

	write_str("  [");
	write_str(block->free ? "free" : "USED");
	write_str("] ");
	write_ptr(start);
	write_str(" - ");
	write_ptr(end);
	write_str(" : ");
	write_num(block->size);
	write_str(" bytes\n");

	// Only show hex dump if bonus mode is enabled
	if (!in_malloc && is_bonus_mode())
	{
		in_malloc = 1;
		show_hex_dump(start, end);
		in_malloc = 0;
	}
}

void	show_alloc_mem_ex(void)
{
	pthread_mutex_lock(&g_malloc_mutex);
	set_malloc_tracking(0);

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

		// Calculate used and free space in this zone
		size_t zone_used = 0;
		size_t zone_free = 0;
		t_block *temp_block = zone->blocks;
		while (temp_block)
		{
			if (temp_block->free)
				zone_free += temp_block->size + sizeof(t_block);
			else
				zone_used += temp_block->size + sizeof(t_block);
			temp_block = temp_block->next;
		}

		write_str(BYEL);
		write_str(label);
		write_str(" ZONE @ ");
		write_ptr(zone);
		write_str(" | capacity: ");
		write_num(zone->size);
		write_str(" bytes (used: ");
		write_num(zone_used);
		write_str(", free: ");
		write_num(zone_free);
		write_str(")");
		write_str(RESET);
		write_str("\n");

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

	write_str(GRNHB);
	write_str("TOTAL USED : ");
	write_num(total_used);
	write_str(" bytes");
	write_str(RESET);
	write_str("\n");
	
	write_str(CYNHB);
	write_str("TOTAL FREED : ");
	write_num(total_freed);
	write_str(" bytes");
	write_str(RESET);
	write_str("\n");

	set_malloc_tracking(1);
	pthread_mutex_unlock(&g_malloc_mutex);
}