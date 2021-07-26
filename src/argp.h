#ifndef ARGP_H
#define ARGP_H

#define _argp_block
#define _argp_expand(...)    __VA_ARGS__
#define _argp_destroy(...)
#define _argp_delay(fn,argl) _argp_expand(fn _argp_block argl)


/* entry point */
#define argp(...) _argp_format ( __VA_ARGS__,  x,x,x,x,x,x,  3, 2, 1, )

/* logic */
#define _argp_format( arg,opt1,opt2,  _6,_5,_4,_3,_2,_1,  fmt, ... )        \
      _argp_dispatch                                                        \
         ( arg _do_,  arg,opt1,opt2,  fmt )

#define _argp_dispatch( pred,  arg,  opt1,opt2, fmt )                       \
      _argp_delay                                                           \
         (_argp_cleanup,(_##pred## argp_##fmt(arg,opt1,opt2)))

/* output generation */
# define  _do_argp_3(arg,tfn,ffn) ,tfn
# define __do_argp_3(arg,tfn,ffn) ,ffn
# define  _do_argp_2(arg,tfn,...) ,tfn
# define __do_argp_2(arg,tfn,...) ,_argp_destroy
# define  _do_argp_1(arg,...)     ,,
# define __do_argp_1(arg,...)     ,

#define _argp_cleanup(_,...) __VA_ARGS__


/* error path */

#define _argp_pragma_expand(...) __VA_ARGS__
#define _argp_unsafe_msg(msg)  #msg

/* invalid input concatenated for dispatch: defer to complainer */
# define  _do_argp_x(...) _argp_unsafe_error(__VA_ARGS__)
# define __do_argp_x(...) _argp_unsafe_error(__VA_ARGS__)

/* complainer */
#define _argp_unsafe_error(_x,_y,_z) ,                                 \
   _argp_pragma_expand( _Pragma                                        \
   _argp_pragma_expand((_argp_unsafe_msg                               \
   _argp_pragma_expand((GCC diagnstic error _argp_unsafe_msg           \
   _argp_pragma_expand((error: unshielded commas passed to macro argp  \
                        at __FILE__:__LINE__   (_x,_y,_z,...) )))))))

#endif /* ARGP_H */

