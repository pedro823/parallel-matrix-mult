#pragma once
#define __UTILS__
#include <stdlib.h>

/*
  Debug macro for stopping the program.
 */
#ifndef sigtrap
#define sigtrap() asm("int $3")
#endif

/*
  Safely allocates memory with handling of out-of-memory errors.
 */
void *emalloc(size_t size);

/*
  Safely allocates zero initialized memory with handling of out-of-memory errors.
 */
void *ecalloc(size_t num, size_t size);

/*
  Safely deallocates memory -- stops if free(NULL).
 */
void efree(void *ptr);
