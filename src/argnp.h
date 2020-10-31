#ifndef HAVE_ARGNP
 #define HAVE_ARGNP

 #define argnp_detect_single(...) ),argnp_insert(
 #define argnp_insert(args...) args
 #define argnp_unwrap(args...) args

 #define argnp_result_multi(...) NONEMPTY

 #define _argnp_single_check() ,NONEMPTY
 #define _empty_argnp_single_check() ,EMPTY

 #define argnp(args...) argnp_i((args),args,argnp_detect_single,)
 #define argnp_i(reexpand...) argnp_ii(reexpand)

 #define argnp_ii(arglst,_,detect_single,...) argnp_iii(arglst,(detect_single() _single),_multi,)
 #define argnp_iii(reexpand...) argnp_iv(reexpand)

 #define argnp_iv(arglst,_,result,...) argnp_result ## result (argnp_unwrap arglst)

 #define argnp_result_single(args...) argnp_result_single_i(args _argnp)
 #define argnp_result_single_i(arg) argnp_cleanup( _empty ## arg ## _single_check() )

 #define argnp_cleanup(reexpand...) argnp_cleanup_i(reexpand)
 #define argnp_cleanup_i(_,result) result

 /* ARGP_OPT */
 #define argp_opt_EMPTY(...)
 #define argp_opt_NONEMPTY(args...) args

 #define argp_opt(out,va...) argp_opt_i(argnp(va)) out
 # define argp_opt_i(reexpand...) argp_opt_ii(reexpand)
 # define argp_opt_ii(result) argp_opt_ ## result

#endif /* HAVE_ARGNP */


#ifdef TEST_ARGP_OPT
 #define argp_opt_test_va(args...) args
 #define argp_opt_test_tostring(reexpand...) argp_opt_test_tostring_i(reexpand)
 #define argp_opt_test_tostring_i(args...) #args

 #define argp_opt_test_errmsg(type,text...) \
  argp_opt_test_errmsg_i(type, argp_opt_test_tostring(TEST FAILED: __FILE__:__LINE__ (text)))
 #define argp_opt_test_errmsg_i(reexpand...) argp_opt_test_errmsg_ii(reexpand)
 #define argp_opt_test_errmsg_ii(type,string) argp_opt_test_tostring(GCC type string)

 #define argp_opt_test_run(reexpand...) argp_opt_test_run_i(reexpand)
 #define argp_opt_test_run_i(text,_,maybe,...) \
  argp_opt_test_maybe_pragma(maybe, argp_opt_test_errmsg(error, argp_opt_test_ ## text()))

 #define argp_opt_test_maybe_pragma(reexpand...) argp_opt_test_maybe_pragma_i(reexpand)
 #define argp_opt_test_maybe_pragma_i(maybe,string) argp_opt_test ## maybe ## _do_pragma()(string)

 #define argp_opt_test_do_pragma() _Pragma
 #define argp_opt_test_dont_do_pragma() argp_opt_test_eat_unused
 #define argp_opt_test_eat_unused(...)

 #define argp_opt_test_expect_emit() no argument emitted where emission expected
 #define argp_opt_test_expect_noemit() argument emitted where null-expansion expected

 argp_opt_test_run( expect_noemit, argp_opt((,),)                       ,_dont, )
 argp_opt_test_run( expect_noemit, argp_opt((,), )                      ,_dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),x)                       _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), x)                      _dont, )
 argp_opt_test_run( expect_noemit, argp_opt((,),argp_opt_test_va())     ,_dont, )
 argp_opt_test_run( expect_noemit, argp_opt((,),argp_opt_test_va( ))    ,_dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(x))     _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( x))    _dont, )

 argp_opt_test_run( expect_emit,   argp_opt((,),,)                       _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), ,)                      _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),, )                      _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), , )                     _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(,))     _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( ,))    _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(, ))    _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( , ))   _dont, )

 argp_opt_test_run( expect_emit,   argp_opt((,),x,)                      _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), x,)                     _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),x, )                     _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), x, )                    _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(x,))    _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( x,))   _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(x, ))   _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( x, ))  _dont, )

 argp_opt_test_run( expect_emit,   argp_opt((,),,x)                      _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), ,x)                     _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),, x)                     _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), , x)                    _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(,x))    _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( ,x))   _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(, x))   _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( , x))  _dont, )

 argp_opt_test_run( expect_emit,   argp_opt((,),x,x)                     _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), x,x)                    _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),x, x)                    _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,), x, x)                   _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(x,x))   _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( x,x))  _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va(x, x))  _dont, )
 argp_opt_test_run( expect_emit,   argp_opt((,),argp_opt_test_va( x, x)) _dont, )

 #undef argp_opt_test_va
 #undef argp_opt_test_tostring
 #undef argp_opt_test_tostring_i
 #undef argp_opt_test_errmsg
 #undef argp_opt_test_errmsg_i
 #undef argp_opt_test_errmsg_ii
 #undef argp_opt_test_run
 #undef argp_opt_test_run_i
 #undef argp_opt_test_maybe_pragma
 #undef argp_opt_test_maybe_pragma_i
 #undef argp_opt_test_do_pragma
 #undef argp_opt_test_dont_do_pragma
 #undef argp_opt_test_eat_unused
 #undef argp_opt_test_expect_emit
 #undef argp_opt_test_expect_noemit
 #undef TEST_ARGP_OPT

#endif

#ifdef TEST_ARGNP

 argnp() : EMPTY
 argnp( ) : EMPTY
 argnp(x) : NONEMPTY

 argnp(,) : NONEMPTY
 argnp( ,) : NONEMPTY
 argnp(, ) : NONEMPTY
 argnp( , ) : NONEMPTY

 argnp(x,) : NONEMPTY
 argnp(,x) : NONEMPTY
 argnp(x, ) : NONEMPTY
 argnp( ,x) : NONEMPTY

 argnp(x,x) : NONEMPTY

 #undef TEST_ARGNP

#endif /* TEST_ARGNP */
