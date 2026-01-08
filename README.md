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

---

## Project overview
This is a fairly simple and small project. Basically, it's a recreation of the usual malloc-based dynamic memory allocation in C that includes my own versions of [malloc](https://pubs.opengroup.org/onlinepubs/7908799/xsh/malloc.html), [free](https://pubs.opengroup.org/onlinepubs/009604499/functions/free.html) and [realloc](https://pubs.opengroup.org/onlinepubs/009696899/functions/realloc.html), all wrapped around thread safe mechanisms, debug options and comprehensive log tools. The main takeaway from this process is learning how [mmap](https://pubs.opengroup.org/onlinepubs/009604499/functions/mmap.html) works, understand the way in which the program and the SO communicate between each other to request, take and return virtual memory chunks (pages, I should write) and how to lightly optimize malloc calls to avoid excessive overhead. A fundamentally theoretical project, in fewer words.

## Feature Breakdown
Below its simplicity, the process of building this recreation can be broken down in a handful of main features: the allocation core, the thread safety mechanisms, the optimization presets, the defragmentation of freed space and the logging tools.

## Allocation Core
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

These calls' arguments can be broken down in:
