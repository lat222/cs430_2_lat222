#include <string.h>
#include <stdio.h>

int main () {
   char str[80] = "This is - www.tutorialspoint.com - website";
   const char s[2] = "-";
   char *token;
   
   /* get the first token */
   token = strtok(str, s);
   
   /* walk through other tokens */
   while( token != NULL ) {
      printf( " %s\n", token );
      if(strcmp(token, " www.tutorialspoint.com ") == 0)
      {
         char* tk = strtok(token,".");
         while(tk != NULL)
         {
            printf("%s\n",tk);
            tk = strtok(NULL,".");
         }
      }
    
      token = strtok(NULL, s);
   }
   printf("--%c--",str[-2]);
   return(0);
}