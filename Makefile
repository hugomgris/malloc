# -=-=-=-=-    HOSTTYPE -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

NAME			:= libft_malloc_$(HOSTTYPE).so
TEST_NAME		:= ft_malloc
SO_LINK			:= libft_malloc.so

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRC				=	main.c					\
					allocator.c				\
					debug.c					\
					ft_free.c				\
					ft_malloc.c				\
					ft_realloc.c			\
					ft_show_alloc_mem_ex.c	\
					ft_show_alloc_mem.c		\
					threader.c

SRCDIR			=	srcs
SRCS			=	$(addprefix $(SRCDIR)/, $(SRC))

OBJDIR			=	.obj
OBJS			=	$(addprefix $(OBJDIR)/, $(SRC:.c=.o))

DEPDIR			=	.dep
DEPS			=	$(addprefix $(DEPDIR)/, $(SRC:.c=.d))

INC				=	./incs/
HEADERS			=	./incs/ft_malloc.h
INCLUDES		=	-I./incs/
LIBFT			=	./libft/libft.a

MAKE			=	Makefile
			
# -=-=-=-=-    FLAGS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -g -fsanitize=thread #-fsanitize=address
DEPFLAGS		=	-MMD -MP

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: directories libs $(NAME) symlink

test: all directories libs $(TEST_NAME)

symlink:
	@ln -sf $(NAME) $(SO_LINK)

directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)

libs:
	@make -C ./libft/

-include $(DEPS)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)

$(TEST_NAME):
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(TEST_NAME)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c Makefile
	mkdir -p $(@D)
	mkdir -p $(DEPDIR)/$(*D)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -I./libft/incs/ -c $< -o $@ -MF $(DEPDIR)/$*.d

clean:
	@/bin/rm -fr $(OBJDIR) $(DEPDIR)
	@make -C ./libft clean

fclean: clean
	@/bin/rm -f $(NAME) $(TEST_NAME)
	@/bin/rm -f libft_malloc.so
	@make -C ./libft fclean

re: fclean all

.PHONY:  all clean fclean re libs directories