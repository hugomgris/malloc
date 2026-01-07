#include "../incs/libft.h"

/* Simply returns 1 if the character is alphabetical
or 0 if it is not.
*/
int	ft_isalpha(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'))
		return (1);
	else
		return (0);
}
