# -=-=-=-=-    HOSTTYPE -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

ifeq ($(HOSTTYPE),)
HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

# -=-=-=-=-    NAME -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= #

NAME			:= libmalloc_$(HOSTTYPE).so
TEST_NAME		:= malloc
SO_LINK			:= libmalloc.so

# -=-=-=-=-    FILES -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

SRC				=	allocator.c				\
					debug.c					\
					free.c					\
					malloc.c				\
					realloc.c				\
					show_alloc_mem_ex.c		\
					show_alloc_mem.c		\
					helpers.c				\
					threader.c				\
					bonus.c

TEST_FILE		=	main.c

SRCDIR			=	srcs
SRCS			=	$(addprefix $(SRCDIR)/, $(SRC))

OBJDIR			=	.obj
OBJS			=	$(addprefix $(OBJDIR)/, $(SRC:.c=.o))

DEPDIR			=	.dep
DEPS			=	$(addprefix $(DEPDIR)/, $(SRC:.c=.d))

INC				=	./incs/
HEADERS			=	./incs/malloc.h
INCLUDES		=	-I./incs/

MAKE			=	Makefile
			
# -=-=-=-=-    FLAGS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

CC				=	cc
CFLAGS			=	-Wall -Wextra -Werror -fPIC -fvisibility=hidden -I incs #-fsanitize=thread #-fsanitize=address
DEPFLAGS		=	-MMD -MP

# -=-=-=-=-    TARGETS -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- #

all: directories $(NAME) symlink

test: all directories $(TEST_NAME)
	LD_LIBRARY_PATH=. ./malloc

symlink:
	@ln -sf $(NAME) $(SO_LINK)

directories:
	@mkdir -p $(OBJDIR)
	@mkdir -p $(DEPDIR)

-include $(DEPS)

$(NAME): $(OBJS)
	$(CC) -shared -o $@ $^ $(LDFLAGS)

$(TEST_NAME): $(OBJS) $(TEST_FILE)
	$(CC) -Wall -Wextra -Werror -I incs $(OBJS) $(TEST_FILE) -L. -lmalloc -o $(TEST_NAME)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c Makefile
	mkdir -p $(@D)
	mkdir -p $(DEPDIR)/$(*D)
	$(CC) $(CFLAGS) $(DEPFLAGS) $(INCLUDES) -c $< -o $@ -MF $(DEPDIR)/$*.d

clean:
	@/bin/rm -fr $(OBJDIR) $(DEPDIR)

fclean: clean
	@/bin/rm -f $(NAME) $(TEST_NAME)
	@/bin/rm -f libmalloc.so
	@/bin/rm -f .history.txt

re: fclean all

.PHONY:  all clean fclean re libs directories test