#include "../incs/libft.h"

/*	This function looks for the first occurence of c in the string s.
 *	It returns the address of the occurence if found, or NULL if no
 *	occurence is found.
 *	If the character to find is '\0', the function basically returns 
 *	the end of the string.
 *	Important: the int c has to be converted into a char for the search. */

char	*ft_strchr(const char *s, int c)
{
	char		cc;

	cc = (char) c;
	while (*s)
	{
		if (*s == cc)
			return ((char *) s);
		s++;
	}
	if (*s == cc)
		return ((char *) s);
	return (NULL);
}
