#ifndef BRV_H
 #define BRV_H
 #ifndef __GNUC__
  #warning __GNUC__ not defined: "brv.h" unlikely to work
 #endif

 #define brv_errmsg() brv_err_i(__FILE__,:__LINE__ ERROR: `brv` operand must, \
  , have integer type.)
 #define brv_err_i(file,re,_,expand...) brv_err_ii(file,re,expand)
 #define brv_err_ii(file,head,tail...) file#head#tail

 #define brv_typecheck(compat) _Static_assert(_Generic(((compat) + 0ULL),     \
                                   unsigned long long: 1, default: 0 ),       \
                                   brv_errmsg() )
 
 #define brv(INPUT) ({ __auto_type _brv_src = (INPUT);       /*expand once*/  \
		       brv_typecheck(_brv_src);                               \
		       typeof(_brv_src) _brv_out = 0;                         \
		       int count = (sizeof(_brv_src)*8);                      \
                       asm("1: shr  $0x1,%[SRC]    \n\t"     /* load cf  */   \
                           "   adc  %[OUT],%[OUT]  \n\t"     /* x=2x+bit */   \
                           "   sub  $0x1,%[COUNT]  \n\t"		      \
			   "   jne  1b		   \n\t"                      \
                            :  [SRC]   "+r" (_brv_src),                       \
			       [COUNT] "+r" (count),                          \
			       [OUT]   "+r" (_brv_out)                        \
            	        ); _brv_out; })

#endif /* BRV_H */

