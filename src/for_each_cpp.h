#ifndef FOR_EACH_CPP_H
 #include "va_traverse.h"
 #define FOR_EACH_CPP_H 1

#define FOR_EACH(fn,...) FE_EX1 (F_E FOR_EACH_(fn,__VA_ARGS__) )
#define FOR_EACH_(fn,...)   (F_EACH(fn,__VA_ARGS__,,) )
// #define F_E(...)  F_E_(__VA_ARGS__)
 #define F_E(A,...) FE_EX2 A   F_E__ (__VA_ARGS__)
 #define F_E__(fn,...) F_ ## fn (__VA_ARGS__)

 #define F_(...) 
 #define FE_EX3(...) , va_drop1 __VA_ARGS__
 #define F_CHEAT(...)  DEFER(FE_EX3,(DEFER(va_take1,( F_CHEAT_(__VA_ARGS__) ))))
 #define F_CHEAT_(...) ( F_EACH1(__VA_ARGS__) 

 #define F_EACH(fn,arg,next,...) ( ARGP(arg,fn,FE_END)(arg) \
   ARGP(next,F_EACH1,FE_END)(fn,next,__VA_ARGS__)

 #define F_EACH1(fn,arg,next,...), fn(arg) ARGP(next,F_EACH2,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH2(fn,arg,next,...), fn(arg) ARGP(next,F_EACH3,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH3(fn,arg,next,...), fn(arg) ARGP(next,F_EACH4,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH4(fn,arg,next,...), fn(arg) ARGP(next,F_EACH5,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH5(fn,arg,next,...), fn(arg) ARGP(next,F_EACH6,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH6(fn,arg,next,...), fn(arg) ARGP(next,F_EACH7,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH7(fn,arg,next,...), fn(arg) ARGP(next,F_EACH8,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH8(fn,arg,next,...), fn(arg) ARGP(next,F_EACH9,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH9(fn,arg,next,...), fn(arg) ARGP(next,F_EACH10,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH10(fn,arg,next,...), fn(arg) ARGP(next,F_EACH11,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH11(fn,arg,next,...), fn(arg) ARGP(next,F_EACH12,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH12(fn,arg,next,...), fn(arg) ARGP(next,F_EACH13,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH13(fn,arg,next,...), fn(arg) ARGP(next,F_EACH14,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH14(fn,arg,next,...), fn(arg) ARGP(next,F_EACH15,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH15(fn,arg,next,...), fn(arg) ARGP(next,F_EACH16,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH16(fn,arg,next,...), fn(arg) ARGP(next,F_EACH17,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH17(fn,arg,next,...), fn(arg) ARGP(next,F_EACH18,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH18(fn,arg,next,...), fn(arg) ARGP(next,F_EACH19,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH19(fn,arg,next,...), fn(arg) ARGP(next,F_EACH20,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH20(fn,arg,next,...), fn(arg) ARGP(next,F_EACH21,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH21(fn,arg,next,...), fn(arg) ARGP(next,F_EACH22,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH22(fn,arg,next,...), fn(arg) ARGP(next,F_EACH23,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH23(fn,arg,next,...), fn(arg) ARGP(next,F_EACH24,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH24(fn,arg,next,...), fn(arg) ARGP(next,F_EACH25,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH25(fn,arg,next,...), fn(arg) ARGP(next,F_EACH26,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH26(fn,arg,next,...), fn(arg) ARGP(next,F_EACH27,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH27(fn,arg,next,...), fn(arg) ARGP(next,F_EACH28,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH28(fn,arg,next,...), fn(arg) ARGP(next,F_EACH29,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH29(fn,arg,next,...), fn(arg) ARGP(next,F_EACH30,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH30(fn,arg,next,...), fn(arg) ARGP(next,F_EACH31,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH31(fn,arg,next,...), fn(arg) ARGP(next,F_EACH32,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH32(fn,arg,next,...), fn(arg) ARGP(next,F_EACH33,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH33(fn,arg,next,...), fn(arg) ARGP(next,F_EACH34,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH34(fn,arg,next,...), fn(arg) ARGP(next,F_EACH35,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH35(fn,arg,next,...), fn(arg) ARGP(next,F_EACH36,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH36(fn,arg,next,...), fn(arg) ARGP(next,F_EACH37,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH37(fn,arg,next,...), fn(arg) ARGP(next,F_EACH38,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH38(fn,arg,next,...), fn(arg) ARGP(next,F_EACH39,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH39(fn,arg,next,...), fn(arg) ARGP(next,F_EACH40,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH40(fn,arg,next,...), fn(arg) ARGP(next,F_EACH41,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH41(fn,arg,next,...), fn(arg) ARGP(next,F_EACH42,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH42(fn,arg,next,...), fn(arg) ARGP(next,F_EACH43,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH43(fn,arg,next,...), fn(arg) ARGP(next,F_EACH44,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH44(fn,arg,next,...), fn(arg) ARGP(next,F_EACH45,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH45(fn,arg,next,...), fn(arg) ARGP(next,F_EACH46,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH46(fn,arg,next,...), fn(arg) ARGP(next,F_EACH47,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH47(fn,arg,next,...), fn(arg) ARGP(next,F_EACH48,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH48(fn,arg,next,...), fn(arg) ARGP(next,F_EACH49,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH49(fn,arg,next,...), fn(arg) ARGP(next,F_EACH50,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH50(fn,arg,next,...), fn(arg) ARGP(next,F_EACH51,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH51(fn,arg,next,...), fn(arg) ARGP(next,F_EACH52,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH52(fn,arg,next,...), fn(arg) ARGP(next,F_EACH53,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH53(fn,arg,next,...), fn(arg) ARGP(next,F_EACH54,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH54(fn,arg,next,...), fn(arg) ARGP(next,F_EACH55,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH55(fn,arg,next,...), fn(arg) ARGP(next,F_EACH56,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH56(fn,arg,next,...), fn(arg) ARGP(next,F_EACH57,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH57(fn,arg,next,...), fn(arg) ARGP(next,F_EACH58,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH58(fn,arg,next,...), fn(arg) ARGP(next,F_EACH59,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH59(fn,arg,next,...), fn(arg) ARGP(next,F_EACH60,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH60(fn,arg,next,...), fn(arg) ARGP(next,F_EACH61,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH61(fn,arg,next,...), fn(arg) ARGP(next,F_EACH62,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH62(fn,arg,next,...), fn(arg) ARGP(next,F_EACH63,FE_END)(fn,next,__VA_ARGS__)
 #define F_EACH63(fn,arg,next,...), fn(arg) ARGP(next,FE_CHEAT,FE_END)(fn,next,__VA_ARGS__)

 #define FE_test_fn(A) A ## __

#define FE_END(...) ), , 
#define FE_CHEAT(...) ), CHEAT,__VA_ARGS__ 
 #define FE_EX1(...) __VA_ARGS__
 #define FE_EX2(...) __VA_ARGS__

#define FE_test_long a,b,c, 2,7,24, 8hr,13,47, 2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1\
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
#define FE_test_short a,b,c, 2,7,24, ,8hr,13,47, 2,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1\
1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1


FOR_EACH( FE_test_fn, FE_test_short )
FOR_EACH( FE_test_fn, FE_test_long )

#endif
