#ifndef FT_MALLOC_H
# define FT_MALLOC_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <pthread.h>
# include "colors.h"
# include "logger.h"
# include "../libft/incs/libft.h"
# include "../libft/incs/ft_printf.h"

// MACROs
# define TINY_LIMIT 128
# define SMALL_LIMIT 1024
# define PAGE_SIZE getpagesize()
# define ALIGN16(x) (((x) + 15 & ~15))

// DATA STRUCTURES
typedef enum e_zone_type {
	TINY,
	SMALL,
	LARGE
} t_zone_type;

typedef struct s_block {
	size_t			size;
	int				free;
	struct s_block	*next;
} t_block;

typedef struct s_zone {
	t_zone_type		type;
	size_t			size;
	struct s_zone	*next;
	t_block			*blocks;
	void			*mem;
} t_zone;

// GLOBALS
extern t_zone			*g_zones;
extern pthread_mutex_t	g_malloc_mutex;

// APIs
FT_API void	*ft_malloc(size_t size);
FT_API void	ft_free(void *ptr);
FT_API void	*ft_realloc(void *ptr, size_t size);

// ALLOCATORS
FT_API void	*allocate_in_zone(t_zone_type type, size_t size);
FT_API void	*allocate_large(size_t size);

// VISUALIZERS
void	ft_show_alloc_mem(void);
void	ft_show_alloc_mem_ex(void);

// DEBUGGER
int is_debug_mode(void);

#endif