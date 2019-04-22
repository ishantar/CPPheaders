#include <stdlib.h>
#include <stdio.h>
#include "../src/passert.h"


#if defined NPRED
  #define RARG0 +1
#else 
  #define RARG0
#endif

#if defined   VALID
   # define RARGS RARG0, "message string yay!"
#elif defined EMPTY
   # define RARGS RARG0, 
#elif defined NOMSG
   # define RARGS RARG0
#elif defined XNARG
   # define RARGS RARG0, "first message string!", "extra arg string :("
#elif defined XTARG
   # define RARGS RARG0, 1
#else
   # define RARGS RARG0
#endif

int main(void) 
 {
    void *p = NULL; //malloc(0);
     Assert(p RARGS);
    printf("%p",p);
    return 0;
 }
