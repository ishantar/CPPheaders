#ifndef REVERSE_H
#define REVERSE_H
#include "zargs.h"


#define _rvex(args...) args



#define rvrs(args...) zargs( _rvrs, args )

#define _rvrs(head,tail) _rvrs_i( tail,)head

#define _rvrs_i(head,tail...) argp( head,_rvex)( head,tail)






#ifdef RVRS_H_TST

    
0: rvrs()                                
1: rvrs(a)                               
2: rvrs(a,b)                             
3: rvrs(a,b,c)                           
4: rvrs(a,b,c,d)                         
5: rvrs(a,b,c,d,e)                       
6: rvrs(a,b,c,d,e,f)                     
7: rvrs(a,b,c,d,e,f,g)                   
8: rvrs(a,b,c,d,e,f,g,h)                 
9: rvrs(a,b,c,d,e,f,g,h,i)               
a: rvrs(a,b,c,d,e,f,g,h,i,j)             
b: rvrs(a,b,c,d,e,f,g,h,i,j,k)           
c: rvrs(a,b,c,d,e,f,g,h,i,j,k,l)         
d: rvrs(a,b,c,d,e,f,g,h,i,j,k,l,m)       
e: rvrs(a,b,c,d,e,f,g,h,i,j,k,l,m,n)    
f: rvrs(a,b,c,d,e,f,g,h,i,j,k,l,m,n,o)  

65:
rvrs(01,02,03,04,05,06,07,08,09,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,
     25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,
     49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,__)


#endif /* RVRS_H_TST */


#endif /* REVERSE_H */
