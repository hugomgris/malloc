#include "./incs/malloc.h"

static void write_str(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	write(1, str, len);
}

static void write_err(const char *str)
{
	size_t len = 0;
	while (str[len])
		len++;
	write(2, str, len);
}

int main(void)
{
	write_err(BGRN);
	write_err("\n***** TEST 1: BASIC TEST *****\n");
	write_err(RESET);

	char	*a = (char *)malloc(64);
	char	*b = (char *)malloc(129);
	char	*c = (char *)malloc(1025);

	ft_strcpy(a, "Holiwi cucufu");
	ft_strcpy(b, "small, not tiny");
	
	write_str(BLU);
	write_str("a: ");
	write_str(a);
	write_str("\n");
	write_str(RESET);
	
	write_str(BLU);
	write_str("b: ");
	write_str(b);
	write_str("\n");
	write_str(RESET);

	write_err(BGRN);
	write_err("\n***** TEST 2: show_alloc_mem TEST *****\n");
	write_err(RESET);
	show_alloc_mem();

	write_err(BGRN);
	write_err("\n***** TEST 3: FREEING AND DEFRAGMENTATION TEST *****\n");
	write_err(RESET);
	free(a);
	free(b);
	free(c);

	write_err(BGRN);
	write_err("\n***** TEST 4: show_alloc_mem_ex TEST *****\n");
	write_err(RESET);
	show_alloc_mem_ex();

	write_err(BGRN);
	write_err("\n***** TEST 5: realloc TEST *****\n");
	write_err(RESET);
	char	*d = (char *)malloc(100);
	ft_strcpy(d, "this is a realloc test");
	d = realloc(d, 200);
	
	write_str("d after realloc: ");
	write_str(d);
	write_str("\n");

	write_err(BGRN);
	write_err("\n***** TEST 6: show_alloc_mem_ex STATE TEST *****\n");
	write_err(RESET);
	show_alloc_mem_ex();
	free(d);

	return 0;
}