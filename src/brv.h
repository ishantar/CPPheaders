#ifndef BRV_H
 /* dependancy check */
 #if !defined(__x86_64__) && !defined(__i386__)
  #error `brv.h` requires x86 instruction set (__i386__ or __x86_64__)
 #elif !defined(__GNUC__)
  #error `brv.h` requires gcc extensions (__GNUC__)
 #endif
 /* supported */
 #define BRV_H

 /* only accept int types */
 #define brv_typecheck(INPUT) _Static_assert(_Generic(((INPUT) + 0ULL),       \
                                   unsigned long long: 1, default: 0 ),       \
                                   brv_errmsg(INPUT) )
 /* only emit useful errors */
 #define brv_errmsg(INPUT) brv_err_i(__FILE__,:__LINE__ ERROR: `brv` operand, \
   , must have integer type (passed `INPUT`))
 #define brv_err_i(file,re,_,expand...) brv_err_ii(file,re,expand)
 #define brv_err_ii(file,head,tail...) file#head#tail

 /* implimentation */
 #define brv(INPUT) ({ brv_typecheck(INPUT);                                  \
                       __auto_type _brv_out = (INPUT);                        \
                       __auto_type _brv_tmp = (INPUT);                        \
                       asm("     mov  %[BITS],%%ecx  \n\t"   /* num bits */   \
                           "top: sal  $0x1,%[TMP]    \n\t"   /* load carry */ \
                           "     rcr  $0x1,%[OUT]    \n\t"   /* push carry */ \
                           "     loop top            \n\t"                    \
                            : [OUT]  "+r" (_brv_out),                         \
                              [TMP]  "+r" (_brv_tmp)                          \
                            : [BITS]  "i" (sizeof(INPUT)*8)                   \
                            : "ecx" );                                        \
                       _brv_out; })

#endif /* BRV_H */
