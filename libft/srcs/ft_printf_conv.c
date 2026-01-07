#include "../incs/ft_printf.h"

int	ft_putnbr(int nbr)
{
	int	count;

	count = 0;
	if (nbr == -2147483648)
		return (ft_putstr("-2147483648"));
	if (nbr < 0)
	{
		count += ft_putchar('-');
		nbr = -nbr;
	}
	if (nbr > 9)
		count += ft_putnbr(nbr / 10);
	count += ft_putchar("0123456789"[nbr % 10]);
	return (count);
}

int	ft_putunsigned(unsigned int nbr)
{
	int	count;

	count = 0;
	if (nbr > 9)
		count += ft_putunsigned(nbr / 10);
	count += ft_putchar("0123456789"[nbr % 10]);
	return (count);
}

int	ft_putptr(unsigned long ptr, int i)
{
	int	count;

	count = 0;
	if (ptr == 0)
		return (ft_putstr("(nil)"));
	if (i++ == 0)
		count += ft_putstr("0x");
	if (ptr > 15)
		count += ft_putptr(ptr / 16, i);
	count += ft_putchar("0123456789abcdef"[ptr % 16]);
	return (count);
}

int	ft_puthexa(unsigned int nbr, char x)
{
	int		count;
	char	*base;

	count = 0;
	if (x == 'X')
		base = "0123456789ABCDEF";
	else
		base = "0123456789abcdef";
	if (nbr > 15)
		count += ft_puthexa(nbr / 16, x);
	count += ft_putchar(base[nbr % 16]);
	return (count);
}