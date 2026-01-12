#include "../incs/malloc.h"

t_history		*g_history = NULL;

static void write_str(const char *str)
{
	if (!str)
		return;
	size_t len = 0;
	while (str[len])
		len++;
	write(1, str, len);
}

static void write_hex_byte(unsigned char byte)
{
	char hex[3];
	const char *digits = "0123456789abcdef";
	hex[0] = digits[byte >> 4];
	hex[1] = digits[byte & 0x0f];
	hex[2] = ' ';
	write(1, hex, 3);
}

static void write_hex_ptr(void *ptr)
{
	char buffer[17];
	unsigned long addr = (unsigned long)ptr;
	int i;
	
	for (i = 15; i >= 0; i--)
	{
		int digit = addr & 0x0f;
		buffer[i] = (digit < 10) ? ('0' + digit) : ('a' + digit - 10);
		addr >>= 4;
	}
	write(1, buffer, 16);
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

void show_hex_dump(void *start, void *end)
{
	unsigned char *ptr = (unsigned char *)start;
	unsigned char *end_ptr = (unsigned char *)end;
	size_t bytes_per_line = 16;
	size_t i;

	set_malloc_tracking(0);
	
	write_str("\nHex Dump from 0x");
	write_hex_ptr(start);
	write_str(" to 0x");
	write_hex_ptr(end);
	write_str(" (");
	write_num((size_t)(end_ptr - ptr));
	write_str(" bytes)\n");
	write_str("Address              : Hex Values                                       | ASCII\n");
	write_str("--------------------------------------------------------------------------------------\n");

	while (ptr < end_ptr)
	{
		write_str("0000");
		write_hex_ptr(ptr);
		write_str(" : ");

		for (i = 0; i < bytes_per_line; i++)
		{
			if (ptr + i < end_ptr)
				write_hex_byte(ptr[i]);
			else
				write_str("   ");
			
			if (i == 7)
				write_str(" ");
		}

		write_str("| ");

		for (i = 0; i < bytes_per_line && ptr + i < end_ptr; i++)
		{
			unsigned char c = ptr[i];
			char ch = (c >= 32 && c <= 126) ? c : '.';
			write(1, &ch, 1);
		}

		write_str("\n");
		ptr += bytes_per_line;
	}

	write_str("--------------------------------------------------------------------------------------\n");
	set_malloc_tracking(1);
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
	
	set_malloc_tracking(0);
	write_str("\n");
	write_str(BYEL);
	write_str("=============== ALLOCATION HISTORY ===============");
	write_str(RESET);
	write_str("\n");
	
	if (current == NULL)
	{
		write_str("(No allocations yet)\n");
	}
	
	while (current != NULL)
	{
		count++;
		write_str(BCYN);
		write_str("[");
		write_num(count);
		write_str("]");
		write_str(RESET);
		write_str(" 0x");
		write_hex_ptr(current->start);
		write_str(" -> 0x");
		write_hex_ptr(current->end);
		write_str(" : ");
		write_num(current->size);
		write_str(" bytes\n");
		current = current->next;
	}
	
	write_str(BYEL);
	write_str("==================================================");
	write_str(RESET);
	write_str("\n\n");
	set_malloc_tracking(1);
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