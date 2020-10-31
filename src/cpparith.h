/* BUGS
 *
 * invalid input catastrophic (unresolveable?)
 *
 * seq(0,n) emits nothing (due to sub(n,decr(0)))
 *
 * divide/mod by zero emits garbage
 * 0/0 -> 0
 *
 * rep(n[,]) emits n comma-separated nothings (n-1 total)
 *    remove, or add n'th?
 *
 *
 */


#ifndef CPPARITH_H
 #define CPPARITH_H 1

 #define ARITH_BLOCK
 #define ARITH_EXPAND(...)        __VA_ARGS__
 #define ARITH_DEFER(Fn,...)      ARITH_EXPAND( Fn ARITH_BLOCK ( ARITH_EXPAND(__VA_ARGS__) ))

 #define ARITH_HEAD(list...)      ARITH_HEAD_i(list)
 #define ARITH_HEAD_i(car,cdr...) car
 #define ARITH_TAIL(list...)      ARITH_TAIL_i(list)
 #define ARITH_TAIL_i(car,cdr...) cdr

 #define ARITH_CATN(OP,N)         ARITH_DEFER( ARITH_CATN_i, OP, N )
 #define ARITH_CATN_i(OP,N)       ARITH_ ## OP ## _ ## N




 #define ARITH_INCR(N)            ARITH_CATN( INCR, N )
 #define ARITH_DECR(N)            ARITH_CATN( DECR, N )

 #define ARITH_INCR_64            ARITH_ERR_MAX
 #define ARITH_INCR_ARITH_ERR_MAX ARITH_ERR_MAX
 #define ARITH_DECR_0             ARITH_ERR_MIN
 #define ARITH_DECR_ARITH_ERR_MIN ARITH_ERR_MIN



 #define ARITH_ADD(N,M)           ARITH_HEAD(ARITH_CATN( ADD, M ) (ARITH_ADDfn,N))
 #define ARITH_ADDfn(...)
 #define ARITH_ADDfn_0(N)         N

 #define ARITH_ADD_0(Fn,N)        Fn##_0(N)


 #define ARITH_SUB(N,M)           ARITH_HEAD(ARITH_CATN( SUB, M ) (ARITH_SUBfn,N,))
 #define ARITH_SUBfn(...)
 #define ARITH_SUBfn_0(N,REP...)  N

 #define ARITH_SUB_0(Fn,N,REP...) Fn##_0 (N,REP)
 #define ARITH_SUB_ARITH_ERR_MIN(...) ARITH_ERR_MIN


 #define ARITH_REP(N,REP...)      ARITH_TAIL(ARITH_CATN( SUB, N ) (ARITH_REPfn,N,REP))
 #define ARITH_REPfn(N,REP...)    , REP
 #define ARITH_REPfn_0(...)



 #define ARITH_INDEX_GE(N,M,NG,MG) ARITH_CATN( GE, ARITH_SUB(ARITH_SUB(N,M),ARITH_SUB(N,M)) ) (NG,MG)
 #define ARITH_INDEX_LE(N,M,NL,ML) ARITH_CATN( GE, ARITH_SUB(ARITH_SUB(M,N),ARITH_SUB(M,N)) ) (NL,ML)
 #define ARITH_GE_0(N,M)          N
 #define ARITH_GE_ARITH_ERR_MIN(N,M) M


 #define ARITH_MAX(N,M)           ARITH_INDEX_GE(N,M,N,M)
 #define ARITH_MIN(N,M)           ARITH_INDEX_LE(N,M,N,M)


 #define ARITH_SEQ(N,M)           ARITH_SEQ_i(ARITH_INDEX_GE(N,M,n,p), N, M)
 #define ARITH_SEQ_i(C,N,M)       ARITH_SEQ_ii(C,N,M)
 #define ARITH_SEQ_ii(C,N,M)      ARITH_CATN( SEQ, C ) (ARITH_SEQ##C##fn,N,M)

 #define ARITH_SEQ_p(C,N,M)       ARITH_TAIL(ARITH_CATN (SUB, ARITH_SUB(ARITH_INCR(M),N)) (C,M))
 #define ARITH_SEQ_n(C,N,M)       ARITH_CATN(ADD, ARITH_SUB(N,M)) (C,M)

 #define ARITH_SEQpfn(N,...)      , N
 #define ARITH_SEQpfn_0(...)

 #define ARITH_SEQnfn(N,...)      , N
 #define ARITH_SEQnfn_0(N)        N



 #define ARITH_MUL(N,M)           ARITH_CATN( MUL, M ) (N,0)
 #define ARITH_DIV(N,M)           ARITH_CATN( DIV, N ) (ARITH_DIVfn,M,0,0)
 #define ARITH_MOD(N,M)           ARITH_CATN( DIV, N ) (ARITH_MODfn,M,0,0)

 #define ARITH_MUL_0(n,i)         0
 #define ARITH_MUL_1(n,i)         ARITH_ADD(i,n)

 #define ARITH_DIV_0(fn,m,i,p)    fn##_0 (i)
 #define ARITH_DIVfn_0(i)         i
 #define ARITH_MODfn_0(i)         0

 #define ARITH_DIVfn_ERR(i,p)     i
 #define ARITH_MODfn_ERR(i,p)     p
 #define ARITH_DIV_ARITH_ERR_MIN(fn,m,i,p) fn##_ERR (ARITH_DECR(i),p)




 #define ARITH_INCR_0    1
 #define ARITH_INCR_1    2
 #define ARITH_INCR_2    3
 #define ARITH_INCR_3    4
 #define ARITH_INCR_4    5
 #define ARITH_INCR_5    6
 #define ARITH_INCR_6    7
 #define ARITH_INCR_7    8
 #define ARITH_INCR_8    9
 #define ARITH_INCR_9   10
 #define ARITH_INCR_10  11
 #define ARITH_INCR_11  12
 #define ARITH_INCR_12  13
 #define ARITH_INCR_13  14
 #define ARITH_INCR_14  15
 #define ARITH_INCR_15  16
 #define ARITH_INCR_16  17
 #define ARITH_INCR_17  18
 #define ARITH_INCR_18  19
 #define ARITH_INCR_19  20
 #define ARITH_INCR_20  21
 #define ARITH_INCR_21  22
 #define ARITH_INCR_22  23
 #define ARITH_INCR_23  24
 #define ARITH_INCR_24  25
 #define ARITH_INCR_25  26
 #define ARITH_INCR_26  27
 #define ARITH_INCR_27  28
 #define ARITH_INCR_28  29
 #define ARITH_INCR_29  30
 #define ARITH_INCR_30  31
 #define ARITH_INCR_31  32
 #define ARITH_INCR_32  33
 #define ARITH_INCR_33  34
 #define ARITH_INCR_34  35
 #define ARITH_INCR_35  36
 #define ARITH_INCR_36  37
 #define ARITH_INCR_37  38
 #define ARITH_INCR_38  39
 #define ARITH_INCR_39  40
 #define ARITH_INCR_40  41
 #define ARITH_INCR_41  42
 #define ARITH_INCR_42  43
 #define ARITH_INCR_43  44
 #define ARITH_INCR_44  45
 #define ARITH_INCR_45  46
 #define ARITH_INCR_46  47
 #define ARITH_INCR_47  48
 #define ARITH_INCR_48  49
 #define ARITH_INCR_49  50
 #define ARITH_INCR_50  51
 #define ARITH_INCR_51  52
 #define ARITH_INCR_52  53
 #define ARITH_INCR_53  54
 #define ARITH_INCR_54  55
 #define ARITH_INCR_55  56
 #define ARITH_INCR_56  57
 #define ARITH_INCR_57  58
 #define ARITH_INCR_58  59
 #define ARITH_INCR_59  60
 #define ARITH_INCR_60  61
 #define ARITH_INCR_61  62
 #define ARITH_INCR_62  63
 #define ARITH_INCR_63  64

 #define ARITH_DECR_1    0
 #define ARITH_DECR_2    1
 #define ARITH_DECR_3    2
 #define ARITH_DECR_4    3
 #define ARITH_DECR_5    4
 #define ARITH_DECR_6    5
 #define ARITH_DECR_7    6
 #define ARITH_DECR_8    7
 #define ARITH_DECR_9    8
 #define ARITH_DECR_10   9
 #define ARITH_DECR_11  10
 #define ARITH_DECR_12  11
 #define ARITH_DECR_13  12
 #define ARITH_DECR_14  13
 #define ARITH_DECR_15  14
 #define ARITH_DECR_16  15
 #define ARITH_DECR_17  16
 #define ARITH_DECR_18  17
 #define ARITH_DECR_19  18
 #define ARITH_DECR_20  19
 #define ARITH_DECR_21  20
 #define ARITH_DECR_22  21
 #define ARITH_DECR_23  22
 #define ARITH_DECR_24  23
 #define ARITH_DECR_25  24
 #define ARITH_DECR_26  25
 #define ARITH_DECR_27  26
 #define ARITH_DECR_28  27
 #define ARITH_DECR_29  28
 #define ARITH_DECR_30  29
 #define ARITH_DECR_31  30
 #define ARITH_DECR_32  31
 #define ARITH_DECR_33  32
 #define ARITH_DECR_34  33
 #define ARITH_DECR_35  34
 #define ARITH_DECR_36  35
 #define ARITH_DECR_37  36
 #define ARITH_DECR_38  37
 #define ARITH_DECR_39  38
 #define ARITH_DECR_40  39
 #define ARITH_DECR_41  40
 #define ARITH_DECR_42  41
 #define ARITH_DECR_43  42
 #define ARITH_DECR_44  43
 #define ARITH_DECR_45  44
 #define ARITH_DECR_46  45
 #define ARITH_DECR_47  46
 #define ARITH_DECR_48  47
 #define ARITH_DECR_49  48
 #define ARITH_DECR_50  49
 #define ARITH_DECR_51  50
 #define ARITH_DECR_52  51
 #define ARITH_DECR_53  52
 #define ARITH_DECR_54  53
 #define ARITH_DECR_55  54
 #define ARITH_DECR_56  55
 #define ARITH_DECR_57  56
 #define ARITH_DECR_58  57
 #define ARITH_DECR_59  58
 #define ARITH_DECR_60  59
 #define ARITH_DECR_61  60
 #define ARITH_DECR_62  61
 #define ARITH_DECR_63  62
 #define ARITH_DECR_64  63

 #define ARITH_ADD_1(fn,n) ARITH_ADD_0(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_2(fn,n) ARITH_ADD_1(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_3(fn,n) ARITH_ADD_2(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_4(fn,n) ARITH_ADD_3(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_5(fn,n) ARITH_ADD_4(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_6(fn,n) ARITH_ADD_5(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_7(fn,n) ARITH_ADD_6(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_8(fn,n) ARITH_ADD_7(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_9(fn,n) ARITH_ADD_8(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_10(fn,n) ARITH_ADD_9(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_11(fn,n) ARITH_ADD_10(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_12(fn,n) ARITH_ADD_11(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_13(fn,n) ARITH_ADD_12(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_14(fn,n) ARITH_ADD_13(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_15(fn,n) ARITH_ADD_14(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_16(fn,n) ARITH_ADD_15(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_17(fn,n) ARITH_ADD_16(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_18(fn,n) ARITH_ADD_17(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_19(fn,n) ARITH_ADD_18(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_20(fn,n) ARITH_ADD_19(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_21(fn,n) ARITH_ADD_20(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_22(fn,n) ARITH_ADD_21(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_23(fn,n) ARITH_ADD_22(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_24(fn,n) ARITH_ADD_23(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_25(fn,n) ARITH_ADD_24(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_26(fn,n) ARITH_ADD_25(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_27(fn,n) ARITH_ADD_26(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_28(fn,n) ARITH_ADD_27(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_29(fn,n) ARITH_ADD_28(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_30(fn,n) ARITH_ADD_29(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_31(fn,n) ARITH_ADD_30(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_32(fn,n) ARITH_ADD_31(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_33(fn,n) ARITH_ADD_32(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_34(fn,n) ARITH_ADD_33(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_35(fn,n) ARITH_ADD_34(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_36(fn,n) ARITH_ADD_35(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_37(fn,n) ARITH_ADD_36(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_38(fn,n) ARITH_ADD_37(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_39(fn,n) ARITH_ADD_38(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_40(fn,n) ARITH_ADD_39(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_41(fn,n) ARITH_ADD_40(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_42(fn,n) ARITH_ADD_41(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_43(fn,n) ARITH_ADD_42(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_44(fn,n) ARITH_ADD_43(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_45(fn,n) ARITH_ADD_44(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_46(fn,n) ARITH_ADD_45(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_47(fn,n) ARITH_ADD_46(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_48(fn,n) ARITH_ADD_47(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_49(fn,n) ARITH_ADD_48(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_50(fn,n) ARITH_ADD_49(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_51(fn,n) ARITH_ADD_50(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_52(fn,n) ARITH_ADD_51(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_53(fn,n) ARITH_ADD_52(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_54(fn,n) ARITH_ADD_53(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_55(fn,n) ARITH_ADD_54(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_56(fn,n) ARITH_ADD_55(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_57(fn,n) ARITH_ADD_56(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_58(fn,n) ARITH_ADD_57(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_59(fn,n) ARITH_ADD_58(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_60(fn,n) ARITH_ADD_59(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_61(fn,n) ARITH_ADD_60(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_62(fn,n) ARITH_ADD_61(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_63(fn,n) ARITH_ADD_62(fn,ARITH_INCR(n)) fn(n,)
 #define ARITH_ADD_64(fn,n) ARITH_ADD_63(fn,ARITH_INCR(n)) fn(n,)

 #define ARITH_SUB_1(fn,n,rep...) ARITH_SUB_0(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_2(fn,n,rep...) ARITH_SUB_1(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_3(fn,n,rep...) ARITH_SUB_2(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_4(fn,n,rep...) ARITH_SUB_3(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_5(fn,n,rep...) ARITH_SUB_4(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_6(fn,n,rep...) ARITH_SUB_5(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_7(fn,n,rep...) ARITH_SUB_6(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_8(fn,n,rep...) ARITH_SUB_7(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_9(fn,n,rep...) ARITH_SUB_8(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_10(fn,n,rep...) ARITH_SUB_9(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_11(fn,n,rep...) ARITH_SUB_10(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_12(fn,n,rep...) ARITH_SUB_11(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_13(fn,n,rep...) ARITH_SUB_12(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_14(fn,n,rep...) ARITH_SUB_13(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_15(fn,n,rep...) ARITH_SUB_14(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_16(fn,n,rep...) ARITH_SUB_15(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_17(fn,n,rep...) ARITH_SUB_16(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_18(fn,n,rep...) ARITH_SUB_17(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_19(fn,n,rep...) ARITH_SUB_18(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_20(fn,n,rep...) ARITH_SUB_19(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_21(fn,n,rep...) ARITH_SUB_20(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_22(fn,n,rep...) ARITH_SUB_21(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_23(fn,n,rep...) ARITH_SUB_22(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_24(fn,n,rep...) ARITH_SUB_23(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_25(fn,n,rep...) ARITH_SUB_24(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_26(fn,n,rep...) ARITH_SUB_25(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_27(fn,n,rep...) ARITH_SUB_26(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_28(fn,n,rep...) ARITH_SUB_27(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_29(fn,n,rep...) ARITH_SUB_28(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_30(fn,n,rep...) ARITH_SUB_29(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_31(fn,n,rep...) ARITH_SUB_30(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_32(fn,n,rep...) ARITH_SUB_31(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_33(fn,n,rep...) ARITH_SUB_32(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_34(fn,n,rep...) ARITH_SUB_33(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_35(fn,n,rep...) ARITH_SUB_34(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_36(fn,n,rep...) ARITH_SUB_35(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_37(fn,n,rep...) ARITH_SUB_36(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_38(fn,n,rep...) ARITH_SUB_37(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_39(fn,n,rep...) ARITH_SUB_38(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_40(fn,n,rep...) ARITH_SUB_39(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_41(fn,n,rep...) ARITH_SUB_40(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_42(fn,n,rep...) ARITH_SUB_41(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_43(fn,n,rep...) ARITH_SUB_42(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_44(fn,n,rep...) ARITH_SUB_43(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_45(fn,n,rep...) ARITH_SUB_44(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_46(fn,n,rep...) ARITH_SUB_45(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_47(fn,n,rep...) ARITH_SUB_46(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_48(fn,n,rep...) ARITH_SUB_47(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_49(fn,n,rep...) ARITH_SUB_48(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_50(fn,n,rep...) ARITH_SUB_49(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_51(fn,n,rep...) ARITH_SUB_50(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_52(fn,n,rep...) ARITH_SUB_51(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_53(fn,n,rep...) ARITH_SUB_52(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_54(fn,n,rep...) ARITH_SUB_53(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_55(fn,n,rep...) ARITH_SUB_54(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_56(fn,n,rep...) ARITH_SUB_55(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_57(fn,n,rep...) ARITH_SUB_56(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_58(fn,n,rep...) ARITH_SUB_57(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_59(fn,n,rep...) ARITH_SUB_58(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_60(fn,n,rep...) ARITH_SUB_59(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_61(fn,n,rep...) ARITH_SUB_60(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_62(fn,n,rep...) ARITH_SUB_61(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_63(fn,n,rep...) ARITH_SUB_62(fn,ARITH_DECR(n),rep) fn(n,rep)
 #define ARITH_SUB_64(fn,n,rep...) ARITH_SUB_63(fn,ARITH_DECR(n),rep) fn(n,rep)

 #define ARITH_MUL_2(n,i) ARITH_MUL_1( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_3(n,i) ARITH_MUL_2( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_4(n,i) ARITH_MUL_3( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_5(n,i) ARITH_MUL_4( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_6(n,i) ARITH_MUL_5( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_7(n,i) ARITH_MUL_6( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_8(n,i) ARITH_MUL_7( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_9(n,i) ARITH_MUL_8( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_10(n,i) ARITH_MUL_9( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_11(n,i) ARITH_MUL_10( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_12(n,i) ARITH_MUL_11( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_13(n,i) ARITH_MUL_12( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_14(n,i) ARITH_MUL_13( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_15(n,i) ARITH_MUL_14( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_16(n,i) ARITH_MUL_15( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_17(n,i) ARITH_MUL_16( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_18(n,i) ARITH_MUL_17( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_19(n,i) ARITH_MUL_18( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_20(n,i) ARITH_MUL_19( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_21(n,i) ARITH_MUL_20( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_22(n,i) ARITH_MUL_21( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_23(n,i) ARITH_MUL_22( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_24(n,i) ARITH_MUL_23( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_25(n,i) ARITH_MUL_24( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_26(n,i) ARITH_MUL_25( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_27(n,i) ARITH_MUL_26( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_28(n,i) ARITH_MUL_27( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_29(n,i) ARITH_MUL_28( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_30(n,i) ARITH_MUL_29( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_31(n,i) ARITH_MUL_30( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_32(n,i) ARITH_MUL_31( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_33(n,i) ARITH_MUL_32( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_34(n,i) ARITH_MUL_33( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_35(n,i) ARITH_MUL_34( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_36(n,i) ARITH_MUL_35( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_37(n,i) ARITH_MUL_36( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_38(n,i) ARITH_MUL_37( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_39(n,i) ARITH_MUL_38( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_40(n,i) ARITH_MUL_39( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_41(n,i) ARITH_MUL_40( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_42(n,i) ARITH_MUL_41( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_43(n,i) ARITH_MUL_42( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_44(n,i) ARITH_MUL_43( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_45(n,i) ARITH_MUL_44( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_46(n,i) ARITH_MUL_45( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_47(n,i) ARITH_MUL_46( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_48(n,i) ARITH_MUL_47( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_49(n,i) ARITH_MUL_48( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_50(n,i) ARITH_MUL_49( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_51(n,i) ARITH_MUL_50( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_52(n,i) ARITH_MUL_51( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_53(n,i) ARITH_MUL_52( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_54(n,i) ARITH_MUL_53( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_55(n,i) ARITH_MUL_54( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_56(n,i) ARITH_MUL_55( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_57(n,i) ARITH_MUL_56( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_58(n,i) ARITH_MUL_57( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_59(n,i) ARITH_MUL_58( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_60(n,i) ARITH_MUL_59( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_61(n,i) ARITH_MUL_60( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_62(n,i) ARITH_MUL_61( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_63(n,i) ARITH_MUL_62( n,ARITH_ADD(i,n) )
 #define ARITH_MUL_64(n,i) ARITH_MUL_63( n,ARITH_ADD(i,n) )

 #define ARITH_DIV_1(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(1,m)) (fn,m,ARITH_INCR(i),1)
 #define ARITH_DIV_2(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(2,m)) (fn,m,ARITH_INCR(i),2)
 #define ARITH_DIV_3(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(3,m)) (fn,m,ARITH_INCR(i),3)
 #define ARITH_DIV_4(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(4,m)) (fn,m,ARITH_INCR(i),4)
 #define ARITH_DIV_5(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(5,m)) (fn,m,ARITH_INCR(i),5)
 #define ARITH_DIV_6(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(6,m)) (fn,m,ARITH_INCR(i),6)
 #define ARITH_DIV_7(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(7,m)) (fn,m,ARITH_INCR(i),7)
 #define ARITH_DIV_8(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(8,m)) (fn,m,ARITH_INCR(i),8)
 #define ARITH_DIV_9(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(9,m)) (fn,m,ARITH_INCR(i),9)
 #define ARITH_DIV_10(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(10,m)) (fn,m,ARITH_INCR(i),10)
 #define ARITH_DIV_11(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(11,m)) (fn,m,ARITH_INCR(i),11)
 #define ARITH_DIV_12(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(12,m)) (fn,m,ARITH_INCR(i),12)
 #define ARITH_DIV_13(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(13,m)) (fn,m,ARITH_INCR(i),13)
 #define ARITH_DIV_14(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(14,m)) (fn,m,ARITH_INCR(i),14)
 #define ARITH_DIV_15(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(15,m)) (fn,m,ARITH_INCR(i),15)
 #define ARITH_DIV_16(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(16,m)) (fn,m,ARITH_INCR(i),16)
 #define ARITH_DIV_17(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(17,m)) (fn,m,ARITH_INCR(i),17)
 #define ARITH_DIV_18(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(18,m)) (fn,m,ARITH_INCR(i),18)
 #define ARITH_DIV_19(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(19,m)) (fn,m,ARITH_INCR(i),19)
 #define ARITH_DIV_20(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(20,m)) (fn,m,ARITH_INCR(i),20)
 #define ARITH_DIV_21(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(21,m)) (fn,m,ARITH_INCR(i),21)
 #define ARITH_DIV_22(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(22,m)) (fn,m,ARITH_INCR(i),22)
 #define ARITH_DIV_23(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(23,m)) (fn,m,ARITH_INCR(i),23)
 #define ARITH_DIV_24(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(24,m)) (fn,m,ARITH_INCR(i),24)
 #define ARITH_DIV_25(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(25,m)) (fn,m,ARITH_INCR(i),25)
 #define ARITH_DIV_26(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(26,m)) (fn,m,ARITH_INCR(i),26)
 #define ARITH_DIV_27(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(27,m)) (fn,m,ARITH_INCR(i),27)
 #define ARITH_DIV_28(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(28,m)) (fn,m,ARITH_INCR(i),28)
 #define ARITH_DIV_29(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(29,m)) (fn,m,ARITH_INCR(i),29)
 #define ARITH_DIV_30(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(30,m)) (fn,m,ARITH_INCR(i),30)
 #define ARITH_DIV_31(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(31,m)) (fn,m,ARITH_INCR(i),31)
 #define ARITH_DIV_32(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(32,m)) (fn,m,ARITH_INCR(i),32)
 #define ARITH_DIV_33(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(33,m)) (fn,m,ARITH_INCR(i),33)
 #define ARITH_DIV_34(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(34,m)) (fn,m,ARITH_INCR(i),34)
 #define ARITH_DIV_35(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(35,m)) (fn,m,ARITH_INCR(i),35)
 #define ARITH_DIV_36(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(36,m)) (fn,m,ARITH_INCR(i),36)
 #define ARITH_DIV_37(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(37,m)) (fn,m,ARITH_INCR(i),37)
 #define ARITH_DIV_38(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(38,m)) (fn,m,ARITH_INCR(i),38)
 #define ARITH_DIV_39(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(39,m)) (fn,m,ARITH_INCR(i),39)
 #define ARITH_DIV_40(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(40,m)) (fn,m,ARITH_INCR(i),40)
 #define ARITH_DIV_41(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(41,m)) (fn,m,ARITH_INCR(i),41)
 #define ARITH_DIV_42(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(42,m)) (fn,m,ARITH_INCR(i),42)
 #define ARITH_DIV_43(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(43,m)) (fn,m,ARITH_INCR(i),43)
 #define ARITH_DIV_44(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(44,m)) (fn,m,ARITH_INCR(i),44)
 #define ARITH_DIV_45(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(45,m)) (fn,m,ARITH_INCR(i),45)
 #define ARITH_DIV_46(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(46,m)) (fn,m,ARITH_INCR(i),46)
 #define ARITH_DIV_47(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(47,m)) (fn,m,ARITH_INCR(i),47)
 #define ARITH_DIV_48(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(48,m)) (fn,m,ARITH_INCR(i),48)
 #define ARITH_DIV_49(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(49,m)) (fn,m,ARITH_INCR(i),49)
 #define ARITH_DIV_50(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(50,m)) (fn,m,ARITH_INCR(i),50)
 #define ARITH_DIV_51(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(51,m)) (fn,m,ARITH_INCR(i),51)
 #define ARITH_DIV_52(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(52,m)) (fn,m,ARITH_INCR(i),52)
 #define ARITH_DIV_53(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(53,m)) (fn,m,ARITH_INCR(i),53)
 #define ARITH_DIV_54(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(54,m)) (fn,m,ARITH_INCR(i),54)
 #define ARITH_DIV_55(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(55,m)) (fn,m,ARITH_INCR(i),55)
 #define ARITH_DIV_56(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(56,m)) (fn,m,ARITH_INCR(i),56)
 #define ARITH_DIV_57(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(57,m)) (fn,m,ARITH_INCR(i),57)
 #define ARITH_DIV_58(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(58,m)) (fn,m,ARITH_INCR(i),58)
 #define ARITH_DIV_59(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(59,m)) (fn,m,ARITH_INCR(i),59)
 #define ARITH_DIV_60(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(60,m)) (fn,m,ARITH_INCR(i),60)
 #define ARITH_DIV_61(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(61,m)) (fn,m,ARITH_INCR(i),61)
 #define ARITH_DIV_62(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(62,m)) (fn,m,ARITH_INCR(i),62)
 #define ARITH_DIV_63(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(63,m)) (fn,m,ARITH_INCR(i),63)
 #define ARITH_DIV_64(fn,m,i,p) ARITH_CATN( DIV,ARITH_SUB(64,m)) (fn,m,ARITH_INCR(i),64)


 #endif

