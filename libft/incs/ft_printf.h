#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <unistd.h>
# include <stdarg.h>

//	FUNCTIONS

int	ft_printf(const char *string, ...);
int	ft_converter(const char *str, int i, va_list args);
int	ft_putchar(int c);
int	ft_putstr(char *str);
int	ft_putnbr(int nbr);
int	ft_putunsigned(unsigned int nbr);
int	ft_putptr(unsigned long ptr, int i);
int	ft_puthexa(unsigned int nbr, char x);

#endif
