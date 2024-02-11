           #include <mcheck.h>
           #include <stdio.h>
           #include <stdlib.h>

           int
           main(void)
           {
               mtrace();

               for (unsigned int j = 0; j < 2; j++)
                   malloc(100);            /* Never freed--a memory leak */

               calloc(16, 16);             /* Never freed--a memory leak */
               exit(EXIT_SUCCESS);
           }
