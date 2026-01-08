# <h1 align="center">Malloc</h1>

<p align="center">
   <b>A dynamic allocation memory management library based on a rewrite of the libc functions malloc, free and realloc.</b><br>
</p>

---

<p align="center">
    <img alt="C" src="https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white" />
    <img alt="Memory Management" src="https://img.shields.io/badge/Memory%20Management-FF6B6B?style=for-the-badge&logo=mathworks&logoColor=white" />
    <img alt="Low Level Programming" src="https://img.shields.io/badge/Low%20Level%20Programming-4ECDC4?style=for-the-badge" />
    <img alt="UNIX" src="https://img.shields.io/badge/UNIX-8892C6?style=for-the-badge" />
</p>

## Table of Contents
1. [Project Overview](#project-overview)
2. [Feature Breakdown](#feature-breakdown)
3. [Execution Instructions](#execution-instructions)

---

## Project overview
This is a fairly simple and small project (although the array of implemented bonus made it not so simple, to be honest). Basically, it's a recreation of the usual malloc-based dynamic memory allocation in C that includes my own versions of [malloc](https://pubs.opengroup.org/onlinepubs/7908799/xsh/malloc.html), [free](https://pubs.opengroup.org/onlinepubs/009604499/functions/free.html) and [realloc](https://pubs.opengroup.org/onlinepubs/009696899/functions/realloc.html), all wrapped around thread safe mechanisms, debug options and comprehensive log tools. The main takeaway from this process is learning how [mmap](https://pubs.opengroup.org/onlinepubs/009604499/functions/mmap.html) works, understand the way in which the program and the SO communicate between each other to request, take and return virtual memory chunks (pages, I should write) and how to lightly optimize malloc calls to avoid excessive overhead. A fundamentally theoretical project, in fewer words.

## Feature Breakdown
Below its simplicity, the process of building this recreation can be broken down in a handful of main features: the allocation core, the thread safety mechanisms, the optimization presets, the defragmentation of freed space and the logging tools.

### Allocation Core
---
All `ft_malloc` calls (which include `ft_realloc` calls in cases in which memory needs to be reallocated and copied, which all go through `ft_malloc`) go through the code contained in `allocator.c`. Here is were the calls to `mmap` take place, be them for small or large chunks of memory. The process is managed via a couple of structs and an enum that shape up the basic data management needed by malloc, stored in `ft_malloc.h`, which are:

```C
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
```

Simply put, zones are the memory segments given by the OS via `mmap` calls, inside which blocks of dynamically allocated (and when needed, freed) memory are managed based on size needs along a programs execution. Therefore, a complete understanding of how `mmap` works is a must before building the allocator core. For this, the best resource is the Linux Manual Page, which states the following prototype and description:

```C
void *mmap(size_t length;
                  void addr[length], size_t length, int prot, int flags,
                  int fd, off_t offset);
```

```
mmap() creates a new mapping in the virtual address space of the
       calling process.  The starting address for the new mapping is
       specified in addr.  The length argument specifies the length of
       the mapping (which must be greater than 0).

       If addr is NULL, then the kernel chooses the (page-aligned)
       address at which to create the mapping; this is the most portable
       method of creating a new mapping.  If addr is not NULL, then the
       kernel takes it as a hint about where to place the mapping; on
       Linux, the kernel will pick a nearby page boundary (but always
       above or equal to the value specified by
       /proc/sys/vm/mmap_min_addr) and attempt to create the mapping
       there.  If another mapping already exists there, the kernel picks
       a new address that may or may not depend on the hint.  The address
       of the new mapping is returned as the result of the call.

       The contents of a file mapping (as opposed to an anonymous
       mapping; see MAP_ANONYMOUS below), are initialized using length
       bytes starting at offset offset in the file (or other object)
       referred to by the file descriptor fd.  offset must be a multiple
       of the page size as returned by sysconf(_SC_PAGE_SIZE).

       After the mmap() call has returned, the file descriptor, fd, can
       be closed immediately without invalidating the mapping.

       The prot argument describes the desired memory protection of the
       mapping (and must not conflict with the open mode of the file).
       It is either PROT_NONE or the bitwise OR of one or more of the
       following flags:

       PROT_EXEC
              Pages may be executed.

       PROT_READ
              Pages may be read.

       PROT_WRITE
              Pages may be written.

       PROT_NONE
              Pages may not be accessed.

The flags argument
       The flags argument determines whether updates to the mapping are
       visible to other processes mapping the same region, and whether
       updates are carried through to the underlying file.  This behavior
       is determined by including exactly one of the following values in
       flags:

       MAP_SHARED
              Share this mapping.  Updates to the mapping are visible to
              other processes mapping the same region, and (in the case
              of file-backed mappings) are carried through to the
              underlying file.  (To precisely control when updates are
              carried through to the underlying file requires the use of
              msync(2).)

       MAP_SHARED_VALIDATE (since Linux 4.15)
              This flag provides the same behavior as MAP_SHARED except
              that MAP_SHARED mappings ignore unknown flags in flags.  By
              contrast, when creating a mapping using
              MAP_SHARED_VALIDATE, the kernel verifies all passed flags
              are known and fails the mapping with the error EOPNOTSUPP
              for unknown flags.  This mapping type is also required to
              be able to use some mapping flags (e.g., MAP_SYNC).

       MAP_PRIVATE
              Create a private copy-on-write mapping.  Updates to the
              mapping are not visible to other processes mapping the same
              file, and are not carried through to the underlying file.
              It is unspecified whether changes made to the file after
              the mmap() call are visible in the mapped region.

[...]

RETURN VALUE         top
       On success, mmap() returns a pointer to the mapped area.  On
       error, the value MAP_FAILED (that is, (void *) -1) is returned,
       and errno is set to indicate the error.

       On success, munmap() returns 0.  On failure, it returns -1, and
       errno is set to indicate the error (probably to EINVAL).

```

Based on this documentation, memory zones are created with calls to `mmap` such as:

```C
t_zone	*zone = mmap(NULL, total_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
```

With this type of calls, memory pages are retrieved from the OS, stored globaly as a linked list of memory zones, each of them containing a linked list of manageable memory blocks. The latter are the ones that are juggled around when allocating, reallocating and/or freeing memory. On the one side, allocating memory is done with the following code:

```C
static size_t get_zone_size(t_zone_type type)
{
	if (type == TINY)
		return PAGE_SIZE * 4;
	else if (type == SMALL)
		return PAGE_SIZE * 32;
	else
		return 0;
}

static t_zone	*create_zone(t_zone_type type, size_t size)
{
	size_t	zone_size = get_zone_size(type);
	t_zone	*zone = mmap(NULL, zone_size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);

	if (zone == MAP_FAILED)
		return NULL;

	zone->type = type;
	zone->size = zone_size;
	zone->next = NULL;
	zone->mem = (void *)zone + sizeof(t_zone);

	t_block	*block = (t_block *)zone->mem;
	block->size = size;
	block->free = 0;
	block->next = NULL;

	zone->blocks = block;

	return zone;
}

void	*allocate_in_zone(t_zone_type type, size_t size)
{
	t_zone	*zone = g_zones;

	// If zone alredy exists, manage through global
	while (zone)
	{
		if (zone->type == type)
		{
			t_block *block = zone->blocks;

			while (block)
			{
				if (block->free && block->size >= size)
				{
					block->free = 0;
					return (void *)(block + 1);
				}
				block = block->next;
			}
		}
		zone = zone->next;
	}

	// Else, create a new zone
	t_zone	*nzone = create_zone(type, size);
	if (nzone == NULL)
		return NULL;

	nzone->next = g_zones;
	g_zones = nzone;
	return (void *)(nzone->blocks + 1);
}
```

Following task directions, zone allocation management is size depending on a first level in order to optimize overhead. `TINY` allocations (0-128) and `SMALL` allocations (128-1024) go through the above function, and `LARGE` allocations go through a specific case that just differs in that the size of the allocation is carefully maneged to be exactly as large as needed. On the other side, freeing of memory is done with the following code:

```C
void	free(void *ptr)
{
	if (ptr == NULL)
		return;

	pthread_mutex_lock(&g_malloc_mutex);

	t_block	*block = (t_block *)ptr - 1;
	block->free = 1;

	t_zone	*zone = g_zones;

	while (zone)
	{
		t_block *current = zone->blocks;
		while (current)
		{
			if (current == block)
			{
				join_free_blocks(zone);
				break;
			}
			current = current->next;
		}
		zone = zone->next;
	}

	pthread_mutex_unlock(&g_malloc_mutex);
	
	mark_as_freed(ptr);
}
```

This is just an iteration through the targeted zones and their blocks, allong wich the blocks both marked as free and fused together to achieve defragmentation. Both of these steps are in place to achieve some extended features suggested by the task, and are based on these functions:

```C
static void join_free_blocks(t_zone *zone)
{
	t_block *current = zone->blocks;

	while (current && current->next)
	{
		if (current->free && current->next->free)
		{
			current->size += sizeof(t_block) + current->next->size;
			current->next = current->next->next;
		}
		else
		{
			current = current->next;
		}
	}
}
```
```C
void	mark_as_freed(void *ptr)
{
	if (!ptr || !is_bonus_mode())
		return;
	
	t_history *current = g_history;
	t_history *last_match = NULL;
	
	while (current != NULL)
	{
		if (current->start == ptr && !current->freed)
			last_match = current;
		current = current->next;
	}
	
	if (last_match != NULL)
		last_match->freed = 1;
}
```

In the case of a reallocation call, if the targeted memory zone is not large enough to store the newly requested size, the flow goes through a free->malloc->memcpy pipeline. The old pointer (previousle allocated) is freed, a new allocation is done with the new size as argument, the data pointed by the old pointer is moved into the newly re-allocated pointer.

## Thread Safety Mechanisms
Thread safety is achieved via a unique global mutex that is locked before any allocation and free related activity. The mutex is stored in the header as a global variable:
```C
extern pthread_mutex_t	g_malloc_mutex;
```
It is initialized in `threader.c` via the `PTHREAD_MUTEX_INITIALIZER` macro, and managed through simple lock-unlock calls.


### Optimization Presets
---
As mentioned before, allocation sizes are divided in a 3 different tiers:
- `TINY` -> 0-128
- `SMALL` -> 128-1024
- `LARGE` -> >1024

This sizes are also managed with a 16-byte alignment for a handful of reasons:
1. CPU Performance - Cache Line Alignment
Modern CPUs fetch data in chunks (cache lines), typically 64 bytes. Aligning to 16 bytes helps ensure efficient cache utilization and reduces the number of cache line crossings when accessing data.

2. SIMD Instructions
Many modern CPUs have SIMD (Single Instruction, Multiple Data) instructions like:

       SSE (Streaming SIMD Extensions) - requires 16-byte alignment
       AVX (Advanced Vector Extensions) - requires 32-byte alignment
       AVX-512 - requires 64-byte alignment
       If data isn't properly aligned, these instructions can cause segmentation faults or significant performance penalties.

3. Struct Padding & ABI Requirements
The x86-64 System V ABI (Application Binary Interface) requires:

       malloc() to return memory aligned to at least 16 bytes
       This ensures structs with double, long double, or 128-bit types are properly aligned
4. Performance on Atomic Operations
Many atomic operations and lock-free algorithms perform better (or only work correctly) with aligned memory.

5. malloc() must return 16-byte aligned addresses for SSE compatibility reasons, as SSE instructions (very common) require 16-byte alignment. On top of that, natural size of pointers on 64-bit system naturally alings to 16 bytes (e.g., two pointers = 16 bytes).

Regarding the specific process of allocation, `TINY` and `SMALL` allocations are handled with fixed sizes, which are based on the memory page size returned by `mmap` (i.e., it is system dependent). This reduces system calls for non-large allocations (which I think are more usual than large ones, but I might be wrong):

```C
static size_t get_zone_size(t_zone_type type)
{
	if (type == TINY)
		return PAGE_SIZE * 4;
	else if (type == SMALL)
		return PAGE_SIZE * 32;
	else
		return 0;
}
```
As mentioned above, `LARGE` allocations request the specific ammount of page-size-based memory needed by the request. 


### Defragmentation of Freed Space
---
This was delegated as a bonus feature in the task, although it can be argued that its importance is paramount. The specific code for this feature (also stated above, in the previous index) is, once again, quite simple: loop through the target zone and its blocks, find the freed (marked as freed) blocks and fuse them together in a single block with a conjoined size of all the respective sizes of the merged blocks:

```C
static void join_free_blocks(t_zone *zone)
{
	t_block *current = zone->blocks;

	while (current && current->next)
	{
		if (current->free && current->next->free)
		{
			current->size += sizeof(t_block) + current->next->size;
			current->next = current->next->next;
		}
		else
		{
			current = current->next;
		}
	}
}
```

### Logging and Debugging Tools
---
Debugging is built in following the system's malloc, i.e. via an environment variable. By exporting `MALLOC_DEBUG` with a value of 1 the user can set up the debug logs for every allocation call, which consist of prints in a size-address format like the following:

```
[DEBUG] Allocated 112 bytes at 0x7fcab18b8040
[DEBUG] Allocated 208 bytes at 0x7fcab163e040
```

Extended logging tools are also provided with a similar approach, although tied around a different environment variable. By exporting `MALLOC_BONUS` with a value of 1, the user can set up several formats of in-depth information logging regarding all the allocation calls done through execution. These are comprised of:

- One line tagged logs: state, starting and ending address of the whole allocation, size.
- Memory Zone logs: pointer to the starting of the zone and its total size.
- Hexadecimal dump of the whole segment of memory covered by an allocation.
- History of all allocations done during program execution.

An example of a full, comprehensive log (debug + bonus):

```
***** TEST 1: BASIC TEST *****
[DEBUG] Allocated 64 bytes at 0x7f929c336040

Hex Dump from 0x7f929c336040 to 0x7f929c336080 (64 bytes)
Address          : Hex Values                                       | ASCII
--------------------------------------------------------------------------------------
00007f929c336040 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c336050 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c336060 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c336070 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
--------------------------------------------------------------------------------------

=============== ALLOCATION HISTORY ===============
[1] 0x7f929c336040 -> 0x7f929c336080 : 64 bytes
==================================================

[DEBUG] Allocated 128 bytes at 0x7f929c332040

Hex Dump from 0x7f929c332040 to 0x7f929c3320c0 (128 bytes)
Address          : Hex Values                                       | ASCII
--------------------------------------------------------------------------------------
00007f929c332040 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c332050 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c332060 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c332070 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c332080 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c332090 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c3320a0 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
00007f929c3320b0 : 00 00 00 00 00 00 00 00  00 00 00 00 00 00 00 00 | ................
--------------------------------------------------------------------------------------

=============== ALLOCATION HISTORY ===============
[1] 0x7f929c336040 -> 0x7f929c336080 : 64 bytes
[2] 0x7f929c332040 -> 0x7f929c3320c0 : 128 bytes
==================================================
```

### Memory Leak Check
---
An additional feature to check memory leaks is also implemented. It works based on the marking of freed blocks that is part of the `free()` process, and is built as a check of the managed memory zones and blocks to retrieve their freed status. If any checked block is not marked as free, it is logged as a `LEAK` in the comprehensive output of the program. Otherwise, a confirmation of no leaks is printed:

```
========== MEMORY LEAK REPORT ==========
1 block not freed (total: 208 bytes)
[LEAK] 0x7f1c2d43d040 - 0x7f1c2d43d110 : 208 bytes
========================================
```

This was specially useful to me as a self-check for my `realloc` implementation.

## Execution Instructions
The basic use of this project is straight forward:
- Clone the repo
- Run `make` command in your console application
- You've succesfully created a shared `.so` library file with a symlink pointing to the system's `malloc`, `realloc` and `free` functions that you can throw in to whatever program compilation you like to use to test the included recreations.

Furthermore, a test file is also provided in the repo (`main.c`). This test can be targeted in several different ways:
- `make test` will build and run a testing executable (`malloc`) in its most basic implementation (no debug nor bonus).
- `make test_debug` will build and run a testing executable with the debug layer activated.
- `make test_bonus` will build and run a testing executable with the bonus layer activated.
- `make test_full` will build and run a testing executable with both the debug and bonus layers activated.

---
Thanks for reading, any feedback is welcomed.
