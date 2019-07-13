/**
 * aliasing and variadic-izing macro definitions for stdatomic.h
 * for reducing clutter and fostering readability
 *
 * reduces function calls to 'explicit' atomic operations by minimum 
 * 22 characters (up to 55 in the case of the worst offender)
 * example: trivial compare-exchange spinloop
 *
 * while (atomic_compare_exchange_weak_explicit(ptr, exp, des, memory_order_acq_rel, memory_order_relaxed)) {;}
 *  ...
 * while (atomic_cxw(ptr, exp, des, ACQ_REL, RELAXED)) {;}
 *
 * otherwise-horrid text formatting choices are intended to be 
 * hopefully demonstrative of the structure and function behind
 * the cascading macro expansions
 **/


/**
 * when present, optional memory-order arguments increase the number of
 * parameters __VA_ARGS__ expands to fill in the following call. this 
 * displaces the trailing token args, thereby populating MBA parameters
 * and altering the value of TOK
 **/

#include <stdatomic.h>

#define ca_fn(N,...)                                          ca_arity##N ( __VA_ARGS__)

#define ca_arity1(...)                                        ca_arity1_2 ( __VA_ARGS__, b, a, )
#define ca_arity1_2(FN,ARG,MBA2,TOK,...)                 ca_arity1_3##TOK ( FN,  ARG, MBA2)
#define ca_arity1_3a(FN,ARG,...)                                       FN (      ARG)
#define ca_arity1_3b(FN,ARG,ARG2)                           FN##_explicit (      ARG, ARG2)

#define ca_arity2(...)                                        ca_arity2_2 (__VA_ARGS__, b, a, )
#define ca_arity2_2(FN,ARG1,ARG2,MBA3,TOK,...)           ca_arity2_3##TOK ( FN, ARG1, ARG2, MBA3)
#define ca_arity2_3a(FN,ARG1,ARG2,...)                                 FN (     ARG1, ARG2)
#define ca_arity2_3b(FN,ARG1,ARG2,ARG3)                     FN##_explicit (     ARG1, ARG2, ARG3)

#define ca_arity3(...)                                        ca_arity3_2 ( __VA_ARGS__, b, c, a, )
#define ca_arity3_2(FN,ARG1,ARG2,ARG3,MBA4,MBA5,TOK,...) ca_arity3_3##TOK ( FN, ARG1, ARG2, ARG3, MBA4, MBA5)
#define ca_arity3_3a(FN,ARG1,ARG2,ARG3,...)                            FN (     ARG1, ARG2, ARG3)
#define ca_arity3_3b(FN,ARG1,ARG2,ARG3,ARG4,ARG5)           FN##_explicit (     ARG1, ARG2, ARG3, ARG4, ARG5)



#define atomic_clr(...)     ca_fn( 1, atomic_flag_clear,              __VA_ARGS__)
#define atomic_tst_set(...) ca_fn( 1, atomic_flag_test_and_set,       __VA_ARGS__)

#define atomic_cxs(...)     ca_fn( 3, atomic_compare_exchange_strong, __VA_ARGS__)
#define atomic_cxw(...)     ca_fn( 3, atomic_compare_exchange_weak,   __VA_ARGS__)

#define atomic_ex(...)      ca_fn( 2, atomic_exchange,  __VA_ARGS__)

#define atomic_Fand(...)    ca_fn( 2, atomic_fetch_and, __VA_ARGS__)
#define atomic_For(...)     ca_fn( 2, atomic_fetch_or,  __VA_ARGS__)
#define atomic_Fxor(...)    ca_fn( 2, atomic_fetch_xor, __VA_ARGS__)
#define atomic_Fadd(...)    ca_fn( 2, atomic_fetch_add, __VA_ARGS__)
#define atomic_Fsub(...)    ca_fn( 2, atomic_fetch_sub, __VA_ARGS__)

#define atomic_Store(...)   ca_fn( 2, atomic_store,     __VA_ARGS__)
#define atomic_Load(...)    ca_fn( 1, atomic_load,      __VA_ARGS__)


#define RELAXED memory_order_relaxed
#define CONSUME memory_order_consume
#define ACQUIRE memory_order_acquire
#define RELEASE memory_order_release
#define ACQ_REL memory_order_acq_rel
#define SEQ_CST memory_order_seq_cst


