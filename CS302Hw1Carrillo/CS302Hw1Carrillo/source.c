/*
   When playing with file filters, it is useful to know that "end of file" is
   denoted at the command-line in Windows by Ctrl-Z and by Ctrl-D in Linux.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
   int i, j, k;
   int r;
   int lines, spaces, data_points;
   double d;

   srand( (unsigned)time(NULL) );

   lines = rand()%100;  // random number of lines of output
   for (i = 0; i < lines; i++)
   {
      if ( rand()%2 )  // "flip" a coin to decide if there should be leading spaces
      {
         spaces = rand()%20;   // random leading spaces
         for (j = 0; j < spaces; j++)
            printf(" ");
      }
      data_points = rand()%20;  // random number of numbers on this line of output
      for (j = 0; j < data_points - 1; j++)
      {
         r = rand();
         d = (r%10000) + ( (double)r / RAND_MAX );
         printf("%.13f", d);
         printf(" ");          // at least once space between numbers
         spaces = rand()%20;   // with some extra random spaces
         for (k = 0; k < spaces; k++)
            printf(" ");
      }
      r = rand();
      d = (r%10000) + (double)r / RAND_MAX;  // last number on this line
      printf("%.13f", d);
      if ( rand()%2 )  // "flip" a coin to decide if there should be trailling spaces
      {
         spaces = rand()%20;   // random trailing spaces
         for (k = 0; k < spaces; k++)
            printf(" ");
      }
      printf("\n");       // end this line
      //printf("#\n\n");  // end this line (in a way so that we can see where it ends)
   }
   return 0;
}
