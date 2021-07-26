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

