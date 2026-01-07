#include "../incs/ft_printf.h"

int	ft_putchar(int c)
{
	return (write(1, &c, 1));
}

int	ft_putstr(char *str)
{
	int	i;
	int	count;

	if (!str)
		return (ft_putstr("(null)"));
	i = 0;
	count = 0;
	while (str[i])
		count += ft_putchar(str[i++]);
	return (count);
}

int	ft_converter(const char *str, int i, va_list args)
{
	if (str[i + 1] == 'c')
		return (ft_putchar(va_arg(args, int)));
	else if (str[i + 1] == 's')
		return (ft_putstr(va_arg(args, char *)));
	if (str[i + 1] == 'p')
		return (ft_putptr(va_arg(args, unsigned long), 0));
	if (str[i + 1] == 'd' || str[i + 1] == 'i')
		return (ft_putnbr(va_arg(args, int)));
	if (str[i + 1] == 'u')
		return (ft_putunsigned(va_arg(args, unsigned int)));
	if (str[i + 1] == 'x' || str[i + 1] == 'X')
		return (ft_puthexa(va_arg(args, unsigned int), str[i + 1]));
	if (str[i + 1] == '%')
		return (ft_putchar(str[i]));
	return (0);
}
