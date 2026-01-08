#include "./incs/malloc.h"

int main(void)
{
	char msg[256];
	int len = snprintf(msg, sizeof(msg), "%s\n***** TEST 1: BASIC TEST *****\n%s", BGRN, RESET);
	if (len > 0)
		write(2, msg, len);

	char	*a = (char *)malloc(64);
	char	*b = (char *)malloc(128);
	char	*c = (char *)malloc(2500);

	ft_strcpy(a, "Holiwi cucufu");
	ft_strcpy(b, "small, not tiny");
	printf("%sa: %s\n%s", BLU, a, RESET);
	printf("%sb: %s\n%s", BLU, b, RESET);

	len = snprintf(msg, sizeof(msg), "%s\n***** TEST 2: show_alloc_mem TEST *****\n%s", BGRN, RESET);
	if (len > 0)
		write(2, msg, len);
	show_alloc_mem();

	len = snprintf(msg, sizeof(msg), "%s\n***** TEST 3: FREEING AND DEFRAGMENTATION TEST *****\n%s", BGRN, RESET);
	if (len > 0)
		write(2, msg, len);
	free(a);
	free(b);
	free(c);

	len = snprintf(msg, sizeof(msg), "%s\n***** TEST 4: show_alloc_mem_ex TEST *****\n%s", BGRN, RESET);
	if (len > 0)
		write(2, msg, len);
	show_alloc_mem_ex();

	len = snprintf(msg, sizeof(msg), "%s\n***** TEST 5: realloc TEST *****\n%s", BGRN, RESET);
	if (len > 0)
		write(2, msg, len);
	char	*d = (char *)malloc(100);
	ft_strcpy(d, "this is a realloc test");
	d = realloc(d, 200);
	printf("a after realloc: %s\n", d);

	len = snprintf(msg, sizeof(msg), "%s\n***** TEST 6: show_alloc_mem_ex STATE TEST *****\n%s", BGRN, RESET);
	if (len > 0)
		write(2, msg, len);
	show_alloc_mem_ex();

	return 0;
}