#include <assert.h>


#define  Assert(...)  PASSERT_VA(__VA_ARGS__, )

# ifdef NDEBUG
  #define  PASSERT_VA(...) 

# else  
  #include <stdio.h>

  #ifdef __GNUC__
    #define PW_IGNORE _Pragma ("GCC diagnostic push")                           \
                      _Pragma ("GCC diagnostic ignored \"-Wpragmas\"")          \
                      _Pragma ("GCC diagnostic ignored \"-Wstring-plus-int\"")
    #define PW_RESET  _Pragma ("GCC diagnostic pop")

  #elif defined _MSC_VER
    #define PW_IGNORE _Pragma ("warning ( push )")                              \
                      _Pragma ("warning ( disable : 4906 )")
    #define PW_RESET  _Pragma ("warning ( pop )")

  #else
    #define PW_IGNORE 
    #define PW_RESET 
  #endif


  #define  PASSERT_MSG(VA)  _Generic((VA +0),                                   \
                             char * : fputs("\n"#VA"\n...\n", stderr),          \
                             default: fputs("\n", stderr) )

  #define  PASSERT_VA(P,VA,...)  PW_IGNORE                                      \
                                 (P  ? (void)0  : PASSERT_MSG( VA), assert(P)); \
                                 PW_RESET

# endif //NDEBUG
