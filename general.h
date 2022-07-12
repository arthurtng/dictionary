#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void on_error(const char* s);
void* ncalloc(int n, size_t size);
void* nfopen(char* fname, char* mode);
