#include "../incs/ft_malloc.h"

int	is_debug_mode(void)
{
	char	*dbg = getenv("MALLOC_DEBUG");
	if (dbg && dbg[0] == '1')
		return 1;
	return 0;
}