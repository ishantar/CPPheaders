#ifndef ARGP_H
 #define ARGP_H

 #define EXPAND(...)    __VA_ARGS__
 #define OBSTRUCT
 #define DEFER(Fn,argl) EXPAND(Fn OBSTRUCT argl)


 #define ARGP(...) ARGP_format(__VA_ARGS__,3,2,1,)

 # define ARGP_format(arg,opt1,opt2,fmt,...)                          \
     ARGP_dispatch( arg do_, arg, opt1, opt2, fmt)

 # define ARGP_dispatch(pred,arg,opt1,opt2,fmt)                       \
EFER( ARGP_cleanup, (_##pred##argp_fmt##fmt(arg,opt1,opt2)))

 # define  do_argp_fmt3(arg,tfn,ffn) ,tfn
 # define _do_argp_fmt3(arg,tfn,ffn) ,ffn
 # define  do_argp_fmt2(arg,tfn,...) ,tfn(arg)
 # define _do_argp_fmt2(arg,tfn,...) ,
 # define  do_argp_fmt1(...)         ,1
 # define _do_argp_fmt1(...)         ,0

 # define ARGP_cleanup(_,...) __VA_ARGS__

#endif
