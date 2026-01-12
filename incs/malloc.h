#ifndef MALLOC_H
# define MALLOC_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdint.h>
# include <sys/mman.h>
# include <sys/resource.h>
# include <pthread.h>
# include <fcntl.h>
# include "colors.h"
# include "logger.h"

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

typedef struct s_history {
	void				*start;
	void				*end;
	size_t				size;
	int					freed;
	struct s_history	*next;
} t_history;

// GLOBALS
extern t_zone			*g_zones;
extern pthread_mutex_t	g_malloc_mutex;
extern t_history		*g_history;

// APIs
FT_API void	*malloc(size_t size);
FT_API void	free(void *ptr);
FT_API void	*realloc(void *ptr, size_t size);

// ALLOCATORS
FT_API void	*allocate_in_zone(t_zone_type type, size_t size);
FT_API void	*allocate_large(size_t size);

// VISUALIZERS
void		show_alloc_mem(void);
void		show_alloc_mem_ex(void);

// HELPERS
void		*ft_memcpy(void *dst, const void *src, size_t len);
char		*ft_strcpy(char *dst, const char *src);
void		set_malloc_tracking(int enable);

// DEBUGGER
int			is_debug_mode(void);

// BONUS
int			is_bonus_mode(void);
void		show_hex_dump(void *start, void *end);
void		append_to_history(void *start, void *end, size_t size);
void		show_malloc_history(void);
void		mark_as_freed(void *ptr);
void		check_memory_leaks(void);

#endif