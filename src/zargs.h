#ifndef ZARGS_H 
#define ZARGS_H 
#include "argp.h"

#define _zx(args...) args
#define _zex(args...) args
#define _zblock

#define zargs(f,args...) _zargn(f,args,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,, \
                            ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,)

#define _zargn(f,_00,_01,_02,_03,_04,_05,_06,_07,_08,_09,_0a,_0b,_0c,_0d,      \
                 _0e,_0f,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_1a,_1b,      \
                 _1c,_1d,_1e,_1f,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,      \
                 _2a,_2b,_2c,_2d,_2e,_2f,_30,_31,_32,_33,_34,_35,_36,_37,      \
                 _38,_39,_3a,_3b,_3c,_3d,_3e,_3f,...)     _zx( _zex _zblock(   \
 _zrgp(f,_00) _zrgp(f,_01) _zrgp(f,_02) _zrgp(f,_03) _zrgp(f,_04) _zrgp(f,_05) \
 _zrgp(f,_06) _zrgp(f,_07) _zrgp(f,_08) _zrgp(f,_09) _zrgp(f,_0a) _zrgp(f,_0b) \
 _zrgp(f,_0c) _zrgp(f,_0d) _zrgp(f,_0e) _zrgp(f,_0f) _zrgp(f,_10) _zrgp(f,_11) \
 _zrgp(f,_12) _zrgp(f,_13) _zrgp(f,_14) _zrgp(f,_15) _zrgp(f,_16) _zrgp(f,_17) \
 _zrgp(f,_18) _zrgp(f,_19) _zrgp(f,_1a) _zrgp(f,_1b) _zrgp(f,_1c) _zrgp(f,_1d) \
 _zrgp(f,_1e) _zrgp(f,_1f) _zrgp(f,_20) _zrgp(f,_21) _zrgp(f,_22) _zrgp(f,_23) \
 _zrgp(f,_24) _zrgp(f,_25) _zrgp(f,_26) _zrgp(f,_27) _zrgp(f,_28) _zrgp(f,_29) \
 _zrgp(f,_2a) _zrgp(f,_2b) _zrgp(f,_2c) _zrgp(f,_2d) _zrgp(f,_2e) _zrgp(f,_2f) \
 _zrgp(f,_30) _zrgp(f,_31) _zrgp(f,_32) _zrgp(f,_33) _zrgp(f,_34) _zrgp(f,_35) \
 _zrgp(f,_36) _zrgp(f,_37) _zrgp(f,_38) _zrgp(f,_39) _zrgp(f,_3a) _zrgp(f,_3b) \
 _zrgp(f,_3c) _zrgp(f,_3d) _zrgp(f,_3e) _zrgp(f,_3f)  )))))))))))))))))))))))) \
   ))))))))))))))))))))))))))))))))))))))))))


#define _zrgp(f,arg) argp(arg,f)_zblock(arg,


#endif  /* ZARGS_H */

