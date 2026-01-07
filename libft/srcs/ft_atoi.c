#include "../incs/libft.h"

long long	ft_atoi(const char *str)
{
	long long	nbr;
	int			sign;

	nbr = 0;
	sign = 1;
	while ((*str >= '\t' && *str <= '\r') || *str == ' ')
		str++;
	if (*str == '-')
	{
		sign = -1;
		str++;
	}
	else if (*str == '+')
		str++;
	while (*str >= '0' && *str <= '9')
	{
		nbr = nbr * 10 + (*str - '0');
		str++;
	}
	return (nbr * sign);
}

// int	main(void)
// {
// 	printf("%lld\n", ft_atoi("-2247483649"));
// 	//printf("%d\n", atoi("-2147483648"));
// 	return (0);
// }
