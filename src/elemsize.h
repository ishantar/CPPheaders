#ifndef  ELEMSIZE_H
 # define  ELEMSIZE_H
 # if ((! defined __USE_GNU) || (__USE_GNU == 0))
   # warning You seem to be compiling without GNU extensions;`elemsize.h` may not be useable.
 # endif


# define  elemsize(T)  sizeof(*(elemsize_deref(T)))
# define  elemsize_deref(T)   _Generic((T),  __typeof__(T) :&T,  default :T)

#endif
