/*****************************************************
 *
 * copyright Michael D. Day 2017
 * made available under the GNU GPL, v2
 *
 ****************************************************/
 
#define _GNU_SOURCE
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include </usr/include/sys/ptrace.h>
#include </usr/include/libelf.h>
#include <stdio.h>
#include <getopt.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include "pmparser.h"

int seize_and_wait(pid_t pid);
uintptr_t map_after_heap(char *backing, int hole);
