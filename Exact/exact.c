#include "specific.h"

void dict_free(dict* x)
{
   if(x){
      for (int i=0; i<x->size; i++){
         data* ptr = x->hashtable[i];            
         while(ptr != NULL){
            data* temp = ptr->next;
            free(ptr->word);
            free(ptr);
            ptr = temp;               
         }
         free(ptr);            
      }
      free(x->hashtable);
      free(x);
   }   
}

dict* dict_init(unsigned int maxwords)
{
   test();
   dict* d = (dict*)ncalloc(1, sizeof(dict));
   d->hashtable = (data**)ncalloc(maxwords*SCALEFACTOR, sizeof(data*));
   d->size = maxwords*SCALEFACTOR;
   return d;
}

bool dict_spelling(dict* x, const char* s)
{
   if (x == NULL || s == NULL){
      return false;
   }
   int hashedval = hash(x->size, s);
   data* ptr = x->hashtable[hashedval];
   while(ptr != NULL){
      if(strcmp(ptr->word, s)==0){
         return true;
      }
      ptr = ptr->next;
   } 
   return false;   
}

bool dict_add(dict* x,  const char* s)
{
   if (x == NULL || s == NULL){
      return false;
   }
   int hashedval = hash(x->size, s);
   if (x->hashtable[hashedval] != NULL){
      if (dict_spelling(x, s)){
         return true;
      }       
      data* p = allocateData(s);
      p->next = x->hashtable[hashedval];
      x->hashtable[hashedval] = p;
      return true;
   }
   x->hashtable[hashedval] = allocateData(s);
   return true; 
}

// Bernstein hash function
int hash(unsigned int sz, const char* s)
{
   unsigned long hash = 5381;
   int c;
   while((c=(*s++))){
      hash = 33 * hash ^ c;
   }
   return (int)(hash%sz);
}

data* allocateData(const char* s)
{
   data* p = (data*)ncalloc(1, sizeof(data));
   p->word = (char*)ncalloc(strlen(s)+1, sizeof(char));
   strcpy(p->word, s);
   p->next = NULL;
   return p;
}

void test(void)
{
   // Smaller size hash
   // Hashing alpha-only strings
   int size = 100;
   assert(hash(size, "seventeen")<size);
   assert(hash(size, "eight")<size);
   assert(hash(size, "seventeen") == hash(size, "seventeen"));
   assert(hash(size, "eight") == hash(size, "eight"));
   // Hashing non-alpha chars
   assert(hash(size, "123456789") == hash(size, "123456789"));
   assert(hash(size, "!£$%^") == hash(size, "!£$%^"));
   assert(hash(size, "  ") == hash(size, "  "));    
   
   // Larger size hash
   int size1 = 10000;
   assert(hash(size1, "seventeen")<size1);
   assert(hash(size1, "eight")<size1);
   assert(hash(size1, "seventeen") == hash(size1, "seventeen"));
   assert(hash(size1, "eight") == hash(size1, "eight"));
   assert(hash(size1, "123456789") == hash(size1, "123456789"));
   assert(hash(size1, "!£$%^") == hash(size1, "!£$%^"));
   
   // allocateData tests
   data* test = allocateData("seventeen");
   assert(strcmp(test->word, "seventeen")==0);     
   free(test->word);
   free(test);
   
   data* test1 = allocateData("!£$%^");
   assert(strcmp(test1->word, "!£$%^")==0);
   free(test1->word);
   free(test1);  

   // dict_init() code copied here for testing
   dict* testdict = (dict*)ncalloc(1, sizeof(dict));
   testdict->hashtable = (data**)ncalloc(size*SCALEFACTOR, sizeof(data*));
   testdict->size = size*SCALEFACTOR;

   // dict_add() and dict_spelling() tests
   // Invalid strings
   assert(!dict_add(testdict, NULL)); 
   assert(!dict_spelling(testdict, NULL));

   // Alpha strings
   assert(dict_add(testdict, "seventeen"));
   assert(strcmp(testdict->hashtable[hash(testdict->size, "seventeen")]->word, "seventeen")==0);
   assert(dict_spelling(testdict, "seventeen"));
   assert(!dict_spelling(testdict, "eighteen"));

   // Non-alpha strings
   assert(dict_add(testdict, "!£$%^"));
   assert(strcmp(testdict->hashtable[hash(testdict->size, "!£$%^")]->word, "!£$%^")==0);
   assert(dict_spelling(testdict, "!£$%^"));
   dict_free(testdict);

}
   

