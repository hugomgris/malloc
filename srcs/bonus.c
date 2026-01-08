#include "../incs/malloc.h"

t_history		*g_history = NULL;

void show_hex_dump(void *start, void *end)
{
	unsigned char *ptr = (unsigned char *)start;
	unsigned char *end_ptr = (unsigned char *)end;
	size_t bytes_per_line = 16;
	size_t i;

	printf("\nHex Dump from %p to %p (%zu bytes)\n", start, end, (size_t)(end_ptr - ptr));
	printf("Address          : Hex Values                                       | ASCII\n");
	printf("--------------------------------------------------------------------------------------\n");

	while (ptr < end_ptr)
	{
		printf("%016lx : ", (unsigned long)ptr);

		for (i = 0; i < bytes_per_line; i++)
		{
			if (ptr + i < end_ptr)
				printf("%02x ", ptr[i]);
			else
				printf("   ");
			
			if (i == 7)
				printf(" ");
		}

		printf("| ");

		for (i = 0; i < bytes_per_line && ptr + i < end_ptr; i++)
		{
			unsigned char c = ptr[i];
			if (c >= 32 && c <= 126)
				printf("%c", c);
			else
				printf(".");
		}

		printf("\n");
		ptr += bytes_per_line;
	}

	printf("--------------------------------------------------------------------------------------\n");
}

void	append_to_history(void *start, void *end, size_t size)
{
	t_history *new_entry = (t_history *)mmap(NULL, sizeof(t_history), 
		PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	
	if (new_entry == MAP_FAILED)
		return;
	
	new_entry->start = start;
	new_entry->end = end;
	new_entry->size = size;
	new_entry->freed = 0;
	new_entry->next = NULL;
	
	if (g_history == NULL)
	{
		g_history = new_entry;
	}
	else
	{
		t_history *current = g_history;
		while (current->next != NULL)
			current = current->next;
		current->next = new_entry;
	}
}

void show_malloc_history()
{
	t_history *current = g_history;
	int count = 0;
	
	printf("\n%s=============== ALLOCATION HISTORY ===============%s\n", BYEL, RESET);
	
	if (current == NULL)
	{
		printf("(No allocations yet)\n");
	}
	
	while (current != NULL)
	{
		count++;
		printf("%s[%d]%s %p -> %p : %zu bytes\n", 
			BCYN, count, RESET, current->start, current->end, current->size);
		current = current->next;
	}
	
	printf("%s==================================================%s\n\n", BYEL, RESET);
}

int	is_bonus_mode(void)
{
	char	*bnm = getenv("MALLOC_BONUS");
	if (bnm && bnm[0] == '1')
		return 1;
	return 0;
}

void	mark_as_freed(void *ptr)
{
	if (!ptr || !is_bonus_mode())
		return;
	
	t_history *current = g_history;
	t_history *last_match = NULL;
	
	// Find the LAST (most recent) entry with this pointer
	// This handles cases where the allocator reuses freed addresses
	while (current != NULL)
	{
		if (current->start == ptr && !current->freed)
			last_match = current;
		current = current->next;
	}
	
	if (last_match != NULL)
		last_match->freed = 1;
}

void	check_memory_leaks(void)
{
	if (!is_bonus_mode() || g_history == NULL)
		return;
	
	t_history *current = g_history;
	int leak_count = 0;
	size_t total_leaked = 0;
	
	while (current != NULL)
	{
		if (!current->freed)
		{
			leak_count++;
			total_leaked += current->size;
		}
		current = current->next;
	}
	
	if (leak_count > 0)
	{
		char msg[256];
		int len;
		
		len = snprintf(msg, sizeof(msg), "\n%s========== MEMORY LEAK REPORT ==========%s\n", BRED, RESET);
		if (len > 0)
			write(2, msg, len);
		
		len = snprintf(msg, sizeof(msg), "%s%d block%s not freed (total: %zu bytes)%s\n", 
			BYEL, leak_count, (leak_count > 1 ? "s" : ""), total_leaked, RESET);
		if (len > 0)
			write(2, msg, len);
		
		current = g_history;
		while (current != NULL)
		{
			if (!current->freed)
			{
				len = snprintf(msg, sizeof(msg), "%s[LEAK]%s %p - %p : %zu bytes\n",
					BRED, RESET, current->start, current->end, current->size);
				if (len > 0)
					write(2, msg, len);
			}
			current = current->next;
		}
		
		len = snprintf(msg, sizeof(msg), "%s========================================%s\n\n", BRED, RESET);
		if (len > 0)
			write(2, msg, len);
	}
	else
	{
		char msg[256];
		int len = snprintf(msg, sizeof(msg), "\n%s✓ No memory leaks detected%s\n\n", BGRN, RESET);
		if (len > 0)
			write(2, msg, len);
	}
}

__attribute__((destructor))
static void cleanup_and_check_leaks(void)
{
	check_memory_leaks();
}