#include "specific.h"

void dict_free(dict* x)
{
   if(x){
      free(x->bfilter);
      free(x);
   }
}

dict* dict_init(unsigned int maxwords)
{
   test();
   dict* d = (dict*)ncalloc(1, sizeof(dict));
   d->bfilter = (bit*)ncalloc(M*maxwords, sizeof(bit));
   d->size = M*maxwords;  
   return d; 
}

bool dict_add(dict* x,  const char* s)
{
   if (x == NULL || s == NULL){
      return false;
   }
   unsigned long* hashfuncs = _hashes(s);
   for (int i=0; i<KHASHES; i++){
      hashfuncs[i] = hashfuncs[i] % x->size;
      x->bfilter[hashfuncs[i]] = true;
   }
   free(hashfuncs);
   return true;
}

bool dict_spelling(dict* x, const char* s)
{
   if (x == NULL || s == NULL){
      return false;
   }
      
   unsigned long* hashfuncs = _hashes(s);
   for (int i=0; i<KHASHES; i++){
      hashfuncs[i] = hashfuncs[i] % x->size;
      if(x->bfilter[hashfuncs[i]] == false){
         free(hashfuncs);
         return false;
      }
   }
   free(hashfuncs);
   return true;
}

unsigned long* _hashes(const char* s)
{
   unsigned long* hashes = ncalloc(KHASHES, sizeof(unsigned long));
   unsigned long bh = _hash(s); 
   int ln = strlen(s);
   srand(bh*(ln*s[0]+s[ln-1]));
   unsigned long h2 = bh;
   for (int i=0; i<KHASHES; i++){
      h2 = 33*h2 ^ rand();
      hashes[i] = h2;
   }   
   return hashes;
}

// Bernstein hash function
int _hash(const char* s)
{
   unsigned long hash = 5381;
   int c;
   while((c=(*s++))){
      hash = 33 * hash ^ c;
   }
   return (int)hash;
}

void test(void)
{

   // Hashing alpha-only strings
   assert(_hash("eight") == _hash("eight"));
   assert(_hash("seventeen") == _hash("seventeen"));
   // Hashing non-alpha chars
   assert(_hash("123456789") == _hash("123456789"));
   assert(_hash("!£$%^") == _hash("!£$%^")); 
   assert(_hash("  ") == _hash("  "));  

   // _hashes() function testing
   unsigned long* hashes = _hashes("eight");
   unsigned long* hashes2 = _hashes("eight");
   for (int i = 0; i<KHASHES; i++){
      assert(hashes[i] == hashes2[i]);
   }
   free(hashes);
   free(hashes2);
   hashes = _hashes("!£$%^");
   hashes2 = _hashes("!£$%^");
   for (int i = 0; i<KHASHES; i++){
      assert(hashes[i] == hashes2[i]);
   }
   free(hashes);
   free(hashes2);  

   // dict_init() code copied here for testing
   int wordcount = 100;
   dict* testdict = (dict*)ncalloc(1, sizeof(dict));
   testdict->bfilter = (bit*)ncalloc(M*wordcount, sizeof(bit));
   testdict->size = M*wordcount;     

   // dict_add() and dict_spelling() tests
   // Invalid strings   
   assert(!dict_add(testdict, NULL));    
   assert(!dict_spelling(testdict, NULL));
    
   // Alpha strings
   assert(dict_add(testdict, "seventeen"));
   unsigned long* hashes3 = _hashes("seventeen");
   for (int i = 0; i<KHASHES; i++){
      assert(testdict->bfilter[hashes3[i]%testdict->size] == 1);
   }
   assert(dict_spelling(testdict, "seventeen"));
   assert(!dict_spelling(testdict, "eighteen"));
   
   // Non-alpha strings
   assert(dict_add(testdict, "!£$%^"));
   hashes = _hashes("!£$%^");
   for (int i = 0; i<KHASHES; i++){
      assert(testdict->bfilter[hashes[i]%testdict->size] == 1);
   }
   assert(dict_spelling(testdict, "!£$%^"));

   free(hashes);
   free(hashes3);
   dict_free(testdict);
}


