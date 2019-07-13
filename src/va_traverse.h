#include "cpparith.h"


/* basic expansion manipulating primitives */

#define IDENT(...)         __VA_ARGS__
#define EXPAND(...)        __VA_ARGS__
#define DISCARD(...)
#define INJECT(...)        ,__VA_ARGS__
#define OBSTRUCT
#define OBS
#define DEFER(Fn,argl)     EXPAND(Fn OBSTRUCT argl)
#define va_wrap(...)       (__VA_ARGS__)
#define va_peel(argl)      EXPAND argl
#define va_drop1(head,...) __VA_ARGS__
#define va_take1(head,...) head


/* single argument "if-empty" conditional 
 * "true" if replacement text contains at least one non-whitespace token
 *
 * the first argument supplied is the test argument. supplying no arguments
 * effectively specifies an empty first argument.
 *
 * result format is specified using a variadic interface.  zero, one, or two
 * additional arguments may be supplied after the test argument.
 * -when zero additional arguments are present, the result is an appropriately
 * testable int literal (1/true or 0/false)
 * -when two additional arguments are present, the result the value of either
 * the first(true) or the second(false) additional argument. note that by
 * supplying the names of function-like macros, arbitrary additional effects
 * can be conditionally specified
 * -supplying one additional argument is a shorthand notation: the argument 
 * contents are not significant. this mode's output is appropriate for 
 * 'append-if-nonempty' list constructing behaviour: specifically, the true
 * case expands to ', arg` (where 'arg` is the test argument), and the false
 * case expands to nothing.
 *
 *
 * test mechanism overview: in ARGP_dispatch, the replacement list fragment
 * '_##pred##argp_[...]` will effectively be replaced by either:
 *   a)   '_## [something] do_##argp_[...]'   >>> '_[something] do_argp_[...]`
Q *   b)   '_##  [nothing]  do_##argp_[...]'   >>> '            _do_argp_[...]`
 * ...thus leading to different macro names being invoked
 *
 * '[something/nothing]` is the content of the test argument
 */


   /* append position indicators for the variadic interface */
#define ARGP(...) ARGP_format(__VA_ARGS__,3,2,1,)

   /* select the indicator value and generate a testable 'arg` (note commas) */
#define ARGP_format(arg,opt1,opt2,fmt,...)       \
     ARGP_dispatch( arg do_, arg, opt1, opt2, fmt)

   /* concat our way to victory */
#define ARGP_dispatch(pred,arg,opt1,opt2,fmt)                            \
     DEFER( ARGP_cleanup, (_##pred##argp_fmt##fmt(arg,opt1,opt2)))

   /* don't emit stray garbage */
#define ARGP_cleanup(_,...) __VA_ARGS__

   /* dispatch targets */
#define  do_argp_fmt3(arg,tfn,ffn) ,tfn
#define _do_argp_fmt3(arg,tfn,ffn) ,ffn
#define  do_argp_fmt2(arg,...)     ,,arg      /* second comma appears in result */
#define _do_argp_fmt2(arg,...)     ,
#define  do_argp_fmt1(...)         ,1
#define _do_argp_fmt1(...)         ,0
/* comma injection in result macros ensures that any unwanted tokens left by
 * concatenation can then be argument-seperated and stripped during ARGP_cleanup
 */


/* for_each impl (primary list-comprehension traversal)
 * 
 */

/*  currently 63 commas (ensuring up to 64 parameters)  */
#define va_commas ,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,,
#define va_n_max(...) __VA_ARGS__ va_commas

#define va_each(fn,init,...)   va_eachi(fn,init,__VA_ARGS__ va_commas)
#define va_eachi(fn,init,...)  each(fn,init,__VA_ARGS__)

// it literally can't be readable, might as well make it small
#define each(fn,init,_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,_17,\
                _18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33,\
                _34,_35,_36,_37,_38,_39,_40,_41,_42,_43,_44,_45,_46,_47,_48,_49,\
                _50,_51,_52,_53,_54,_55,_56,_57,_58,_59,_60,_61,_62,_63,_64,...)\
fn (_64,fn (_63,fn (_62,fn (_61,fn (_60,fn (_59,fn (_58,fn (_57,fn (_56,fn (_55,\
fn (_54,fn (_53,fn (_52,fn (_51,fn (_50,fn (_49,fn (_48,fn (_47,fn (_46,fn (_45,\
fn (_44,fn (_43,fn (_42,fn (_41,fn (_40,fn (_39,fn (_38,fn (_37,fn (_36,fn (_35,\
fn (_34,fn (_33,fn (_32,fn (_31,fn (_30,fn (_29,fn (_28,fn (_27,fn (_26,fn (_25,\
fn (_24,fn (_23,fn (_22,fn (_21,fn (_20,fn (_19,fn (_18,fn (_17,fn (_16,fn (_15,\
fn (_14,fn (_13,fn (_12,fn (_11,fn (_10,fn (_9,fn  (_8,fn  (_7,fn  (_6,fn  (_5, \
fn (_4,fn  (_3,fn  (_2,fn  (_1,init( ) )))))))))))))))))))))))))))))))))))))))))\
 )))))))))))))))))))))))




/*  for_each comprehensions  */


   // truncate: strip tailing args that are empty
#define trunc_init(A)          (A),(A)
#define iEXPAND(...)           __VA_ARGS__
#define newcandl(a,outl,candl) (iEXPAND outl,a)
#define oldcandl(a,outl,candl) candl
#define trunc(a,...)           trnc(a,__VA_ARGS__)
#define trncx(...)             trnc(__VA_ARGS__)
#define trnc(a,outl,candl)     DEFER(trnci,(IDENT(ARGP(a,newcandl,oldcandl),outl,candl)))
#define trnci(fn,a,outl,candl) (iEXPAND outl,a),fn(a,outl,candl)
#define va_trunc(...)          EXPAND(va_drop1 EXPAND(DEFER(va_drop1,\
                                 (va_each(trunc,trunc_init,__VA_ARGS__)))))

   // count: compute the number of list elements, ignoring trailing-empty elems
#define count_init( )          (0),(0)
#define ncandc(fullc,candc)    (INCR fullc)
#define ocandc(fullc,candc)    candc
#define acount(a,...)          acnt(a,__VA_ARGS__)
#define acnt(a,fullc,candc)    DEFER(acnti,(IDENT(ARGP(a,ncandc,ocandc),fullc,candc)))
#define acnti(fn,a,fullc,candc)(INCR fullc),fn(fullc,candc)
#define va_count(...)          va_peel(EXPAND(DEFER(va_drop1,\
                                 (va_each(acount,count_init,__VA_ARGS__)))))


  
                //va_trunc(a,b, ,2,3,4, , ,6,7,8,9,,10,,)
                //va_count(a,b, ,2,3,4, , ,6,7,8,9,,10,,)
