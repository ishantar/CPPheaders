#ifndef REVERSE_H
 #define REVERSE_H
 #include "zargs.h"

 #define _rvex(args...) args

 #define rvrs(args...) zargs( _rvrs, args )
 #define _rvrs(head,tail) _rvrs_i( tail,)head
 #define _rvrs_i(head,tail...) argp( head,_rvex)( head,tail)

#endif /* REVERSE_H */

