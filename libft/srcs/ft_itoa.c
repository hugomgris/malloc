#include "../incs/libft.h"

static int	intlen(int n)
{
	int		l;

	l = 1;
	if (n < 0)
		l++;
	while ((n / 10) != 0)
	{
		n = n / 10;
		l++;
	}
	return (l);
}

char *ft_itoa(int n)
{
	char *str;
	int len;
	int is_negative = 0;

	if (n == -2147483648)
		return (ft_strdup("-2147483648"));
	
	if (n < 0)
	{
		is_negative = 1;
		n = -n;
	}
	
	len = intlen(n) + is_negative;
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	
	str[len] = '\0';
	len--;
	
	while (n > 0)
	{
		str[len] = (n % 10) + '0';
		n = n / 10;
		len--;
	}
	
	if (is_negative)
		str[0] = '-';
	
	return (str);
}