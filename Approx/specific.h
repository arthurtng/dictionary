#include "../dict.h"

#define KHASHES 11
#define M 20

typedef bool bit;

struct dict {
   bit* bfilter;
   int size;
};

unsigned long* _hashes(const char* s);
int _hash(const char* s);
void test(void);
   
