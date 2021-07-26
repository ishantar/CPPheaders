#ifndef  ELEMSIZE_H
 #define  ELEMSIZE_H

 #define  elemsize(T)  sizeof(*(elemsize_deref(T)))
 #define  elemsize_deref(T)   _Generic((T),  __typeof__(T) :&T,  default :T)

#endif

