#include <assert.h>


# ifdef NDEBUG
  #define  PASSERT_VA(...) 

# else  
  #include <stdio.h>

  #define  PASSERT_VA(P,VA,...)  (P  ? 0                                        \
                                     : fputs("\n"#VA"\n\n",stderr), assert(P))
/*NDEBUG*/
# endif

#define  Assert(...)  PASSERT_VA(__VA_ARGS__, )





/*

  #ifdef __GNUC__
    #define PW_IGNORE _Pragma ("GCC diagnostic push")                             \
                      _Pragma ("GCC diagnostic ignored \"-Wpragmas\"")            \
                      _Pragma ("GCC diagnostic ignored \"-Wstring-plus-int\"")    \
                      _Pragma ("GCC diagnostic ignored \"-Wpointer-to-int-cast\"")
    #define PW_RESET  _Pragma ("GCC diagnostic pop")
  #else
    #define PW_IGNORE 
    #define PW_RESET 
  #endif

    #define  PASSERT_MSG(VA)  _Generic(((unsigned char)VA +0),                    \
                                 int : fputs("\n"#VA"\n\n",stderr),               \
                       unsigned char : fputs("\n"#VA"\n\n", stderr) )
*/
