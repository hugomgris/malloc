# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

NAME		= 	libft.a

# -=-=-=-=-    PATH -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRC		=	ft_atoi.c ft_bzero.c ft_calloc.c ft_isalnum.c ft_isalpha.c 	\
			ft_isascii.c ft_isdigit.c ft_isprint.c ft_itoa.c ft_memchr.c 	\
			ft_memcmp.c ft_memcpy.c ft_memmove.c ft_memset.c ft_putchar_fd.c\
			ft_putendl_fd.c ft_putnbr_fd.c ft_putstr_fd.c ft_split.c 		\
			ft_strcpy.c ft_strchr.c ft_strdup.c ft_striteri.c ft_strjoin.c ft_strlcat.c \
			ft_strlcpy.c ft_strlen.c ft_strmapi.c ft_strcmp.c ft_strncmp.c	\
			ft_strnstr.c ft_strrchr.c ft_strtrim.c ft_substr.c ft_tolower.c \
			ft_toupper.c ft_printf.c ft_printf_conv.c ft_printf_utils.c 	\
			ft_lstnew.c ft_lstadd_front.c ft_lstsize.c ft_lstlast.c 		\
			ft_lstadd_back.c ft_lstdelone.c ft_lstclear.c ft_lstiter.c 		\
			ft_lstmap.c	ft_list_sort.c ft_list_reverse.c ft_list_cmp.c		\
			get_next_line.c ft_atoi_base.c cucufu.c ft_array_free.c			\
			ft_array_print.c ft_strjoin_free.c ft_isxdigit.c ft_strndup.c	\
			ft_array_size.c

SRCDIR		=	srcs
SRCS		=	$(addprefix $(SRCDIR)/, $(SRC))

OBJS		=	$(addprefix $(OBJDIR)/, $(SRC:.c=.o))
DEPS		=	$(addprefix $(DEPDIR)/, $(SRC:.c=.d))

OBJDIR 		=	.obj
DEPDIR		=	.dep

INC			=	./incs/
HEADERS		=	./incs/libft.h ./incs/ft_printf.h
INCLUDES	=	-I./incs/

MAKE		=	Makefile

# -=-=-=-=-    FLAGS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

CC			=	cc
CFLAGS		=	-Wall -Wextra -Werror -g
DEPFLAGS	=	-MMD -MP

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: directories $(NAME)

directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)

-include $(DEPS)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c Makefile
	@mkdir -p $(@D)
	@mkdir -p $(DEPDIR)/$(*D)
	@$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@ -MF $(DEPDIR)/$*.d
	
clean:
	@/bin/rm -fr $(OBJDIR) $(DEPDIR)

fclean: clean
	@/bin/rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re directories
