#include "./incs/malloc.h"

int main(void)
{
	printf(BGRN);
	printf("***** TEST 1: BASIC TEST *****\n");
	printf(RESET);

	char	*a = (char *)malloc(64);
	char	*b = (char *)malloc(128);
	char	*c = (char *)malloc(2500);

	ft_strcpy(a, "Holiwi cucufu");
	ft_strcpy(b, "small, not tiny");
	printf("%sa: %s\n%s", BLU, a, RESET);
	printf("%sb: %s\n%s", BLU, b, RESET);

	printf(BGRN);
	printf("\n***** TEST 2: show_alloc_mem TEST *****\n");
	printf(RESET);
	show_alloc_mem();

	printf(BGRN);
	printf("\n***** TEST 3: FREEING AND DEFRAGMENTATION TEST *****\n");
	printf(RESET);
	free(a);
	free(b);
	free(c);

	printf(BGRN);
	printf("\n***** TEST 4: show_alloc_mem_ex TEST *****\n");
	printf(RESET);
	show_alloc_mem_ex();

	printf(BGRN);
	printf("\n***** TEST 5: realloc TEST *****\n");
	printf(RESET);
	char	*d = (char *)malloc(100);
	ft_strcpy(d, "this is a realloc test");
	d = realloc(d, 200);
	printf("a after realloc: %s\n", d);

	printf(BGRN);
	printf("\n***** TEST 6: show_alloc_mem_ex STATE TEST *****\n");
	printf(RESET);
	show_alloc_mem_ex();

	return 0;
}