#ifndef LIBFT_H
# define LIBFT_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 42
# endif

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <ctype.h>
# include <string.h>
# include <stdarg.h>
# include "ft_printf.h"

typedef struct s_list
{
	void			*content;
	struct s_list	*next;
}	t_list;

// LIBC FUNCTIONS

int			ft_isalpha(int c);
int			ft_isdigit(int c);
int			ft_isxdigit(int c);
int			ft_isalnum(int c);
int			ft_isascii(int c);
int			ft_isprint(int c);
size_t		ft_strlen(const char *s);
void		*ft_memset(void *dest, int c, size_t len);
void		ft_bzero(void *b, size_t len);
void		*ft_memcpy(void *dst, const void *src, size_t len);
void		*ft_memmove(void *dest, const void *src, size_t len);
char		*ft_strcpy(char *dst, const char *src);
size_t		ft_strlcpy(char *dst, const char *src, size_t size);
size_t		ft_strlcat(char *dst, const char *src, size_t size);
int			ft_toupper(int c);
int			ft_tolower(int c);
char		*ft_strchr(const char *s, int c);
char		*ft_strrchr(const char *s, int c);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t len);
void		*ft_memchr(const void *s, int c, size_t len);
int			ft_memcmp(const void *b1, const void *b2, size_t len);
char		*ft_strnstr(const char *big, const char *little, size_t len);
long long	ft_atoi(const char *str);
int			ft_atoi_base(char *str, int base);

// USING MALLOC

void		*ft_calloc(size_t nmemb, size_t size);
char		*ft_strdup(char *str);
char		*ft_strndup(char *str, int n);

// ADDITIONAL FUNCTIONS

char		*ft_substr(char const *s, unsigned int start, size_t len);
char		*ft_strjoin(char *s1, char const *s2);
char		*ft_strjoin_free(char *s1, char const *s2);
char		*ft_strtrim(char const *s1, char const *set);
char		**ft_split(char const *s, char c);
char		*ft_itoa(int n);
char		*ft_strmapi(char const *s, char (*f)(unsigned int, char));
void		ft_striteri(char *s, void (*f)(unsigned int, char*));
void		ft_putchar_fd(char c, int fd);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
void		ft_putnbr_fd(int n, int fd);

// LISTS

t_list		*ft_lstnew(void *content);
void		ft_lstadd_front(t_list **lst, t_list *new);
int			ft_lstsize(t_list *lst);
t_list		*ft_lstlast(t_list *lst);
void		ft_lstadd_back(t_list **lst, t_list *new);
void		ft_lstdelone(t_list *lst, void (*del)(void*));
void		ft_lstclear(t_list **lst, void (*del)(void*));
void		ft_lstiter(t_list *lst, void (*f)(void *));
t_list		*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *));
void		ft_list_sort(t_list **lst, int (*)(struct s_list *, struct s_list *));
void		ft_list_reverse(t_list **lst, int (*)(struct s_list *, struct s_list *));
int			ft_lst_cmp(t_list *first, t_list *second);

// FT_PRINTF

int			ft_printf(const char *string, ...);
int			ft_converter(const char *str, int i, va_list args);
int			ft_putchar(int c);
int			ft_putstr(char *str);
int			ft_putnbr(int nbr);
int			ft_putunsigned(unsigned int nbr);
int			ft_putptr(unsigned long ptr, int i);
int			ft_puthexa(unsigned int nbr, char x);

// GET NEXT LINE

char		*get_next_line(int fd);
char		*ft_gnl_free(char **stash);
char		*ft_fill_line(char *stash);
char		*ft_read_buffer(int fd, char *stash, char *buffer);

// ARRAY
void		ft_array_free(void **array);
void		ft_array_print(char **array);
size_t		ft_array_size(void **array);

//	DEBUG
void		cucufu(int code);


#endif
