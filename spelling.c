/*
Driver code for a simple spell checker 
*/

#include "dict.h"

#define BIGSTR 1000

int main(int argc, char* argv[])
{
   if(argc != 3){
      fprintf(stderr, "Usage : %s <dictionary.txt> <test.txt>\n", argv[0]);
      exit(EXIT_FAILURE);
   }
   char str[BIGSTR];
   dict* x = dict_init(400000);
   FILE* fp = nfopen(argv[1], "rt");
   bool done = false;
   do{
      if(fscanf(fp, "%s", str) != 1){
         done = true;
      }
      else{
         assert(dict_add(x, str));
      }
   }while(!done);
   fclose(fp);

   fp = nfopen(argv[2], "rt");
   done = false;
   do{
      if(fscanf(fp, "%s", str) != 1){
         done = true;
      }
      if(dict_spelling(x, str)==false){
         printf("Misspelt : %s\n", str);
         // This is a new word, don't want to see it again
         assert(dict_add(x, str));
      }
   }while(!done);
   fclose(fp);

   dict_free(x);
   return EXIT_SUCCESS;
}
