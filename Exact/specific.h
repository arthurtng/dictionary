#include "../dict.h"

#define SCALEFACTOR 2

struct data {
   char* word;
   struct data* next;
};
typedef struct data data;

struct dict {
   data** hashtable;
   int size;
};


int hash(unsigned int sz, const char* s);
data* allocateData(const char* s);
void test(void);


