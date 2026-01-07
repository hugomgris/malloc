#include "../incs/ft_malloc.h"

int main(void)
{
	printf(BGRN);
	printf("***** TEST 1: BASIC TEST *****\n");
	printf(RESET);

	char	*a = (char *)ft_malloc(64);
	char	*b = (char *)ft_malloc(128);
	char	*c = (char *)ft_malloc(2500);

	ft_strcpy(a, "Holiwi cucufu");
	ft_strcpy(b, "small, not tiny");
	printf("%sa: %s\n%s", BLU, a, RESET);
	printf("%sb: %s\n%s", BLU, b, RESET);

	printf(BGRN);
	printf("\n***** TEST 2: FT_SHOW_ALLOC_MEM TEST *****\n");
	printf(RESET);
	ft_show_alloc_mem();

	printf(BGRN);
	printf("\n***** TEST 3: FREEING AND DEFRAGMENTATION TEST *****\n");
	printf(RESET);
	ft_free(a);
	ft_free(b);
	ft_free(c);

	printf(BGRN);
	printf("\n***** TEST 4: FT_SHOW_ALLOC_MEM_EX TEST *****\n");
	printf(RESET);
	ft_show_alloc_mem_ex();

	printf(BGRN);
	printf("\n***** TEST 5: FT_REALLOC TEST *****\n");
	printf(RESET);
	char	*d = (char *)ft_malloc(100);
	ft_strcpy(d, "this is a realloc test");
	d = ft_realloc(d, 200);
	printf("a after ft_realloc: %s\n", d);

	printf(BGRN);
	printf("\n***** TEST 6: FT_SHOW_ALLOC_MEM_EX STATE TEST *****\n");
	printf(RESET);
	ft_show_alloc_mem_ex();

	return 0;
}