#!/bin/sh
export LD_LIBRARY_PATH=.
export LD_PRELOAD=libmalloc_x86_64_Linux.so
$@
