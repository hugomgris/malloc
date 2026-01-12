#include "../incs/malloc.h"

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

void show_alloc_mem(void)
{
    pthread_mutex_lock(&g_malloc_mutex);
    set_malloc_tracking(0);

    t_zone *zone = g_zones;
    size_t total = 0;

    while (zone)
    {
        const char *label = (zone->type == TINY) ? "TINY" :
                            (zone->type == SMALL) ? "SMALL" : "LARGE";

        write_str(BMAG);
        write_str(label);
        write_str(" : ");
        write_ptr(zone);
        write_str(RESET);
        write_str("\n");

        t_block *block = zone->blocks;
        while (block)
        {
            if (!block->free)
            {
                void *start = (void *)(block + 1);
                void *end = (char *)start + block->size;
                
                write_ptr(start);
                write_str(" - ");
                write_ptr(end);
                write_str(" : ");
                write_num(block->size);
                write_str(" bytes\n");
                
                total += block->size;
            }

            block = block->next;
        }

        zone = zone->next;
    }

    write_str("TOTAL : ");
    write_num(total);
    write_str(" bytes\n");

    set_malloc_tracking(1);
    pthread_mutex_unlock(&g_malloc_mutex);
}