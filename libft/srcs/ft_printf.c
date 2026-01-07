#include "../incs/ft_printf.h"

int	ft_printf(const char *string, ...)
{
	va_list	args;
	int		i;
	int		count;

	count = 0;
	i = 0;
	va_start(args, string);
	while (string[i] != '\0')
	{
		if (string[i] == '%')
		{
			count += ft_converter(string, i, args);
			i += 2;
		}
		else
			count += ft_putchar(string[i++]);
	}
	va_end(args);
	return (count);
}
