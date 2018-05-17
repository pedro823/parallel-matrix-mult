#include "utils.h"
#include <stdlib.h>
#include <stdio.h>

void *emalloc(size_t size) {
  void *p = malloc(size);
  if (p == NULL) {
    fprintf(stderr, "ERROR: Could not allocate pointer of size %lld\n", size);
    exit(-1);
  }
  return p;
}

void efree(void* ptr) {
  if (ptr == NULL) {
    fprintf(stderr, "ERROR: Could not free pointer: ptr is NULL\n");
    sigtrap();
  }
  free(ptr);
}
